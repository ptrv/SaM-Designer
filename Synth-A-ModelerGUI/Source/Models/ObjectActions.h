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
	AddObjectAction(Component* objHolderComp_, ObjectComponent* objComp_,
			ValueTree mdlTree_, const Identifier& objId_, int posX, int posY)
	: mdlSubTree(mdlTree_),
      objId(objId_),
      holderComp(objHolderComp_),
      objComp(objComp_)
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
		mdlSubTree.addChild(newValue,-1, nullptr);
		objComp->setData(newValue);
		holderComp->addAndMakeVisible(objComp);
        String logText = "Add ";
        logText << objId.toString() << " number " << mdlSubTree.getNumChildren();
		SAM_LOG(logText);
		return true;
	}

	bool undo()
	{
		holderComp->removeChildComponent(objComp);
		mdlSubTree.removeChild(newValue, nullptr);
        String logText = "Undo add ";
        logText << objId.toString() << " number " << mdlSubTree.getNumChildren();
		SAM_LOG(logText);
		return true;
	}
private:
	ValueTree mdlSubTree;
	ValueTree newValue;
	const Identifier& objId;
	Component* holderComp;
	ObjectComponent* objComp;
};

class RemoveObjectAction : public UndoableAction
{
public:
	RemoveObjectAction(Component* objHolderComp_,
			Array<ObjectComponent*> componentsToRemove,
			Array<ValueTree> childrenToRemove)
	: holderComp(objHolderComp_),
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
			oldValue[i].getParent().removeChild(oldValue[i], nullptr);
		}
		return true;
	}

	bool undo()
	{
		for (int i = 0; i < oldValue.size(); ++i) {
			SAM_LOG("Undo remove "+oldValue[i][Ids::identifier].toString());
			holderComp->addAndMakeVisible(objComps[i]);
			oldValue[i].getParent().addChild(oldValue[i],-1, nullptr);
		}
		return true;
	}
private:
	Component* holderComp;
	Array<ObjectComponent*> objComps;
	Array<ValueTree> oldValue;

};
#endif  // __OBJECTACTIONS_H_7C20FDA1__
