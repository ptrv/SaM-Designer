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
#include "../Models/ObjectActions.h"
#include "../Models/MDLFile.h"
#include "../Models/ObjectFactory.h"
#include "../View/ObjectComponent.h"
#include "../View/LinkComponent.h"
#include "../View/ObjectPropertiesPanel.h"
#include "MDLController.h"

#include "ObjController.h"

ObjController::ObjController(MDLController& owner_)
: owner(owner_)
{

}

ObjController::~ObjController()
{
    objects.clear(true);
    links.clear(true);
}

bool ObjController::perform(UndoableAction * const action, const String& actionName)
{
    return owner.perform(action, actionName);
}

ObjectComponent* ObjController::addObject(ObjectsHolder* holder, ValueTree objValues, int index, bool undoable)
{
    selectedObjects.deselectAll();
    
    if(undoable)
    {
        AddObjectAction* action = new AddObjectAction(this, objValues, holder);
        owner.getUndoManager()->perform(action, "Add new Object");
        
        return objects[action->indexAdded];
    }
    else
    {
        const Identifier& groupName = Objects::getObjectGroup(objValues.getType().toString());
        ValueTree mdl = owner.getMDLTree();
        ValueTree subTree = mdl.getOrCreateChildWithName(groupName, nullptr);

		subTree.addChild(objValues,-1, nullptr);
        ObjectComponent* objComp = ObjectFactory::createNewObjectComponentFromTree(*this, objValues, index);

		holder->addAndMakeVisible(objComp);
        holder->updateComponents();
        changed();
        return objComp;
    }
    return 0;
}

void ObjController::addNewObject(ObjectsHolder* holder, ValueTree objValues)
{
    
    addObject(holder, objValues, -1, true);
}

