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
		mdlSubTree.addChild(newValue,-1, nullptr);
		return true;
	}

	bool undo()
	{
		DBG("Undo add "+objId.toString());
		mdlSubTree.removeChild(newValue, nullptr);
		return true;
	}
private:
	ValueTree mdlSubTree;
	ValueTree newValue;
	const Identifier& objId;
};


#endif  // __OBJECTACTIONS_H_7C20FDA1__
