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

bool ObjController::perform (UndoableAction* const action, const String& actionName)
{
	return owner.perform(action, actionName);
}

void ObjController::addObject(ObjectsHolder* holder, const Identifier& objId,
		int posX, int posY)
{
	const Identifier& tmpIdent = Objects::getObjectType(objId);
	if(tmpIdent != Objects::invalid)
	{
		ValueTree mdl = owner.getMDLTree();
		ValueTree subTree = mdl.getOrCreateChildWithName(tmpIdent, nullptr);

		this->perform(new AddObjectAction(objects, holder,subTree, objId,
				posX, posY), "Add new Object");
	}
}


void ObjController::removeObject(ObjectsHolder* holder)
{
	Array<ValueTree> childrenDataToRemove = getSelectedChildrenData();
	Array<ObjectComponent*> childrenComponentsToRemove = getSelectedChildrenComponents();

	this->perform(new RemoveObjectAction(objects, holder, childrenComponentsToRemove,
				childrenDataToRemove), "Remove Object");
}

void ObjController::loadComponents(Component* holder)
{
	MDLFile*  mf = owner.getMDLFile();
	ValueTree mdl = mf->mdlRoot;
	for (int i = 0; i < mdl.getNumChildren(); ++i)
	{
		ValueTree objectGroup = mdl.getChild(i);
		for (int j = 0; j < objectGroup.getNumChildren(); ++j) {
			ValueTree obj = objectGroup.getChild(j);

			// TODO: Remove conditional when icons zip is complete
			if(obj.getType() == Ids::mass || obj.getType() == Ids::port
					|| obj.getType() == Ids::ground || obj.getType() == Ids::resonator
					|| obj.getType() == Ids::link || obj.getType() == Ids::touch
					|| obj.getType() == Ids::pluck || obj.getType() == Ids::audioout)
			{
				ObjectComponent* objComp = new ObjectComponent(obj.getType(),
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
	if(rect.getWidth() < 0)
	{
		rect.setX(rect.getX() + rect.getWidth());
		rect.setWidth(-rect.getWidth());
	}
	if(rect.getHeight() < 0)
	{
		rect.setY(rect.getY() + rect.getHeight());
		rect.setHeight(-rect.getHeight());
	}

	for (int i = 0; i < objects.size(); ++i)
	{
		if(rect.intersects(objects[i]->getBounds()))
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

Array<ValueTree> ObjController::getSelectedChildrenData()
{
	Array<ValueTree> childrenSelected;
//	for (int i = 0; i < objects.size(); ++i) {
//		if(objects.getUnchecked(i)->selected())
//		{
//			childrenSelected.add(objects.getUnchecked(i)->getData());
//		}
//	}
    for (int i = 0; i < selectedObjects.getNumSelected(); i++)
    {
        childrenSelected.add(selectedObjects.getSelectedItem(i)->getData());
    }

	return childrenSelected;
}
Array<ObjectComponent*> ObjController::getSelectedChildrenComponents()
{
	Array<ObjectComponent*> childrenSelected;
//	for (int i = 0; i < objects.size(); ++i) {
//		if(objects.getUnchecked(i)->selected())
//		{
//			childrenSelected.add(objects.getUnchecked(i));
//		}
//	}
    for (int i = 0; i < selectedObjects.getNumSelected(); i++)
    {
        childrenSelected.add(selectedObjects.getSelectedItem(i));
    }

	return childrenSelected;
}

void ObjController::moveObjects(ObjectsHolder* holder, Point<int> offset)
{
	Array<ValueTree> childrenDataMove = getSelectedChildrenData();
	Array<ObjectComponent*> childrenComponentsToMove = getSelectedChildrenComponents();

	this->perform(new MoveObjectAction(objects, holder, childrenComponentsToMove,
			childrenDataMove, offset), "Move objects");

}

void ObjController::editObjectProperties(ObjectComponent* oc, UndoManager* undoManager)
{
	ObjectPropertiesPanel::show(oc, undoManager);
}