LinkComponent* ObjController::addLink(ObjectsHolder* holder, ValueTree linkValues, int index, bool undoable)
{
    selectedLinks.deselectAll();
    if(undoable)
    {
        AddLinkAction* action = new AddLinkAction(this, linkValues, holder);
        owner.getUndoManager()->perform(action, "Add new Link");
        
        return links[action->indexAdded];
    }
    else
    {
        const Identifier& gruopName = Objects::getObjectGroup(linkValues.getType());
        ValueTree mdl = owner.getMDLTree();
        ValueTree subTree = mdl.getOrCreateChildWithName(gruopName, nullptr);
		subTree.addChild(linkValues,-1, nullptr);
        LinkComponent* linkComp = ObjectFactory::createNewLinkComponentFromTree(*this, linkValues, index);

		holder->addAndMakeVisible(linkComp);
        holder->updateComponents();
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
    for (int i = 0; i < links.size(); i++)
    {
        if(links[i]->sameStartEnd(linkTree))
        {
            return true;
        }
    }
    return false;

}
void ObjController::addNewLinkIfPossible(ObjectsHolder* holder, ValueTree linkValues)
{
    if(selectedObjects.getNumSelected() == 2)
    {
        if(! checkIfLinkExitsts(linkValues))
        {
            addLink(holder, linkValues, -1, true);
            holder->updateComponents();
        }
    }
    else
    {
        SAM_CONSOLE("Error: ", "Please select 2 Objects");
    }
}

void ObjController::addComponent(ObjectComponent* comp, int index)
{
    objects.insert(index, comp);
}

void ObjController::addLinkComponent(LinkComponent* comp, int index)
{
    links.insert(index, comp);
}
void ObjController::removeObject(ObjectComponent* objComp, bool undoable, ObjectsHolder* holder)
{
    if (undoable)
    {
        owner.getUndoManager()->perform(new RemoveObjectAction(holder, objComp, this));
    }
    else
    {
        selectedObjects.deselect(objComp);
        selectedObjects.changed(true);
        Array<int> indices = checkIfObjectHasLinks(objComp);
        if(indices.size() > 0)
            selectedLinks.deselectAll();
        for(int i = indices.size(); --i >= 0;)
        {
//            DBG("Object has link nr " + String(lIdx));
            removeLink(getLink(i), true, holder);
//            links.remove(lIdx);
        }
        const Identifier& groupName = Objects::getObjectGroup(objComp->getData().getType());
        ValueTree mdl = owner.getMDLTree();
        ValueTree subTree = mdl.getOrCreateChildWithName(groupName, nullptr);
        subTree.removeChild(objComp->getData(), nullptr);
        objects.removeObject(objComp);
    }
    
}

void ObjController::removeSelectedObjects(ObjectsHolder* holder)
{
//    owner.getUndoManager()->beginNewTransaction();

    const SelectedItemSet <ObjectComponent*> temp(selectedObjects);

    if (temp.getNumSelected() > 0)
    {
        selectedObjects.deselectAll();
        selectedObjects.changed(true);

        for (int i = temp.getNumSelected(); --i >= 0;)
        {
            removeObject(temp.getSelectedItem(i), true, holder);
        }
    }
//    owner.getUndoManager()->beginNewTransaction();

}

void ObjController::removeSelectedLinks(ObjectsHolder* holder)
{
    const SelectedItemSet <LinkComponent*> temp(selectedLinks);

    if (temp.getNumSelected() > 0)
    {
        selectedLinks.deselectAll();
        selectedLinks.changed(true);

        for (int i = temp.getNumSelected(); --i >= 0;)
        {
            removeLink(temp.getSelectedItem(i), true, holder);
        }
    }
}

void ObjController::removeLink(LinkComponent* linkComp, bool undoable, ObjectsHolder* holder)
{
    if (undoable)
    {
        owner.getUndoManager()->perform(new RemoveLinkAction(holder, linkComp, this));
    }
    else
    {
        selectedLinks.deselect(linkComp);
        selectedLinks.changed(true);
        const Identifier& groupName = Objects::getObjectGroup(linkComp->getData().getType());
        ValueTree mdl = owner.getMDLTree();
        ValueTree subTree = mdl.getOrCreateChildWithName(groupName, nullptr);
        subTree.removeChild(linkComp->getData(), nullptr);
        links.removeObject(linkComp);
    }
}

void ObjController::loadComponents(ObjectsHolder* holder)
{
    MDLFile* mf = owner.getMDLFile();
    ValueTree mdl = mf->mdlRoot;
    
    ValueTree massObjects = mdl.getChildWithName(Objects::masses);
    for (int i = 0; i < massObjects.getNumChildren(); i++)
    {
        ValueTree obj = massObjects.getChild(i);
        ObjectComponent* objComp = new ObjectComponent(*this, obj);
        objects.add(objComp);
        holder->addAndMakeVisible(objComp);
        objComp->update();
        SAM_LOG("Load " + obj.getType().toString() + " " + obj[Ids::identifier].toString());
    }
    ValueTree audioObjects = mdl.getChildWithName(Objects::audioobjects);
    for (int i = 0; i < audioObjects.getNumChildren(); i++)
    {
        ValueTree obj = audioObjects.getChild(i);
        ObjectComponent* objComp = new ObjectComponent(*this, obj);
        objects.add(objComp);
        holder->addAndMakeVisible(objComp);
        objComp->update();
        SAM_LOG("Load " + obj.getType().toString() + " " + obj[Ids::identifier].toString());
    }

    ValueTree linkObjects = mdl.getChildWithName(Objects::links);
    for (int i = 0; i < linkObjects.getNumChildren(); i++)
    {
        ValueTree obj = linkObjects.getChild(i);
        LinkComponent* linkComp = new LinkComponent(*this, obj);
        links.add(linkComp);
        holder->addAndMakeVisible(linkComp);
        linkComp->update();
        SAM_LOG("Load " + obj.getType().toString() + " " + obj[Ids::identifier].toString());
    }
    holder->updateComponents();
}

void ObjController::selectAll(bool shouldBeSelected)
{
    if(shouldBeSelected)
    {
        selectedObjects.deselectAll();
        for (int i = 0; i < objects.size(); ++i)
        {
            selectedObjects.addToSelection(objects[i]);
        }
        selectedLinks.deselectAll();
        for (int j = 0; j < links.size(); ++j)
        {
            selectedLinks.addToSelection(links[j]);
        }
    }
    else
    {
        selectedObjects.deselectAll();
        selectedLinks.deselectAll();
    }
}

void ObjController::editObjectProperties(BaseObjectComponent* oc, UndoManager* undoManager)
{
    ObjectPropertiesPanel::show(oc, undoManager);
}

void ObjController::startDragging(const Rectangle<int>& parentArea)
{
    for (int i = 0; i < objects.size(); ++i)
    {
        ObjectComponent * const c = objects[i];

        Point<int> r(c->getPosition());

        c->getProperties().set("xDragStart", r.getX());
        c->getProperties().set("yDragStart", r.getY());
    }

    owner.getUndoManager()->beginNewTransaction();
}

void ObjController::dragSelectedComps(int dx, int dy, const Rectangle<int>& parentArea)
{
    owner.getUndoManager()->undoCurrentTransactionOnly();

    for (int i = 0; i < selectedObjects.getNumSelected(); ++i)
    {
        ObjectComponent * const c = selectedObjects.getSelectedItem(i);

        const int startX = c->getProperties() ["xDragStart"];
        const int startY = c->getProperties() ["yDragStart"];

        Point<int> r(c->getPosition());

        r.setXY(startX + dx, startY + dy);

        c->setPosition(Point<int>(r.x + c->getWidth() / 2, r.y + c->getHeight() / 2), true);
    }

    changed();
    owner.getMDLFile()->changed();
    //    changed();
}

void ObjController::endDragging()
{
    owner.getUndoManager()->beginNewTransaction();
}

UndoManager* ObjController::getUndoManager()
{
    return owner.getUndoManager();
}

void ObjController::changed()
{
    owner.changed();
}

ObjectComponent* ObjController::getObjectForId(String idString) const throw()
{
    for (int i = 0; i < objects.size(); i++)
    {
        ObjectComponent* elem = objects[i];
        if(idString.compare(elem->getData().getProperty(Ids::identifier).toString()) == 0)
        {
            return elem;
        }
    }
    return nullptr;

}

void ObjController::reverseLinkDirection()
{
    owner.getUndoManager()->beginNewTransaction();

    for (int i = 0; i < selectedLinks.getNumSelected(); ++i)
    {
        ReverseLinkDirectionAction* action = new ReverseLinkDirectionAction(selectedLinks.getItemArray()[i],this);
        owner.getUndoManager()->perform(action, "reverse link direction");
    }
    changed();
    
    owner.getUndoManager()->beginNewTransaction();
}

Array<int> ObjController::checkIfObjectHasLinks(ObjectComponent* objComp)
{
    Array<int> linkIndices;
    ValueTree objTree = objComp->getData();
    for (int i = 0; i < links.size(); i++)
    {
        ValueTree linkTree = links[i]->getData();
        
        if(linkTree.getProperty(Ids::startVertex) == objTree.getProperty(Ids::identifier)
            || linkTree.getProperty(Ids::endVertex) == objTree.getProperty(Ids::identifier))
        {
            linkIndices.add(i);
        }
    }
    return linkIndices;
}