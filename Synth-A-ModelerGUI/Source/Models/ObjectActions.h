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

class AddObjectAction : public UndoableAction
{
public:
	AddObjectAction(ValueTree mdlTree_, const Identifier& objId_, int posX, int posY)
	: mdlSubTree(mdlTree_),
	  objId(objId_)
	{
		newValue = ObjectFactory::createNewObjectTree(objId, posX, posY);
	}
	~AddObjectAction()
	{
	}

	bool perform()
	{
		DBG("Add "+objId.toString());
		DBG(mdlSubTree.getNumChildren());
		mdlSubTree.addChild(newValue,-1, nullptr);
		DBG(mdlSubTree.getNumChildren());
		return true;
	}

	bool undo()
	{
		DBG("Undo add "+objId.toString());
		DBG(mdlSubTree.getNumChildren());
		mdlSubTree.removeChild(newValue, nullptr);
		DBG(mdlSubTree.getNumChildren());
		return true;
	}
private:
	ValueTree mdlSubTree;
	ValueTree newValue;
	const Identifier& objId;
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
		DBG("Remove "+oldValue[Ids::identifier].toString());
		mdlSubTree.removeChild(oldValue, nullptr);
		return true;
	}

	bool undo()
	{
		DBG("Undo remove "+oldValue[Ids::identifier].toString());
		mdlSubTree.addChild(oldValue,-1, nullptr);
		return true;
	}
private:
	ValueTree mdlSubTree;
	ValueTree oldValue;
};
#endif  // __OBJECTACTIONS_H_7C20FDA1__
