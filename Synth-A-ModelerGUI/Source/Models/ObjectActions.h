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
#include "../View/LinkComponent.h"
#include "../View/ObjectsHolder.h"
#include "../Controller/ObjController.h"

class AddObjectAction : public UndoableAction
{
public:
	AddObjectAction(ObjController* objController_, 
                 ValueTree objTree_, ObjectsHolder* holder_)
	: objTree(objTree_.createCopy()), 
        holderComp(holder_), 
        objController(objController_)
	{
	}
	~AddObjectAction()
	{
	}

	bool perform()
	{
        ObjectComponent* objComp = objController->addObject(holderComp, objTree, -1,false);
        indexAdded = objController->indexOfObject(objComp);
        String logText = "Add ";
        logText << objTree.getType().toString();// << " number " << mdlSubTree.getNumChildren();
		SAM_LOG(logText);
		return true;
	}

	bool undo()
	{
        objController->removeObject(objController->getObject(indexAdded), false, holderComp);

        String logText = "Undo add ";
        logText <<  objTree.getType().toString();// << " number " << mdlSubTree.getNumChildren();
		SAM_LOG(logText);
		return true;
	}
    int indexAdded;
private:
    ValueTree objTree;
	ObjectsHolder* holderComp;
    ObjController* objController;
    
    
};

class RemoveObjectAction : public UndoableAction
{
public:
    RemoveObjectAction(ObjectsHolder* objHolderComp_,
                       ObjectComponent* componentToRemove,
                       ObjController* objController_)
    : 
    holderComp(objHolderComp_),
    objController(objController_),
    oldIndex(-1)
	{
        oldValue = componentToRemove->getData().createCopy();
        oldIndex = objController->indexOfObject(componentToRemove);
	}
	~RemoveObjectAction()
	{
	}

	bool perform()
	{
        SAM_LOG("Remove "+oldValue[Ids::identifier].toString());
        ObjectComponent* oc = objController->getObject(oldIndex);
        if(objController->getSelectedObjects().getNumSelected() == 0)
        {
            objController->getSelectedObjects().selectOnly(oc);
        }
        objController->removeObject(oc, false, holderComp);
		return true;
	}

	bool undo()
	{
        SAM_LOG("Undo remove "+oldValue[Ids::identifier].toString());

        ObjectComponent* oc = objController->addObject(holderComp, oldValue, oldIndex, false);
        if(objController->getSelectedObjects().getNumSelected() == 0)
        {
            objController->getSelectedObjects().selectOnly(oc);
        }

		return true;
	}
private:
	ObjectsHolder* holderComp;
	ValueTree oldValue;
    ObjController* objController;
    int oldIndex;
    

};

class MoveObjectAction : public UndoableAction {
public:
	MoveObjectAction(ObjectsHolder* objHolderComp_,
			ObjectComponent* componentToMove,
			Point<int> newPos_)
	: holderComp(objHolderComp_),
	  objComp(componentToMove),
	  newPos(newPos_),
        oldPos(componentToMove->getActualPos())
	{

	}

	~MoveObjectAction()
	{

	}

	bool perform()
	{
        objComp->setPosition(newPos, false);
		return true;
	}

	bool undo()
	{
        objComp->setPosition(oldPos, false);
		return true;
	}
private:
	ObjectsHolder* holderComp;
	ObjectComponent* objComp;
	Point<int> newPos;
    Point<int> oldPos;
};

class AddLinkAction : public UndoableAction
{
public:
	AddLinkAction(ObjController* objController_, 
                 ValueTree linkTree_, ObjectsHolder* holder_)
	: linkTree(linkTree_.createCopy()), 
        holderComp(holder_), 
        objController(objController_)
	{
	}
	~AddLinkAction()
	{
	}

	bool perform()
	{
        LinkComponent* linkComp = objController->addLink(holderComp, linkTree, -1, false);
        indexAdded = objController->indexOfLink(linkComp);

        String logText = "Add ";
        logText << linkTree.getType().toString();// << " number " << mdlSubTree.getNumChildren();
		SAM_LOG(logText);
		return true;
	}

	bool undo()
	{
        
        objController->removeLink(objController->getLink(indexAdded), false, holderComp);
        String logText = "Undo add ";
        logText <<  linkTree.getType().toString();// << " number " << mdlSubTree.getNumChildren();
		SAM_LOG(logText);
		return true;
	}
    int indexAdded;
private:
    ValueTree linkTree;
	ObjectsHolder* holderComp;
    ObjController* objController;
    
    
};

class RemoveLinkAction : public UndoableAction
{
public:
    RemoveLinkAction(ObjectsHolder* objHolderComp_,
                       LinkComponent* linkToRemove,
                       ObjController* objController_)
    : 
    holderComp(objHolderComp_),
    objController(objController_),
    oldIndex(-1)
	{
        oldValue = linkToRemove->getData().createCopy();
        oldIndex = objController->indexOfLink(linkToRemove);
	}
	~RemoveLinkAction()
	{
	}

	bool perform()
	{
        SAM_LOG("Remove "+oldValue[Ids::identifier].toString());
        LinkComponent* lc = objController->getLink(oldIndex);
        if(objController->getSelectedLinks().getNumSelected() == 0)
        {
            objController->getSelectedLinks().selectOnly(lc);
        }
        objController->removeLink(lc, false, holderComp);
		return true;
	}

	bool undo()
	{
        SAM_LOG("Undo remove "+oldValue[Ids::identifier].toString());

        LinkComponent* lc = objController->addLink(holderComp, oldValue, oldIndex, false);
        if(objController->getSelectedLinks().getNumSelected() == 0)
        {
            objController->getSelectedLinks().selectOnly(lc);
        }
        
		return true;
	}
private:
	ObjectsHolder* holderComp;
	ValueTree oldValue;
    ObjController* objController;
    int oldIndex;
};

class ReverseLinkDirectionAction : public UndoableAction
{
public:
    ReverseLinkDirectionAction(LinkComponent* linkToReverse,
                               ObjController* objController_)
    : 
    objController(objController_)
	{
        linkIndex = objController->indexOfLink(linkToReverse);
	}
	~ReverseLinkDirectionAction()
	{
	}

	bool perform()
	{
//        SAM_LOG("Reverse direction"+oldValue[Ids::identifier].toString());
        LinkComponent* lc = objController->getLink(linkIndex);
        if(objController->getSelectedLinks().getNumSelected() == 0)
        {
            objController->getSelectedLinks().selectOnly(lc);
        }
        lc->reverseDirection();
		return true;
	}

	bool undo()
	{
//        SAM_LOG("Undo remove "+oldValue[Ids::identifier].toString());

        LinkComponent* lc = objController->getLink(linkIndex);
        if(objController->getSelectedLinks().getNumSelected() == 0)
        {
            objController->getSelectedLinks().selectOnly(lc);
        }
        lc->reverseDirection();
		return true;
	}
private:
	ValueTree linkTree;
    ObjController* objController;
    String oldStart;
    String oldEnd;
    int linkIndex;
};
#endif  // __OBJECTACTIONS_H_7C20FDA1__
