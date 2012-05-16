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

class AddObjectAction : public UndoableAction
{
public:
	AddObjectAction(OwnedArray<ObjectComponent>& objects_, Component* objHolderComp_,
			ValueTree mdlTree_, const Identifier& objId_, int posX, int posY)
	: objects(objects_),
	  mdlSubTree(mdlTree_),
      objId(objId_),
      holderComp(objHolderComp_)
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
		objComp = new ObjectComponent(objId, int(newValue[Ids::posX]), int(newValue[Ids::posY]));
		mdlSubTree.addChild(newValue,-1, nullptr);
		objComp->setData(newValue);
		objects.add(objComp);
		holderComp->addAndMakeVisible(objComp);
        String logText = "Add ";
        logText << objId.toString() << " number " << mdlSubTree.getNumChildren();
		SAM_LOG(logText);
		return true;
	}

	bool undo()
	{
		holderComp->removeChildComponent(objComp);
		objects.removeObject(objComp);
		mdlSubTree.removeChild(newValue, nullptr);
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
	Component* holderComp;
	ObjectComponent* objComp;
};

class RemoveObjectAction : public UndoableAction
{
public:
	RemoveObjectAction(OwnedArray<ObjectComponent>& objects_,
			Component* objHolderComp_,
			Array<ObjectComponent*> componentsToRemove,
			Array<ValueTree> childrenToRemove)
	: objects(objects_),
	  holderComp(objHolderComp_),
	  objComps(componentsToRemove),
	  oldValue(childrenToRemove)
	{
	}
	~RemoveObjectAction()
	{
	}

	bool perform()
	{
		for (int i = 0; i < oldValue.size(); ++i)
		{
			SAM_LOG("Remove "+oldValue[i][Ids::identifier].toString());
			holderComp->removeChildComponent(objComps[i]);
			objects.removeObject(objComps[i]);
			oldValue[i].getParent().removeChild(oldValue[i], nullptr);
		}
		objComps.clear();
		return true;
	}

	bool undo()
	{
		for (int i = 0; i < oldValue.size(); ++i) {
			SAM_LOG("Undo remove "+oldValue[i][Ids::identifier].toString());
			oldValue[i].getParent().addChild(oldValue[i],-1, nullptr);
			ObjectComponent* oc = new ObjectComponent(oldValue[i].getType(),
					int(oldValue[i][Ids::posX]), int(oldValue[i][Ids::posY]));
			objComps.add(oc);
			oc->setData(oldValue[i]);
			objects.add(oc);
			holderComp->addAndMakeVisible(oc);
		}
		return true;
	}
private:
	OwnedArray<ObjectComponent>& objects;
	Component* holderComp;
	Array<ObjectComponent*> objComps;
	Array<ValueTree> oldValue;

};
#endif  // __OBJECTACTIONS_H_7C20FDA1__
