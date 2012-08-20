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
}

bool ObjController::perform(UndoableAction * const action, const String& actionName)
{
    return owner.perform(action, actionName);
}

ObjectComponent* ObjController::addObject(ObjectsHolder* holder, ValueTree objValues, bool undoable)
{
//    const Identifier& tmpIdent = Objects::getObjectType(objId);
//    if (tmpIdent != Objects::invalid)
//    {
//        ValueTree mdl = owner.getMDLTree();
//        ValueTree subTree = mdl.getOrCreateChildWithName(tmpIdent, nullptr);
//
//    }
//
    selectedObjects.deselectAll();
    
//    if(objComp == nullptr)
//    {
//        objComp = new ObjectComponent(holder->getObjController(),
//                             objValues.getType(), int(objValues[Ids::posX]), int(objValues[Ids::posY]));
//
//    }
    
    if(undoable)
    {
        AddObjectAction* action = new AddObjectAction(this, objValues, holder);
        owner.getUndoManager()->perform(action, "Add new Object");
        
        return objects[action->indexAdded];
    }
    else
    {
        const Identifier& tmpIdent = Objects::getObjectType(objValues.getType().toString());
        ValueTree mdl = owner.getMDLTree();
        ValueTree subTree = mdl.getOrCreateChildWithName(tmpIdent, nullptr);

////        ValueTree newValue = ObjectFactory::createNewObjectTree(objId, posX, posY);
//        ObjectComponent* objComp = new ObjectComponent(holder->getObjController(),
//                                                       objValues.getType(),
//                                                       int(objValues[Ids::posX]),
//                                                       int(objValues[Ids::posY]));

		subTree.addChild(objValues,-1, nullptr);
        ObjectComponent* objComp = ObjectFactory::createNewObjectComponentFromTree(*this, objValues);

//		objComp->setData(objValues);
//		objects.add(objComp);
		holder->addAndMakeVisible(objComp);
        holder->updateComponents();
        changed();
        return objComp;
    }
    return 0;
}

void ObjController::addNewObject(ObjectsHolder* holder, ValueTree objValues)
{
    
    addObject(holder, objValues, true);
}

void ObjController::addComponent(ObjectComponent* comp)
{
    objects.add(comp);
}
void ObjController::removeObject(ObjectComponent* objComp, bool undoable, ObjectsHolder* holder)
{
    if (undoable)
    {
        owner.getUndoManager()->perform(new RemoveObjectAction(objects, holder, objComp, this));

    }
    else
    {
        DBG(selectedObjects.getNumSelected());
        selectedObjects.deselect(objComp);
        selectedObjects.changed(true);
        
        ValueTree root = objComp->getData().getParent();
        ValueTree oldValue = objComp->getData();
//        holder->removeChildComponent(objComp);
        objects.removeObject(objComp);
//        root.removeChild(oldValue, nullptr);
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

void ObjController::loadComponents(Component* holder)
{
    MDLFile* mf = owner.getMDLFile();
    ValueTree mdl = mf->mdlRoot;
    for (int i = 0; i < mdl.getNumChildren(); ++i)
    {
        ValueTree objectGroup = mdl.getChild(i);
        for (int j = 0; j < objectGroup.getNumChildren(); ++j)
        {
            ValueTree obj = objectGroup.getChild(j);

            // TODO: Remove conditional when icons zip is complete
            if (obj.getType() == Ids::mass || obj.getType() == Ids::port
                || obj.getType() == Ids::ground || obj.getType() == Ids::resonator
                || obj.getType() == Ids::link || obj.getType() == Ids::touch
                || obj.getType() == Ids::pluck || obj.getType() == Ids::audioout)
            {
                ObjectComponent* objComp = new ObjectComponent(*this, obj.getType(),
                                                               int(obj.getProperty(Ids::posX)), int(obj.getProperty(Ids::posY)));
                objComp->setData(obj);
                objects.add(objComp);
                holder->addAndMakeVisible(objComp);
                SAM_LOG("Load " + obj.getType().toString() + " " + obj[Ids::identifier].toString());
            }
        }
    }
}

Array<ObjectComponent*> ObjController::selectObjectsWithinRectagle(Rectangle<int> rect)
{
    Array<ObjectComponent*> selectedObjects;
    if (rect.getWidth() < 0)
    {
        rect.setX(rect.getX() + rect.getWidth());
        rect.setWidth(-rect.getWidth());
    }
    if (rect.getHeight() < 0)
    {
        rect.setY(rect.getY() + rect.getHeight());
        rect.setHeight(-rect.getHeight());
    }

    for (int i = 0; i < objects.size(); ++i)
    {
        if (rect.intersects(objects[i]->getBounds()))
        {
            objects[i]->toggleSelected();
            objects[i]->setSelected(true);
            selectedObjects.add(objects[i]);
        }
    }
    return selectedObjects;
}

void ObjController::selectAll(bool shouldBeSelected)
{
    for (int i = 0; i < objects.size(); ++i)
    {
        objects[i]->setSelected(shouldBeSelected);

    }
}

void ObjController::editObjectProperties(ObjectComponent* oc, UndoManager* undoManager)
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

        c->setPosition(Point<int>(r.x + c->getWidth() / 2, r.y + c->getHeight() / 2));
    }

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