/*
  ==============================================================================

    ObjectsHelper.cpp
    Created: 3 Feb 2014 12:37:51am
    Author:  Peter Vasil

  ==============================================================================

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

#include "ObjectsHelper.h"

#include "Application/CommonHeaders.h"

#include "Controller/ObjController.h"

#include "Graph/Node.h"
#include "Graph/DirectedGraph.h"
#include "Graph/GraphUtils.h"

#include "Models/MDLFile.h"
#include "Models/ObjectFactory.h"

#include "View/SelectableObject.h"
#include "View/BaseObjectComponent.h"
#include "View/ObjectComponent.h"
#include "View/LinkComponent.h"
#include "View/Connector.h"
#include "View/CommentComponent.h"
#include "View/ObjectsHolder.h"

#include "Utilities/RegularExpression.h"

#include "IdManager.h"

#include <functional>


using namespace synthamodeler;

//------------------------------------------------------------------------------

bool ObjectsHelper::changeObjectNameInLink(const ObjController& objController,
                                           const String& oldName,
                                           const String& newName,
                                           UndoManager* undoManager)
{
    ObjectComponent* oc = objController.getObjectForId(newName);
    if (oc == nullptr)
        oc = objController.getObjectForId(oldName);

    if (oc == nullptr)
        return false;

    for (LinkComponent* const lc : oc->getAttachedLinks())
    {
        if (lc->getData()[Ids::startVertex] == oldName)
        {
            lc->getData().setProperty(Ids::startVertex, newName, undoManager);
        }
        else if (lc->getData()[Ids::endVertex] == oldName)
        {
            lc->getData().setProperty(Ids::endVertex, newName, undoManager);
        }
    }
    return true;
}

//------------------------------------------------------------------------------

void ObjectsHelper::changeObjectNameInAudioSources(const ObjController& objController,
                                                   const String& oldName,
                                                   const String& newName,
                                                   UndoManager* undoManager)
{
    const int numAudioConnections = objController.getNumAudioConnections();
    for (int i = 0; i < numAudioConnections; ++i)
    {
        Connector* conn = objController.getAudioConnector(i);

        ValueTree aoData = conn->getTargetObject()->getData();
        ValueTree sources = aoData.getChildWithName(Ids::sources);

        for (int j = 0; j < sources.getNumChildren(); ++j)
        {
            ValueTree source = sources.getChild(j);

            String srcStr = source.getProperty(Ids::value);
            if (srcStr.contains(oldName))
            {
                StringArray srcParts;
                srcParts.addTokens(srcStr, "*", "\"");
                int oldNameIdx = srcParts.indexOf(oldName);
                if (oldNameIdx == 0)
                {
                    srcParts.set(oldNameIdx, newName);
                    srcStr = srcParts.joinIntoString("*");
                    source.setProperty(Ids::value, srcStr, undoManager);
                }
                break;
            }
        }

    }
}

//------------------------------------------------------------------------------

void ObjectsHelper::tidyUpObjects(ObjController& objController)
{
    objController.startDragging();

    const int YTOLERANCE = 30;
    const int XTOLERANCE = 100;
    const int NHIST = 350;
    bool all = (objController.getSelectedObjects().getNumSelected() == 0);
    int numObjects = objController.getNumObjects();

    // tidy horizontally

    for (int i = 0; i < numObjects; ++i)
    {
        ObjectComponent* obj1 = objController.getObject(i);
        if (obj1->isSelected() || all)
        {
            for (int j = 0; j < numObjects; ++j)
            {
                ObjectComponent* obj2 = objController.getObject(j);
                if (obj2 != obj1 && (obj2->isSelected() || all))
                {
                    if (obj2->getPinPos().y <= obj1->getPinPos().y + YTOLERANCE &&
                        obj2->getPinPos().y >= obj1->getPinPos().y - YTOLERANCE &&
                        obj2->getPinPos().y != obj1->getPinPos().y)
                    {
                        int dx = 0;
                        int dy = obj1->getPinPos().y - obj2->getPinPos().y;
                        const int startX = obj2->getProperties() ["xDragStart"];
                        const int startY = obj2->getProperties() ["yDragStart"];
                        obj2->setActualPosition(Point<int>(startX + dx, startY + dy));
                    }
                }
            }
        }
    }

    // tidy vertically
    int histogram[NHIST], besthist = 0, bestdist = 50;
    for (int i = 0; i < NHIST; ++i)
        histogram[i] = 0;

    for (int i = 0; i < numObjects; ++i)
    {
        ObjectComponent* obj1 = objController.getObject(i);
        Rectangle<int> obj1rect = obj1->getBounds();
        if (obj1->isSelected() || all)
        {
            for (int j = 0; j < numObjects; ++j)
            {
                ObjectComponent* obj2 = objController.getObject(j);
                if (obj2 != obj1 && (obj2->isSelected() || all))
                {

                    if (obj2->getPinPos().x <= obj1->getPinPos().x + XTOLERANCE &&
                        obj2->getPinPos().x >= obj1->getPinPos().x - XTOLERANCE)
                    {
                        int distance = obj2->getPinPos().y - obj1->getPinPos().y;
                        if (distance >= 0 && distance < NHIST)
                            histogram[distance]++;
                    }
                }
            }
        }
    }
    //    for (int i = 2; i < (NHIST - 2); ++i)
    //    {
    //        int hit = 0.5 * histogram[i-2] + histogram[i-1] + 2 * histogram[i] + histogram[i+1] + 0.5 * histogram[i+2];
    //        if (hit > besthist)
    //        {
    //            besthist = hit;
    //            bestdist = i;
    //        }
    //    }
    for (int i = 1; i < (NHIST - 1); ++i)
    {
        int hit = histogram[i - 1] + 2 * histogram[i] + histogram[i + 1];
        if (hit > besthist)
        {
            besthist = hit;
            bestdist = i;
        }
    }
    DBG("best vertical distance " << bestdist);

    for (int i = 0; i < numObjects; ++i)
    {
        bool keep = true;
        ObjectComponent* obj1 = objController.getObject(i);
        int ax = obj1->getPinPos().x;
        int ay = obj1->getPinPos().y;
        if (obj1->isSelected() || all)
        {
            for (int j = 0; j < numObjects; ++j)
            {
                ObjectComponent* obj2 = objController.getObject(j);
                int bx = obj2->getPinPos().x;
                int by = obj2->getPinPos().y;
                if (obj2 != obj1 && (obj2->isSelected() || all))
                {
                    while (keep)
                    {
                        keep = false;

                        if (bx <= ax + XTOLERANCE && bx >= ax - XTOLERANCE &&
                            by > ay && by < ay + NHIST)
                        {
                            int vmove = ay + bestdist - by;
                            int dx = ax - bx;
                            int dy = vmove;
                            const int startX = obj2->getProperties() ["xDragStart"];
                            const int startY = obj2->getProperties() ["yDragStart"];
                            obj2->setActualPosition(Point<int>(startX + dx, startY + dy));

                            ay = by + vmove;

                            keep = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    objController.endDragging();
}

//------------------------------------------------------------------------------

void ObjectsHelper::makeGraph(const ObjController& objController, DirectedGraph& graph)
{
    // add nodes
    for (const LinkComponent* const l : objController.getLinks())
    {
        ObjectComponent* const ocStart =
            objController.getObjectForId(l->getData()[Ids::startVertex].toString());
        ObjectComponent* const ocEnd =
            objController.getObjectForId(l->getData()[Ids::endVertex].toString());

        graph.addNode(ocStart);
        graph.addNode(ocEnd);
    }

    tNodes& nodes = graph.getNodes();

    graph.init(nodes.size());

    // link nodes
    for (const LinkComponent* const l : objController.getLinks())
    {
        ObjectComponent* const ocStart =
            objController.getObjectForId(l->getData()[Ids::startVertex].toString());
        ObjectComponent* const ocEnd =
            objController.getObjectForId(l->getData()[Ids::endVertex].toString());

        graph.linkNodes(ocStart, ocEnd);
    }

    // init node data
    std::for_each(nodes.begin(), nodes.end(),
                  [](Node* const n) { n->initNodeData(); });

    // find combined groups in graph
    GraphUtils::calculateConnectedGroups(graph);

    // String outStr;
    // for (int i = 0; i < graph.edges.size(); ++i)
    // {
    //     for (int j = 0; j < graph.edges[i].size(); ++j)
    //     {
    //         outStr << (graph.edges[i][j] ? "1" : "0");
    //     }
    //     outStr << newLine;
    // }
    // DBG(outStr);
}

//------------------------------------------------------------------------------

String fixValue(const String& val)
{
    String tmpVal = val.trim();
    bool hasParentheses = false;
    String sign = String::empty;

    if(tmpVal.containsAnyOf("()"))
    {
        tmpVal = tmpVal.removeCharacters("()");
        hasParentheses = true;
    }
    if(tmpVal.startsWith("-") || tmpVal.startsWith("+"))
    {
        sign = tmpVal.substring(0, 1);
        tmpVal = tmpVal.substring(1);
    }

    if (tmpVal.containsOnly("0123456789") && tmpVal.indexOf(".") == -1)
    {
        // when value has integer notation, i.e 42 results in 42.0
        tmpVal << ".0";
    }
    else if(tmpVal.startsWith("."))
    {
        if(tmpVal.substring(1).containsOnly("0123456789"))
        {
            // when value has floating point notation but 0 is ommitted,
            // i.e .42 result in 0.42
            tmpVal = "0" + tmpVal;
        }
    }

    if(sign != String::empty)
    {
        tmpVal = sign + tmpVal;
    }
    if(hasParentheses)
    {
        tmpVal = "(" + tmpVal;
        tmpVal += ")";
    }
    return tmpVal;
}

//------------------------------------------------------------------------------

String ObjectsHelper::fixParameterValueIfNeeded(const String& paramVal)
{
    if( paramVal == String::empty)
        return "0.0";

    String tmpVal;
    StringArray operators;
    StringArray params;
    if(paramVal.containsAnyOf("*+-/"))
    {
        String tmp = "";
        for (int i = 0; i < paramVal.length(); ++i)
        {
            if(paramVal[i] == '*' || paramVal[i] == '+'
                || paramVal[i] == '/')
            {
                String op = "";
                op << paramVal[i];
                operators.add(op);
                params.add(tmp);
                tmp = "";
            }
            else if(paramVal[i] == '-' )
            {
                if(i == 0)
                {
                    tmp << paramVal[i];
                }
                else
                {
                    int j = 1;
                    while(paramVal[i - j] == ' ')
                    {
                        ++j;
                    }
                    if(paramVal[i-j] == '(')
                    {
                        tmp << paramVal[i];
                    }
                    else
                    {
                        String op = "";
                        op << paramVal[i];
                        operators.add(op);
                        params.add(tmp);
                        tmp = "";
                    }
                }
            }
            else
            {
                tmp << paramVal[i];
            }
        }
        if(tmp.compare("") != 0)
            params.add(tmp);

        for (int i = 0; i < params.size(); ++i)
        {
            if(params[i] == String::empty)
                tmpVal << "0.0";
            else
                tmpVal << fixValue(params[i]);

            if(i < operators.size())
                tmpVal << operators[i];
        }
    }
    else
    {
        tmpVal = fixValue(paramVal);
    }

    return tmpVal;
}

//------------------------------------------------------------------------------

String ObjectsHelper::getGainForSourceId(const ValueTree& sources, const String& sourceId)
{
    String gainStr;
    for (int i = 0; i < sources.getNumChildren(); ++i)
    {
        ValueTree source = sources.getChild(i);
        StringArray vals;
        vals.addTokens(source[Ids::value].toString(), "*", "\"");
        int srcIdx = -1;
        for (int j = 0; j < vals.size(); ++j)
        {
            if(vals[j].contains(sourceId))
            {
                srcIdx = j;
                break;
            }
        }
        if(srcIdx != -1)
        {
            int gainVals = 0;
            for (int j = 0; j < vals.size(); ++j)
            {
                if(j != srcIdx)
                {
                    if (gainVals >= 1)
                        gainStr << "*";
                    gainStr << vals[j];
                    ++ gainVals;
                }
            }
            break;
        }
    }
    return gainStr;
}

void ObjectsHelper::setGainForSourceId(const ValueTree& sources, const String& sourceId,
                                       const String& gainVal, UndoManager* undoManager)
{
    String gainStr;
    for (int i = 0; i < sources.getNumChildren(); ++i)
    {
        ValueTree source = sources.getChild(i);
        StringArray vals;
        vals.addTokens(source[Ids::value].toString(), "*", "\"");
        int srcIdx = -1;
        for (int j = 0; j < vals.size(); ++j)
        {
            if(vals[j].contains(sourceId))
            {
                srcIdx = j;
                break;
            }
        }
        if(srcIdx != -1)
        {
            gainStr << sourceId << "*";
            gainStr << gainVal;
            source.setProperty(Ids::value, gainStr, undoManager);
            break;
        }
    }
}

//------------------------------------------------------------------------------

BaseObjectComponent* ObjectsHelper::getBaseObjectFromSource(const ObjController& objController,
                                                            const ValueTree& source)
{
    const String& srcVal = source[Ids::value].toString();

    String sourceName = getObjectNameFromSource(
        objController.getIdManager().getAllIdsRegex(), srcVal);

    if (ObjectComponent* oc = objController.getObjectForId(sourceName))
    {
        return oc;
    }
    else if(LinkComponent* lc = objController.getLinkForId(sourceName))
    {
        return lc;
    }

    return nullptr;
}

//------------------------------------------------------------------------------

String ObjectsHelper::getObjectNameFromSource(const String& regex, const String& source)
{
    String result;

    RegularExpression re(regex);
    int found = re.match(source);

    if (found > 0)
    {
        int start, end;
        result = re.getStringOffsetSize(1, start, end);
    }

    return result;
}

//------------------------------------------------------------------------------

const Identifier& ObjectsHelper::getObjectGroup(const Identifier& ident)
{
    if (ObjectsHelper::isMass(ident))
        return Objects::masses;
    else if (ObjectsHelper::isLink(ident))
        return Objects::links;
    else if (ident == Ids::audioout)
        return Objects::audioobjects;
    else if (ident == Ids::waveguide)
        return Objects::waveguides;
    else if (ident == Ids::termination)
        return Objects::terminations;
    else if (ident == Ids::junction)
        return Objects::junctions;
    else if (ident == Ids::faustcode)
        return Objects::faustcodeblock;
    else if (ident == Ids::comment)
        return Objects::comments;
    else if (ident == Ids::display)
        return Objects::displays;

    else
        return Objects::invalid;
}

//------------------------------------------------------------------------------

bool ObjectsHelper::containsStringInValueTree(const ValueTree& valTree,
                                              const String& stringToSearch)
{
    for (int i = 0; i < valTree.getNumProperties(); ++i)
    {
        const Identifier& propName = valTree.getPropertyName(i);
        const String& propString = valTree.getProperty(propName).toString();
        if (propString.containsWholeWord(stringToSearch))
        {
            return true;
        }
    }

    for (int i = 0; i < valTree.getNumChildren(); ++i)
    {
        const ValueTree& child = valTree.getChild(i);
        if (containsStringInValueTree(child, stringToSearch))
        {
            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------

const Array<Identifier>& ObjectsHelper::getResonatorParamsIds()
{
    static const Identifier resonatorParamIdsArr[] = {
        Ids::resonatorsFreq, Ids::resonatorsDecay, Ids::resonatorsEqMass
    };
    static const int numAllResParams =
        sizeof(resonatorParamIdsArr)/sizeof(resonatorParamIdsArr[0]);
    static const Array<Identifier> resonatorParamIds(resonatorParamIdsArr,
                                                     numAllResParams);
    return resonatorParamIds;
}

//------------------------------------------------------------------------------

const Array<Identifier>& ObjectsHelper::getAllObjectIds()
{
    static const Identifier allObjectIdsArr[] = {
        Ids::mass, Ids::ground, Ids::port, Ids::resonators, Ids::link, Ids::touch,
        Ids::pluck, Ids::pulsetouch, Ids::faustcode, Ids::audioout, Ids::waveguide,
        Ids::termination, Ids::junction, Ids::comment, Ids::detent,
        Ids::softeninglink, Ids::stiffeninglink, Ids::display
    };
    static const int numAllObjects =
        sizeof(allObjectIdsArr)/sizeof(allObjectIdsArr[0]);
    static const Array<Identifier> allObjectIds(allObjectIdsArr, numAllObjects);
    return allObjectIds;
}

//------------------------------------------------------------------------------

template <typename ObjType>
Array<ObjType*> ObjectsHelper::getSelectedComponents(ObjController& objController)
{
    const SelectedItemSet<SelectableObject*>& sis = objController.getSelectedObjects();
    Array<ObjType*> selectedObjs;
    for (SelectableObject* const selectedItem : sis.getItemArray())
    {
        if (ObjType* const oc = dynamic_cast<ObjType*>(selectedItem))
        {
            selectedObjs.add(oc);
        }
    }

    return selectedObjs;
}

template Array<ObjectComponent*> ObjectsHelper::getSelectedComponents(ObjController& objController);
template Array<LinkComponent*> ObjectsHelper::getSelectedComponents(ObjController& objController);

//------------------------------------------------------------------------------

bool ObjectsHelper::allObjectsSelected(ObjController& objController)
{
    const int numSelected = objController.getSelectedObjects().getNumSelected();
    const int numAllObjects =
        objController.getNumObjects() +
        objController.getNumLinks() +
        objController.getNumAudioConnections() +
        objController.getNumComment();

    return numSelected == numAllObjects;
}

//------------------------------------------------------------------------------

void ObjectsHelper::loadComponents(ObjController& objController,
                                   ObjectsHolder& objHolder,
                                   const ValueTree& mdlRoot,
                                   int& numObjects, int& numZeroPos)
{
    IdManager& idMgr = objController.getIdManager();

    // helper functions

    auto fnObjectsCounter = [&numObjects, &numZeroPos](ValueTree objTree)
    {
        ++numObjects;
        if(float(objTree[Ids::posX]) < 0.00001f && float(objTree[Ids::posY]) < 0.00001f)
            ++numZeroPos;
    };

    auto fnAddObjectComponent = [&](const ValueTree& objTree)
    {
        ObjectComponent* const oc = new ObjectComponent(objController, objTree);
        addComp(objController, objHolder, oc);
        objHolder.checkExtent(oc->getBounds());
        fnObjectsCounter(objTree);
    };

    auto fnAddLinkComponent = [&](const ValueTree& linkTree)
    {
        try
        {
            addComp(objController, objHolder,
                    new LinkComponent(objController, linkTree))->toBack();
        }
        catch (const std::exception& e)
        {
            SAM_CONSOLE(e.what(), PostLevel::ERROR);
        }
    };

    auto fnIterateObjectGroup = [&](const Identifier& groupId,
                                    const std::function<void(const ValueTree&)>& addFunctor)
    {
        ValueTree objGroup = mdlRoot.getChildWithName(groupId);
        for (int i = 0; i < objGroup.getNumChildren(); ++i)
        {
            const ValueTree obj = objGroup.getChild(i);
            const String& objName = obj[Ids::identifier].toString();

            if (idMgr.addId(obj.getType(), objName, nullptr))
            {
                addFunctor(obj);
            }
            else
            {
                SAM_LOG("Couldn't add duplicate Object " + objName);
            }
        }
    };

    // objects and links

    fnIterateObjectGroup(Objects::masses, fnAddObjectComponent);
    fnIterateObjectGroup(Objects::terminations, fnAddObjectComponent);
    fnIterateObjectGroup(Objects::junctions, fnAddObjectComponent);
    fnIterateObjectGroup(Objects::links, fnAddLinkComponent);
    fnIterateObjectGroup(Objects::waveguides, fnAddLinkComponent);

    // audio objects and comments

    fnIterateObjectGroup(Objects::audioobjects, [&](const ValueTree& aoTree)
    {
        fnAddObjectComponent(aoTree);

        ObjectComponent* const audioOutComp =
            objController.getObjectForId(aoTree[Ids::identifier].toString());

        ValueTree aoSources = aoTree.getChildWithName(Ids::sources);
        for (int j = 0; j < aoSources.getNumChildren(); ++j)
        {
            ValueTree source = aoSources.getChild(j);
            BaseObjectComponent* sourceComp =
                ObjectsHelper::getBaseObjectFromSource(objController, source);

            if (sourceComp != nullptr)
            {
                Connector* conn = new Connector(
                    objController, sourceComp, audioOutComp);
                objController.addAudioConnectionComp(conn);
                objHolder.addAndMakeVisible(conn);
                conn->update();
                objHolder.checkExtent(conn->getBounds());
            }
        }
    });

    fnIterateObjectGroup(Objects::comments, [&](const ValueTree& comment)
    {
        addComp(objController, objHolder,
                new CommentComponent(objController, comment))->update();
        fnObjectsCounter(comment);
    });
}

//------------------------------------------------------------------------------

bool ObjectsHelper::isMass(const Identifier& id)
{
    return id == Ids::mass || id == Ids::ground ||
        id == Ids::port || id == Ids::resonators;
}

//------------------------------------------------------------------------------

bool ObjectsHelper::isLink(const Identifier& id)
{
    return id == Ids::link || id == Ids::touch || id == Ids::pluck ||
        id == Ids::pulsetouch || id == Ids::detent ||
        id == Ids::softeninglink || id == Ids::stiffeninglink;
}

//------------------------------------------------------------------------------

template <typename ObjType>
ObjType* ObjectsHelper::addComp(ObjController& objController, ObjectsHolder& objHolder,
                                ObjType* const obj)
{
    objController.addComp(obj);
    objHolder.addAndMakeVisible(obj);
    const ValueTree& objData = obj->getData();
    SAM_LOG("Load " + objData.getType().toString() + " " + objData[Ids::identifier].toString());
    return obj;
}

template ObjectComponent* ObjectsHelper::addComp<ObjectComponent>(ObjController&, ObjectsHolder&, ObjectComponent*);
template LinkComponent* ObjectsHelper::addComp<LinkComponent>(ObjController&, ObjectsHolder&, LinkComponent*);
template CommentComponent* ObjectsHelper::addComp<CommentComponent>(ObjController&, ObjectsHolder&, CommentComponent*);

//------------------------------------------------------------------------------

bool ObjectsHelper::canObjectsBeConnected(const ObjectComponent& oc1,
                                          const ObjectComponent& oc2,
                                          const juce::Identifier& linkType)
{
    return oc1.canBeConnected(linkType) && oc2.canBeConnected(linkType);
}

//------------------------------------------------------------------------------

bool ObjectsHelper::canSelectedObjectsBeConnected(ObjController& objController,
                                                  const Identifier& linkType)
{
    const Array<ObjectComponent*> selectedObjects =
        ObjectsHelper::getSelectedComponents<ObjectComponent>(objController);

    return selectedObjects.size() == 2 &&
        ObjectsHelper::canObjectsBeConnected(
            *selectedObjects[0], *selectedObjects[1], linkType);
}

//------------------------------------------------------------------------------

bool ObjectsHelper::equivalentById(const ValueTree& lhs, const ValueTree& rhs)
{
    return lhs[Ids::identifier] == rhs[Ids::identifier];
}

//------------------------------------------------------------------------------

ObjectsHelper::tSourceTargetPair ObjectsHelper::getSourceTargetPairFromSelection(
    SelectableObject* obj1, SelectableObject* obj2, const Identifier& targetType)
{
    ObjectComponent* oc1 = dynamic_cast<ObjectComponent*>(obj1);
    LinkComponent* lc1 = dynamic_cast<LinkComponent*>(obj1);
    ObjectComponent* oc2 = dynamic_cast<ObjectComponent*>(obj2);
    LinkComponent* lc2 = dynamic_cast<LinkComponent*>(obj2);

    if (oc1 && oc2)
    {
        const ValueTree& data1 = oc1->getData();
        const ValueTree& data2 = oc2->getData();

        if (data1.getType() == targetType && data2.getType() != targetType)
        {
            return tSourceTargetPair(oc2, oc1);
        }
        else if (data1.getType() != targetType && data2.getType() == targetType)
        {
            return tSourceTargetPair(oc1, oc2);
        }
    }
    else if (oc1 && lc2 && !oc2 && !lc1 &&
             oc1->getData().getType() == targetType &&
             lc2->getData().getType() != Ids::waveguide)
    {
        return tSourceTargetPair(lc2, oc1);
    }
    else if (oc2 && lc1 && !oc1 && !lc2 &&
             oc2->getData().getType() == targetType &&
             lc1->getData().getType() != Ids::waveguide)
    {
        return tSourceTargetPair(lc1, oc2);
    }

    return tSourceTargetPair(nullptr,nullptr);
}

//------------------------------------------------------------------------------

#if UNIT_TESTS

#include "ObjectsHelper_test.h"

#endif
