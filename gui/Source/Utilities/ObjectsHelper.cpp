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

#include "../Application/CommonHeaders.h"
#include "../View/SelectableObject.h"
#include "../Graph/Node.h"
#include "../Graph/DirectedGraph.h"
#include "../View/BaseObjectComponent.h"
#include "../View/ObjectComponent.h"
#include "../View/LinkComponent.h"
#include "../View/AudioOutConnector.h"
#include "../Controller/ObjController.h"
#include "../Graph/GraphUtils.h"

#include "ObjectsHelper.h"

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

    Array<LinkComponent*> linksTmp = oc->getAttachedLinks();
    std::for_each(linksTmp.begin(), linksTmp.end(), [&](LinkComponent* const lc)
    {
        if (lc->getData()[Ids::startVertex] == oldName)
        {
            lc->getData().setProperty(Ids::startVertex, newName, undoManager);
        }
        else if (lc->getData()[Ids::endVertex] == oldName)
        {
            lc->getData().setProperty(Ids::endVertex, newName, undoManager);
        }
    });
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
        AudioOutConnector* aoc = objController.getAudioConnector(i);

        ValueTree aoData = aoc->getAudioObject()->getData();
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
                        Point<int> r(obj2->getPosition());
                        r.setXY(startX + dx, startY + dy);
                        obj2->setPosition(Point<int>(r.x + obj2->getWidth() / 2, r.y + obj2->getHeight() / 2), true);
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

                    //                    Rectangle<int> obj2rect = obj2->getBounds();
                    //                    if (obj2rect.getX() <= obj1rect.getRight() + XTOLERANCE &&
                    //                        obj2rect.getX() >= obj1->getPinPos().x - XTOLERANCE)
                    //
                    //                    int px2 = obj2->getPinPos().x;
                    //                    int px1 = obj1->getPinPos().x;
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
                            Point<int> r(obj2->getPosition());
                            r.setXY(startX + dx, startY + dy);
                            obj2->setPosition(Point<int>(r.x + obj2->getWidth() / 2, r.y + obj2->getHeight() / 2), true);

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
    const int numLinks = objController.getNumLinks();

    // add nodes
    for (int i = 0; i < numLinks; ++i)
    {
        const LinkComponent* const l = objController.getLink(i);

        ObjectComponent* const ocStart =
            objController.getObjectForId(l->getData()[Ids::startVertex].toString());
        ObjectComponent* const ocEnd =
            objController.getObjectForId(l->getData()[Ids::endVertex].toString());

        graph.addNode(ocStart);
        graph.addNode(ocEnd);
    }

    tNodes& nodes = graph.nodes;

    graph.init(nodes.size());

    // link nodes
    for (int i = 0; i < numLinks; ++i)
    {
        const LinkComponent* const l = objController.getLink(i);

        ObjectComponent* const ocStart =
            objController.getObjectForId(l->getData()[Ids::startVertex].toString());
        ObjectComponent* const ocEnd =
            objController.getObjectForId(l->getData()[Ids::endVertex].toString());

        graph.linkNodes(ocStart, ocEnd);
    }

    // init node data
    std::for_each(nodes.begin(), nodes.end(),
                  [](Node* const n) {n->initNodeData();});

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

String ObjectsHelper::getGainForSourceId(ValueTree& sources, const String& sourceId)
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

