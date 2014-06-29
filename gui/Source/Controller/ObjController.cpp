/*
  ==============================================================================

    ObjController.cpp
    Created: 13 Apr 2012 12:06:25am
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
#include "../Models/ObjectActions.h"
#include "../Models/MDLFile.h"
#include "../Models/ObjectFactory.h"
#include "../View/ObjectComponent.h"
#include "../View/LinkComponent.h"
#include "../View/AudioOutConnector.h"
#include "MDLController.h"
#include "../Utilities/IdManager.h"
#include "../View/CommentComponent.h"
//#include "../Graph/FlowAlgorithm.h"
//#include "../Graph/ForceDirectedFlowAlgorithm.h"
#include "../Graph/DirectedGraph.h"
#include "../Utilities/ObjectsHelper.h"
#include "GraphReflowController.h"

#include "ObjController.h"

#include <algorithm>

using namespace synthamodeler;

ObjController::ObjController(MDLController& owner_)
: owner(owner_),
  timesPasted(0),
  isReflowing(false),
  reflowController(new GraphReflowController(*this))
{
    idMgr = new IdManager();
    sObjects.addChangeListener(propertiesWindow);
}

ObjController::~ObjController()
{
    audioConnections.clear(true);
    links.clear(true);
    objects.clear(true);
    comments.clear(true);
    idMgr = nullptr;
    sObjects.removeChangeListener(propertiesWindow);
    masterReference.clear();
    reflowController = nullptr;
}

bool ObjController::perform(UndoableAction * const action, const String& actionName)
{
    return owner.perform(action, actionName);
}

ObjectComponent* ObjController::addObject(ObjectsHolder* holder, ValueTree objValues, int index, bool undoable)
{
    if(undoable)
    {
        AddObjectAction* action = new AddObjectAction(this, objValues, holder);
        owner.getUndoManager().perform(action, "Add new Object");

        return objects[action->indexAdded];
    }
    else
    {
        const Identifier& groupName = ObjectsHelper::getObjectGroup(objValues.getType().toString());
        ValueTree mdl = owner.getMDLTree();
        ValueTree subTree = mdl.getOrCreateChildWithName(groupName, nullptr);

		subTree.addChild(objValues,-1, nullptr);
        idMgr->addId(objValues.getType(), objValues[Ids::identifier].toString(), nullptr);

        ObjectComponent* objComp = new ObjectComponent(*this, objValues);
        objects.insert(index, objComp);

        holder->addAndMakeVisible(objComp);
        changed();
        return objComp;
    }
    return 0;
}

void ObjController::addNewObject(ObjectsHolder* holder, ValueTree objValues)
{

    ObjectComponent* oc = addObject(holder, objValues, -1, true);
    sObjects.selectOnly(oc);

}

LinkComponent* ObjController::addLink(ObjectsHolder* holder, ValueTree linkValues, int index, bool undoable)
{
    if(undoable)
    {
        AddLinkAction* action = new AddLinkAction(this, linkValues, holder);
        owner.getUndoManager().perform(action, "Add new Link");

        return links[action->indexAdded];
    }
    else
    {
        const Identifier& gruopName = ObjectsHelper::getObjectGroup(linkValues.getType());
        ValueTree mdl = owner.getMDLTree();
        ValueTree subTree = mdl.getOrCreateChildWithName(gruopName, nullptr);
		subTree.addChild(linkValues,-1, nullptr);
        idMgr->addId(linkValues.getType(), linkValues[Ids::identifier].toString(), nullptr);

        LinkComponent* linkComp = new LinkComponent(*this, linkValues);
        links.insert(index, linkComp);

		holder->addAndMakeVisible(linkComp);
        linkComp->toBack();
        changed();
        return linkComp;
    }
    return 0;
}

void ObjController::addNewLink(ObjectsHolder* holder, ValueTree linkValues)
{
    addLink(holder, linkValues, -1, true);
}

bool ObjController::checkIfLinkExitsts(ValueTree linkTree)
{
    return std::any_of(links.begin(), links.end(), [&](const LinkComponent* const lc)
    {
        return lc->sameStartEnd(linkTree);
    });
}

bool ObjController::checkIfAudioConnectionExitsts(ValueTree source,
                                                  ValueTree audioOut)
{
    auto fnCompareAudioConnections = [&](const AudioOutConnector* const aoc)
    {
        const ValueTree& sourceComp = aoc->getSourceObject()->getData();
        const ValueTree& aoComp = aoc->getAudioObject()->getData();

        return sourceComp[Ids::identifier] == source[Ids::identifier]
            && aoComp[Ids::identifier] == audioOut[Ids::identifier];
    };
    return std::any_of(audioConnections.begin(), audioConnections.end(),
                       fnCompareAudioConnections);
}

void ObjController::addNewLinkIfPossible(ObjectsHolder* holder, ValueTree linkValues)
{
    Array<ObjectComponent*> selectedObjects;
    ObjectsHelper::getSelectedObjectComponents(*this, selectedObjects);

    if(selectedObjects.size() == 2)
    {
        ObjectComponent* oc1 = selectedObjects[0];
        ObjectComponent* oc2 = selectedObjects[1];

        if (oc1 != nullptr && oc2 != nullptr &&
            oc1->canBeConnected(linkValues.getType()) &&
            oc2->canBeConnected(linkValues.getType()) &&
            !checkIfLinkExitsts(linkValues))
        {
            addLink(holder, linkValues, -1, true);
        }
        else
        {
            String msg = "Cannot connect ";
            msg << oc1->getData().getType().toString();
            msg << " and ";
            msg << oc2->getData().getType().toString();
            msg << " with ";
            msg << linkValues.getType().toString();
            msg << ".";
            SAM_CONSOLE(msg, PostLevel::ERROR);
        }
    }
    else
    {
        SAM_CONSOLE("Please select 2 Objects", PostLevel::ERROR);
    }
}

AudioOutConnector* ObjController::addAudioConnection(ObjectsHolder* holder,
                                                     BaseObjectComponent* objComp,
                                                     ObjectComponent* audioOutComp,
                                                     ValueTree source,
                                                     int index,
                                                     bool undoable)
{
    if(undoable)
    {
        AddAudioConnectionAction* action = new AddAudioConnectionAction(this,
                                                                        objComp,
                                                                        source,
                                                                        audioOutComp,
                                                                        holder);
        owner.getUndoManager().perform(action, "Add new audio connection");

        return audioConnections[action->indexAdded];
    }
    else
    {
        AudioOutConnector* aoc = new AudioOutConnector(*this, objComp, audioOutComp);
        ValueTree sources = aoc->getAudioObject()->getData().getChildWithName(Ids::sources);
        if(! sources.getChildWithProperty(Ids::value, source[Ids::value]).isValid())
            sources.addChild(source, -1, nullptr);
        audioConnections.insert(index, aoc);
        holder->addAndMakeVisible(aoc);
        aoc->update();
        aoc->toBack();
        return aoc;
    }
    return nullptr;
}

void ObjController::addNewAudioConnection(ObjectsHolder* holder)
{
    if(sObjects.getNumSelected() == 2)
    {
        if(! StoredSettings::getInstance()->getShowAudioConnections())
        {
            StoredSettings::getInstance()->setShowAudioConnections(true);
            setAudioConnectionVisibility(true);
        }

        ObjectComponent* oc1 = ObjectsHelper::getObject(sObjects.getSelectedItem(0));
        LinkComponent* lc1 = ObjectsHelper::getLink(sObjects.getSelectedItem(0));
        ObjectComponent* oc2 = ObjectsHelper::getObject(sObjects.getSelectedItem(1));
        LinkComponent* lc2 = ObjectsHelper::getLink(sObjects.getSelectedItem(1));

        if(oc1 != nullptr && oc2 != nullptr)
        {
            if (oc1->getData().getType() == Ids::audioout
                && oc2->getData().getType() != Ids::audioout)
            {
                if(! checkIfAudioConnectionExitsts(oc2->getData(), oc1->getData()))
                {
                    ValueTree src = ObjectFactory::createAudioSourceTree(
                        oc2->getData()[Ids::identifier].toString(), "*1.0");
//                    ValueTree sources = oc1->getData().getOrCreateChildWithName(Ids::sources)
                    addAudioConnection(holder, oc2, oc1, src, -1, true);
                }
            }
            else if (oc1->getData().getType() != Ids::audioout
                && oc2->getData().getType() == Ids::audioout)
            {
                if(! checkIfAudioConnectionExitsts(oc1->getData(), oc2->getData()))
                {
                    ValueTree src = ObjectFactory::createAudioSourceTree(
                        oc1->getData()[Ids::identifier].toString(), "*1.0");
                    addAudioConnection(holder, oc1, oc2, src, -1, true);
                }
            }
            else
            {
                SAM_CONSOLE("Cannot create audio connection", PostLevel::ERROR);
            }
        }
        else if(oc1 == nullptr && oc2 != nullptr && lc1 != nullptr
            && lc1->getData().getType() != Ids::waveguide)
        {
            if( ! checkIfAudioConnectionExitsts(lc1->getData(), oc2->getData()))
            {
                ValueTree src = ObjectFactory::createAudioSourceTree(
                    lc1->getData()[Ids::identifier].toString(), "*1.0");
                addAudioConnection(holder, lc1, oc2, src, -1, true);
            }
        }
        else if(oc2 == nullptr && oc1 != nullptr && lc2 != nullptr
            && lc2->getData().getType() != Ids::waveguide)
        {
            if( ! checkIfAudioConnectionExitsts(lc2->getData(), oc1->getData()))
            {
                ValueTree src = ObjectFactory::createAudioSourceTree(
                    lc2->getData()[Ids::identifier].toString(), "*1.0");
                addAudioConnection(holder, lc2, oc1, src, -1, true);
            }
        }
        else
        {
            SAM_CONSOLE("Cannot create audio connection", PostLevel::ERROR);
        }
    }
}

CommentComponent* ObjController::addComment(ObjectsHolder* holder,
                                            ValueTree commentValues,
                                            int index, bool undoable)
{
    if(undoable)
    {
        AddCommentAction* action = new AddCommentAction(this, commentValues, holder);
        owner.getUndoManager().perform(action, "Add new Comment");

        return comments[action->indexAdded];
    }
    else
    {
        const Identifier& groupName = ObjectsHelper::getObjectGroup(commentValues.getType().toString());
        ValueTree mdl = owner.getMDLTree();
        ValueTree subTree = mdl.getOrCreateChildWithName(groupName, nullptr);

        subTree.addChild(commentValues,-1, nullptr);
        idMgr->addId(commentValues.getType(),
                     commentValues[Ids::identifier].toString(),
                     nullptr);

        CommentComponent* commentComp = new CommentComponent(*this, commentValues);
        comments.insert(index, commentComp);

        holder->addAndMakeVisible(commentComp);
        commentComp->update();
        changed();
        return commentComp;
    }
}

void ObjController::addNewComment(ObjectsHolder* holder, ValueTree commentValues)
{
    CommentComponent* cc = addComment(holder, commentValues, -1, true);
    sObjects.selectOnly(cc);
}


void ObjController::removeObject(ObjectComponent* objComp, bool undoable, ObjectsHolder* holder)
{
    if (undoable)
    {
        owner.getUndoManager().perform(new RemoveObjectAction(holder, objComp, this));
    }
    else
    {
        sObjects.deselect(objComp);
        sObjects.changed(true);

        // Get link indices attached to this object and remove them.
        // TODO needs better solution
        Array<int> lIndices = checkIfObjectHasLinks(objComp->getData());
//        if(indices.size() > 0)
//            sObjects.deselectAll();
        for(int i = lIndices.size(); --i >= 0;)
        {
            removeLink(getLink(lIndices[i]), true, holder);
        }
        Array<int> aoIndices = checkIfObjectHasAudioConnections(objComp->getData());
        for(int i = aoIndices.size(); --i >= 0;)
        {
            // remove audioouts attached to this object only if this is not an audioout
            bool isAudioOut = objComp->getData().getType() == Ids::audioout;
            removeAudioConnection(getAudioConnector(aoIndices[i]), !isAudioOut, holder);
        }

        const Identifier& groupName = ObjectsHelper::getObjectGroup(objComp->getData().getType());
        ValueTree mdl = owner.getMDLTree();
        ValueTree subTree = mdl.getOrCreateChildWithName(groupName, nullptr);
        idMgr->removeId(objComp->getData().getType(),
                        objComp->getData()[Ids::identifier].toString(), nullptr);
        subTree.removeChild(objComp->getData(), nullptr);
        objects.removeObject(objComp);
    }

}

void ObjController::removeSelectedObjects(ObjectsHolder* holder)
{
    SelectedItemSet <SelectableObject*> temp;
    temp = sObjects;

    if (temp.getNumSelected() > 0)
    {
        sObjects.deselectAll();
        sObjects.changed(true);
        // first remove all selected links
        for (int i = temp.getNumSelected(); --i >= 0;)
        {
            if(LinkComponent* lc = ObjectsHelper::getLink(temp.getSelectedItem(i)))
            {
                temp.deselect(lc);
                removeLink(lc, true, holder);
            }
        }
        // then objects and remaining links connected to the objects
        for (int i = temp.getNumSelected(); --i >= 0;)
        {
            if(AudioOutConnector* aoc = dynamic_cast<AudioOutConnector*>(temp.getSelectedItem(i)))
            {
                temp.deselect(aoc);
                removeAudioConnection(aoc, true, holder);
                continue;
            }
        }
        for (int i = temp.getNumSelected(); --i >= 0;)
        {
            if(ObjectComponent* oc = ObjectsHelper::getObject(temp.getSelectedItem(i)))
            {
                removeObject(oc, true, holder);
            }
            else if(CommentComponent* cc = ObjectsHelper::getComment(temp.getSelectedItem(i)))
            {
                removeComment(cc, true, holder);
            }
//            LinkComponent* lc = ObjectsHelper::getLink(temp.getSelectedItem(i));
//            if(lc != nullptr)
//            {
//                removeLink(lc, true, holder);
//                continue;
//            }

        }
    }
}

void ObjController::removeAudioConnection(AudioOutConnector* aocComp,
                                          bool undoable,
                                          ObjectsHolder* holder)
{
    if(undoable)
    {
        owner.getUndoManager().perform(new RemoveAudioConnectionAction(holder, aocComp, this));
    }
    else
    {
        sObjects.deselect(aocComp);
        sObjects.changed(true);
        ValueTree sources = aocComp->getAudioObject()->getData().getChildWithName(Ids::sources);
        ValueTree source;
        for (int i = 0; i < sources.getNumChildren(); ++i)
        {
            String val = sources.getChild(i)[Ids::value];
//            DBG(aocComp->getSourceObject()->getData()[Ids::identifier].toString());
            if(val.contains(aocComp->getSourceObject()->getData()[Ids::identifier].toString()))
            {
                source = sources.getChild(i);
                break;
            }
        }
        sources.removeChild(source, nullptr);
        audioConnections.removeObject(aocComp);
    }
}

void ObjController::removeLink(LinkComponent* linkComp, bool undoable, ObjectsHolder* holder)
{
    if (undoable)
    {
        owner.getUndoManager().perform(new RemoveLinkAction(holder, linkComp, this));
    }
    else
    {
        sObjects.deselect(linkComp);
        sObjects.changed(true);

        Array<int> aoIndices = checkIfObjectHasAudioConnections(linkComp->getData());
        for(int i = aoIndices.size(); --i >= 0;)
        {
            removeAudioConnection(getAudioConnector(aoIndices[i]), true, holder);
        }

        const Identifier& groupName = ObjectsHelper::getObjectGroup(linkComp->getData().getType());
        ValueTree mdl = owner.getMDLTree();
        ValueTree subTree = mdl.getOrCreateChildWithName(groupName, nullptr);
        idMgr->removeId(linkComp->getData().getType(),
                        linkComp->getData()[Ids::identifier].toString(), nullptr);
        subTree.removeChild(linkComp->getData(), nullptr);
        links.removeObject(linkComp);
    }
}

void ObjController::removeComment(CommentComponent* commentComp,
                                  bool undoable, ObjectsHolder* holder)
{
    if(undoable)
    {
        owner.getUndoManager().perform(new RemoveCommentAction(holder, commentComp, this));
    }
    else
    {
        sObjects.deselect(commentComp);
        sObjects.changed(true);

        const Identifier& groupName = ObjectsHelper::getObjectGroup(commentComp->getData().getType());
        ValueTree mdl = owner.getMDLTree();
        ValueTree subTree = mdl.getOrCreateChildWithName(groupName, nullptr);
        idMgr->removeId(commentComp->getData().getType(),
                        commentComp->getData()[Ids::identifier].toString(), nullptr);
        subTree.removeChild(commentComp->getData(), nullptr);
        comments.removeObject(commentComp);
    }
}

void ObjController::loadComponents(ObjectsHolder& holder)
{

    int numObjects = 0;
    int numZeroPos = 0;

    ObjectsHelper::loadComponents(*this, holder, *owner.getMDLFile(),
                                  numObjects, numZeroPos);

    setAudioConnectionVisibility(StoredSettings::getInstance()->getShowAudioConnections());

    if(StoredSettings::getInstance()->getShouldRedrawOnLoad())
        if(numZeroPos >= numObjects || numZeroPos > 1)
            holder.redrawObjects(CommandIDs::redrawForceDirected);


    auto fnObjectToFront = [](Component* const comp)
    {
        comp->toFront(false);
    };
    std::for_each(objects.begin(), objects.end(), fnObjectToFront);
    std::for_each(comments.begin(), comments.end(), fnObjectToFront);

}

void ObjController::selectAll(bool shouldBeSelected)
{
    sObjects.deselectAll();

    if (shouldBeSelected)
    {
        auto fnAddToSelection = [&](SelectableObject* const obj)
        {
            sObjects.addToSelection(obj);
        };

        std::for_each(objects.begin(), objects.end(), fnAddToSelection);
        std::for_each(links.begin(), links.end(), fnAddToSelection);
        std::for_each(audioConnections.begin(), audioConnections.end(), fnAddToSelection);
        std::for_each(comments.begin(), comments.end(), fnAddToSelection);
    }
}

void ObjController::selectObjectsIfContainsText(const String& selectionText)
{
    sObjects.deselectAll();

    if (selectionText.isEmpty())
        return;

    auto fnSelectIfContainsText = [&](const ValueTree& data_, SelectableObject* const obj)
    {
        if (ObjectsHelper::containsStringInValueTree(data_, selectionText, true))
        {
            sObjects.addToSelection(obj);
        }
    };

    for (ObjectComponent* const oc : objects)
    {
        fnSelectIfContainsText(oc->getData(), oc);
    }
    for (LinkComponent* const lc : links)
    {
        fnSelectIfContainsText(lc->getData(), lc);
    }
}

void ObjController::startDragging()
{
    auto fnSetDragStart = [](BaseObjectComponent* const boc)
    {
        const Point<int>& r = boc->getCenter();
        boc->getProperties().set("xDragStart", r.getX());
        boc->getProperties().set("yDragStart", r.getY());
    };
    std::for_each(objects.begin(), objects.end(), fnSetDragStart);
    std::for_each(comments.begin(), comments.end(), fnSetDragStart);

    owner.getUndoManager().beginNewTransaction();
}

void ObjController::dragSelectedComps(int dx, int dy)
{
    const ObjectsHolder& holder = *owner.getHolderComponent();

    for (SelectableObject* const selectedItem : sObjects.getItemArray())
    {
        if(ObjectComponent * const c = ObjectsHelper::getObject(selectedItem))
        {
            const int startX = c->getProperties() ["xDragStart"];
            const int startY = c->getProperties() ["yDragStart"];

            c->setActualPosition(Point<int>(holder.snapPosition(startX + dx),
                                            holder.snapPosition(startY + dy)));
        }
        else if(CommentComponent* const cc = ObjectsHelper::getComment(selectedItem))
        {
            const int startX = cc->getProperties() ["xDragStart"];
            const int startY = cc->getProperties() ["yDragStart"];

            cc->setActualPosition(Point<int>(holder.snapPosition(startX + dx),
                                             holder.snapPosition(startY + dy)));
        }
    }
}

void ObjController::endDragging()
{
    if (isReflowing)
    {
        return;
    }

    ObjectsHolder& holder = *owner.getHolderComponent();

    std::for_each(sObjects.begin(), sObjects.end(), [&holder](SelectableObject* obj)
    {
        if(ObjectComponent * const c = ObjectsHelper::getObject(obj))
        {
            c->setPosition(c->getActualPos(), true);
            holder.checkExtent(c->getBounds());
        }
        else if(CommentComponent* const cc = ObjectsHelper::getComment(obj))
        {
            cc->setPosition(cc->getActualPos(), true);
            holder.checkExtent(c->getBounds());
        }
    });

    changed();

    owner.getUndoManager().beginNewTransaction();
}

void ObjController::moveSelectedComps (int dxFromMoveStart, int dyFromMoveStart)
{
    startDragging();
    dragSelectedComps(dxFromMoveStart, dyFromMoveStart);
    endDragging();
}

UndoManager& ObjController::getUndoManager()
{
    return owner.getUndoManager();
}

void ObjController::changed()
{
    owner.changed();
}

struct ObjectIdComparator
{
    ObjectIdComparator(const String& idStr) : idStr_(idStr){}

    String idStr_;
    bool operator()(const BaseObjectComponent* const boc)
    {
        return idStr_.compare(boc->getData().getProperty(Ids::identifier).toString()) == 0;
    }
};

ObjectComponent* ObjController::getObjectForId(const String& idString) const
{
    auto it = std::find_if(objects.begin(), objects.end(), ObjectIdComparator(idString));
    return it != objects.end() ? *it : nullptr;
}

LinkComponent* ObjController::getLinkForId(const String& idString) const
{
    auto it = std::find_if(links.begin(), links.end(), ObjectIdComparator(idString));
    return it != links.end() ? *it : nullptr;
}

CommentComponent* ObjController::getCommentForId(const String& idString) const
{
    auto it = std::find_if(comments.begin(), comments.end(), ObjectIdComparator(idString));
    return it != comments.end() ? *it : nullptr;
}

void ObjController::reverseLinkDirection()
{
    owner.getUndoManager().beginNewTransaction();

    for (SelectableObject* const selectedItem : sObjects.getItemArray())
    {
        if(LinkComponent* lc = ObjectsHelper::getLink(selectedItem))
        {
            ReverseLinkDirectionAction* action = new ReverseLinkDirectionAction(lc,this);
            owner.getUndoManager().perform(action, "reverse link direction");
        }
    }
    changed();

    owner.getUndoManager().beginNewTransaction();
}

Array<int> ObjController::checkIfObjectHasLinks(ValueTree objTree)
{
    Array<int> linkIndices;
    for (int i = 0; i < links.size(); i++)
    {
        const ValueTree linkTree = links.getUnchecked(i)->getData();

        if(linkTree.getProperty(Ids::startVertex) == objTree.getProperty(Ids::identifier) ||
           linkTree.getProperty(Ids::endVertex) == objTree.getProperty(Ids::identifier))
        {
            linkIndices.add(i);
        }
    }

    return linkIndices;
}

Array<int> ObjController::checkIfObjectHasAudioConnections(ValueTree objTree)
{
    Array<int> aocIndices;
    for (int i = 0; i < audioConnections.size(); ++i)
    {
        const AudioOutConnector* const aoc = audioConnections.getUnchecked(i);

        const ValueTree aocSource = aoc->getSourceObject()->getData();
        const ValueTree aocAudioObject = aoc->getAudioObject()->getData();

        if(aocSource[Ids::identifier] == objTree[Ids::identifier] ||
           aocAudioObject[Ids::identifier] == objTree[Ids::identifier])
        {
            aocIndices.add(i);
        }
    }
    return aocIndices;
}

//==============================================================================

bool ObjController::checkIfIdExists(const Identifier& objId, const String& idStr)
{
    return idMgr->contains(objId, idStr);
}

bool ObjController::renameId(const Identifier& objId, const String& oldId,
                             const String& newId, UndoManager* undoManager_)
{
    return idMgr->renameId(objId, oldId, newId, undoManager_);
}

String ObjController::getNewNameForObject(const Identifier& objId)
{
    return idMgr->getNextId(objId);
}

void ObjController::setLinksSegmented(bool isSegmented)
{
    std::for_each(links.begin(), links.end(), [=](LinkComponent* const lc)
    {
        lc->setSegmented(isSegmented);
    });
    std::for_each(audioConnections.begin(), audioConnections.end(),
                  [=](AudioOutConnector* const aoc)
    {
        aoc->setSegmented(isSegmented);
    });
}

void ObjController::destroy()
{
    links.clear();
    audioConnections.clear();
    objects.clear();
    sObjects.deselectAll();
    idMgr = nullptr;
    idMgr = new IdManager();

}

void ObjController::setAudioConnectionVisibility(bool shouldBeVisible)
{
    for (AudioOutConnector* const aoc : audioConnections)
    {
        aoc->setVisible(shouldBeVisible);
    }
}

template<typename T>
void ObjController::setAsFromtmostLink(T& t)
{
    auto fnToBehind = [&t](Component* const c)
    {
        c->toBehind(&t);
    };

    std::for_each(links.begin(), links.end(), fnToBehind);
    std::for_each(audioConnections.begin(), audioConnections.end(), fnToBehind);
}

template void ObjController::setAsFromtmostLink<LinkComponent>(LinkComponent& t);
template void ObjController::setAsFromtmostLink<AudioOutConnector>(AudioOutConnector& t);

void ObjController::startReflow(ObjectsHolder& objHolder, const int cmdId)
{
    reflowController->startReflow(objHolder, cmdId);
}

void ObjController::stopReflow()
{
    reflowController->stopReflow();
}
