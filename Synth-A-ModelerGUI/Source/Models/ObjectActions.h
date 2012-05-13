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
#include "../View/BaseObjectComponent.h"

class AddObjectAction : public UndoableAction
{
public:
	AddObjectAction(Component* objHolderComp_, BaseObjectComponent* objComp_,
			ValueTree mdlTree_, const Identifier& objId_, int posX, int posY)
	: holderComp(objHolderComp_),
	  objComp(objComp_),
	  mdlSubTree(mdlTree_),
	  objId(objId_)
	{
		newValue = ObjectFactory::createNewObjectTree(objId, posX, posY);
	}
	~AddObjectAction()
	{
	}

	bool perform()
	{

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
	BaseObjectComponent* objComp;
};

class RemoveObjectAction : public UndoableAction
{
public:
	RemoveObjectAction(ValueTree mdlTree_, ValueTree objToRemove_)
	: mdlSubTree(mdlTree_),
	  oldValue(objToRemove_)
	{
	}
	~RemoveObjectAction()
	{
	}

	bool perform()
	{
		SAM_LOG("Remove "+oldValue[Ids::identifier].toString());
		mdlSubTree.removeChild(oldValue, nullptr);
		return true;
	}

	bool undo()
	{
		SAM_LOG("Undo remove "+oldValue[Ids::identifier].toString());
		mdlSubTree.addChild(oldValue,-1, nullptr);
		return true;
	}
private:
	ValueTree mdlSubTree;
	ValueTree oldValue;
};
#endif  // __OBJECTACTIONS_H_7C20FDA1__
