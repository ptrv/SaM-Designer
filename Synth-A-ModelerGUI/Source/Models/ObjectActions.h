/*
  ==============================================================================

    ObjectActions.h
    Created: 5 May 2012 5:32:12pm
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

#ifndef __OBJECTACTIONS_H_7C20FDA1__
#define __OBJECTACTIONS_H_7C20FDA1__

#include "../../JuceLibraryCode/JuceHeader.h"
#include "ObjectFactory.h"
#include "../View/ObjectComponent.h"
#include "../View/ObjectsHolder.h"
#include "../Controller/ObjController.h"

class AddObjectAction : public UndoableAction
{
public:
	AddObjectAction(OwnedArray<ObjectComponent>& objects_, ObjectsHolder* objHolderComp_,
			ValueTree mdlTree_, const Identifier& objId_, int posX, int posY, ObjController* objController_)
	: objects(objects_),
	  mdlSubTree(mdlTree_),
      objId(objId_),
      holderComp(objHolderComp_),
        objController(objController_)
	{
		// create a ValueTree with default values.
		newValue = ObjectFactory::createNewObjectTree(objId, posX, posY);
	}
	~AddObjectAction()
	{
	}

	bool perform()
	{
		// Add new Object to mdlRoot and ObjectHolder
		objComp = new ObjectComponent(holderComp->getObjController(),
                                     objId, int(newValue[Ids::posX]), 
                                     int(newValue[Ids::posY]));
		mdlSubTree.addChild(newValue,-1, nullptr);
		objComp->setData(newValue);
		objects.add(objComp);
		holderComp->addAndMakeVisible(objComp);
//		holderComp->updateSelectedObjects();
        String logText = "Add ";
        logText << objId.toString() << " number " << mdlSubTree.getNumChildren();
		SAM_LOG(logText);
		return true;
	}

	bool undo()
	{
        
        objController->removeObject(objComp, false, holderComp);

        String logText = "Undo add ";
        logText << objId.toString() << " number " << mdlSubTree.getNumChildren();
		SAM_LOG(logText);
		return true;
	}
private:
	OwnedArray<ObjectComponent>& objects;
	ValueTree mdlSubTree;
	ValueTree newValue;
	const Identifier& objId;
	ObjectsHolder* holderComp;
	ObjectComponent* objComp;
    ObjController* objController;
};

class RemoveObjectAction : public UndoableAction
{
public:
	RemoveObjectAction(OwnedArray<ObjectComponent>& objects_,
                     ObjectsHolder* objHolderComp_,
                    ObjectComponent* componentToRemove,
                     ObjController* objController_)
	: objects(objects_),
      holderComp(objHolderComp_),
	  objComp(componentToRemove),
        objController(objController_)
	{
        oldValue = objComp->getData();
        root = oldValue.getParent();
	}
	~RemoveObjectAction()
	{
	}

	bool perform()
	{
        SAM_LOG("Remove "+oldValue[Ids::identifier].toString());
        holderComp->removeChildComponent(objComp);
        objects.removeObject(objComp, true);
        root.removeChild(oldValue, nullptr);

		return true;
	}

	bool undo()
	{
        SAM_LOG("Undo remove "+oldValue[Ids::identifier].toString());
        root.addChild(oldValue,-1, nullptr);
        ObjectComponent* oc = new ObjectComponent(holderComp->getObjController(),
                                                 oldValue.getType(),
                int(oldValue[Ids::posX]), int(oldValue[Ids::posY]));
        oc->setData(oldValue);
        oc->setCentrePosition(int(oldValue[Ids::posX]), int(oldValue[Ids::posY]));
        objects.add(oc);
        holderComp->addAndMakeVisible(oc);
        objController->getSelectedElements().addToSelection(oc);
		return true;
	}
private:
    OwnedArray<ObjectComponent>& objects;
	ObjectsHolder* holderComp;
	ObjectComponent* objComp;
	ValueTree oldValue;
    ValueTree root;
    ObjController* objController;

};

class MoveObjectAction : public UndoableAction {
public:
	MoveObjectAction(ObjectsHolder* objHolderComp_,
			ObjectComponent* componentToMove,
			Point<int> newPos_)
	: holderComp(objHolderComp_),
	  objComp(componentToMove),
	  newPos(newPos_)
	{

	}

	~MoveObjectAction()
	{

	}

	bool perform()
	{
        ValueTree objData = objComp->getData();
		oldPos.x = int(objData[Ids::posX]);
		oldPos.y = int(objData[Ids::posY]);
		objData.setProperty(Ids::posX, newPos.getX(), nullptr);
		objData.setProperty(Ids::posY, newPos.getY(), nullptr);
		objComp->setActualPosition(newPos);
		return true;
	}

	bool undo()
	{
        ValueTree objData = objComp->getData();
		objData.setProperty(Ids::posX, oldPos.getX(), nullptr);
		objData.setProperty(Ids::posY, oldPos.getY(), nullptr);
		objComp->setActualPosition(oldPos);
		return true;
	}
private:
	ObjectsHolder* holderComp;
	ObjectComponent* objComp;
	Point<int> newPos;
    Point<int> oldPos;
};

#endif  // __OBJECTACTIONS_H_7C20FDA1__