void ObjectsHelper::setGainForSourceId(ValueTree& sources, const String& sourceId,
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

BaseObjectComponent* ObjectsHelper::getBaseObjectFromSource(ObjController* objController,
                                                            ValueTree& source)
{
    String srcVal = source[Ids::value].toString();
    StringArray srcArray;
    srcArray.addTokens(srcVal, "*", "\"");
    for (int i = 0; i < srcArray.size(); ++i)
    {
        if (ObjectComponent* oc = objController->getObjectForId(srcArray[i]))
        {
            return oc;
        }
        else if(LinkComponent* lc = objController->getLinkForId(srcArray[i]))
        {
            return lc;
        }
    }
    return nullptr;
}

//------------------------------------------------------------------------------

const Identifier& ObjectsHelper::getObjectGroup(const Identifier& ident)
{
    if(ident == Ids::mass || ident == Ids::port
            || ident == Ids::ground	|| ident == Ids::resonators)
        return Objects::masses;
    else if(ident == Ids::link || ident == Ids::touch
            || ident == Ids::pluck || ident == Ids::pulsetouch)
        return Objects::links;
    else if(ident == Ids::audioout)
        return Objects::audioobjects;
    else if(ident == Ids::waveguide)
        return Objects::waveguides;
    else if(ident == Ids::termination)
        return Objects::terminations;
    else if(ident == Ids::junction)
        return Objects::junctions;
    else if(ident == Ids::faustcode)
        return Objects::faustcodeblock;
    else if(ident == Ids::comment)
        return Objects::comments;

    else
        return Objects::invalid;
}

//------------------------------------------------------------------------------

bool ObjectsHelper::containsStringInValueTree(ValueTree valTree,
                                              const String& searchStr,
                                              bool isRoot)
{
    if (isRoot)
    {
        for (int k = 0; k < valTree.getNumProperties(); ++k)
        {
            if (valTree.getProperty(valTree.getPropertyName(k)).toString().containsWholeWord(searchStr))
            {
                return true;
            }
        }
    }
    else
    {
        for (int k = 0; k < valTree.getParent().getNumChildren(); ++k)
        {
            ValueTree c = valTree.getParent().getChild(k);
            for (int j = 0; j < c.getNumProperties(); ++j)
            {
                if (c.getProperty(c.getPropertyName(j)).toString().containsWholeWord(searchStr))
                {
                    return true;
                }
            }
        }
    }
    for (int i = 0; i < valTree.getNumChildren(); ++i)
    {
        ValueTree c = valTree.getChild(i);
        return containsStringInValueTree(c, searchStr, false);
    }
    return false;
}

//------------------------------------------------------------------------------

const Array<Identifier>& ObjectsHelper::getResonatorParamsIds()
{
    static const Identifier resonatorParamIdsArr[] = {
        Ids::resonatorsFreq,
        Ids::resonatorsDecay,
        Ids::resonatorsEqMass
    };
    static const Array<Identifier> resonatorParamIds(resonatorParamIdsArr, 3);
    return resonatorParamIds;
}

//------------------------------------------------------------------------------

const Array<Identifier>& ObjectsHelper::getAllObjectIds()
{
    static const Identifier allObjectIdsArr[] = {
        Ids::mass,
        Ids::ground,
        Ids::port,
        Ids::resonators,
        Ids::link,
        Ids::touch,
        Ids::pluck,
        Ids::pulsetouch,
        Ids::faustcode,
        Ids::audioout,
        Ids::waveguide,
        Ids::termination,
        Ids::junction,
        Ids::comment
    };
    static const Array<Identifier> allObjectIds(allObjectIdsArr, 14);
    return allObjectIds;
}

//------------------------------------------------------------------------------

template <typename BaseCompType>
ObjectComponent* ObjectsHelper::getObject(BaseCompType* baseCompType)
{
    return dynamic_cast<ObjectComponent*>(baseCompType);
}

template ObjectComponent* ObjectsHelper::getObject(BaseObjectComponent*);
template ObjectComponent* ObjectsHelper::getObject(SelectableObject*);
template ObjectComponent* ObjectsHelper::getObject(Component*);

//------------------------------------------------------------------------------

template <typename BaseCompType>
LinkComponent* ObjectsHelper::getLink(BaseCompType* baseCompType)
{
    return dynamic_cast<LinkComponent*>(baseCompType);
}

template LinkComponent* ObjectsHelper::getLink(BaseObjectComponent*);
template LinkComponent* ObjectsHelper::getLink(SelectableObject*);

//------------------------------------------------------------------------------
