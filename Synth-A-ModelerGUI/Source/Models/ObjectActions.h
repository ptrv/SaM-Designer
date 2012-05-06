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

class AddObjectAction : public UndoableAction
{
public:
	AddObjectAction(ValueTree mdlTree_) : mdlTree(mdlTree_)
	{
		oldValue = mdlTree.getOrCreateChildWithName(Objects::mdlMasses, nullptr);
		newValue = ValueTree(Ids::mass);
		newValue.setProperty(Ids::posX, 10, nullptr);
		newValue.setProperty(Ids::posY, 10, nullptr);
		ValueTree paramsTree(Ids::parameters);
		paramsTree.setProperty(Ids::idx[0], 1.0f, nullptr);
		newValue.addChild(paramsTree, -1, nullptr);
		newValue.setProperty(Ids::identifier, "blabla", nullptr);
		ValueTree labelsTree(Ids::labels);
		labelsTree.setProperty(Ids::idx[0], "", nullptr);
		newValue.addChild(labelsTree, -1, nullptr);
	}
	~AddObjectAction()
	{
	}

	bool perform()
	{
		DBG("Add mass");
		ValueTree masses = mdlTree.getChildWithName(Objects::mdlMasses);
		masses.addChild(newValue,-1, nullptr);
		return true;
	}

	bool undo()
	{
		DBG("Undo add mass");
		mdlTree.getChildWithName(Objects::mdlMasses).removeChild(newValue, nullptr);
		return true;
	}
private:
	ValueTree mdlTree;
	ValueTree oldValue, newValue;
};


#endif  // __OBJECTACTIONS_H_7C20FDA1__
