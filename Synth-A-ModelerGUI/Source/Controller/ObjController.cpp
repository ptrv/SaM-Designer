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

#include "ObjController.h"
#include "AppController.h"
#include "../Models/ObjectIDs.h"
#include "../Models/ObjectActions.h"

ObjController::ObjController(AppController& owner_)
: owner(owner_)
{

}

ObjController::~ObjController()
{

}

bool ObjController::perform (UndoableAction* const action, const String& actionName)
{
	return owner.perform(action, actionName);
}
static const Identifier& getObjectType(const Identifier& ident)
{
	if(ident == Ids::mass || ident == Ids::port
			|| ident == Ids::ground	|| ident == Ids::resonator)
		return Objects::mdlMasses;
	else if(ident == Ids::link || ident == Ids::touch
			|| ident == Ids::pluck)
		return Objects::mdlLinks;
	else if(ident == Ids::audioout)
		return Objects::mdlAudioObjects;
	else if(ident == Ids::waveguide)
		return Objects::mdlWaveguides;
	else if(ident == Ids::termination)
		return Objects::mdlTerminations;
	else if(ident == Ids::junction)
		return Objects::mdlJunctions;
	else if(ident == Ids::label)
		return Objects::mdlLabels;

	else
		return Objects::mdlInvalid;
}
void ObjController::addObject(const Identifier& objId, int posX, int posY)
{
	const Identifier& tmpIdent = getObjectType(objId);
	if(tmpIdent != Objects::mdlInvalid)
	{
		ValueTree subTree = owner.getMDLTree().getOrCreateChildWithName(tmpIdent, nullptr);
		this->perform(new AddObjectAction(subTree, objId, posX, posY), "Add new Object");
	}
}
