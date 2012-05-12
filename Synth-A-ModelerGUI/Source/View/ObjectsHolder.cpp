/*
  ==============================================================================

    ObjectsHolder.cpp
    Created: 11 Apr 2012 5:10:20pm
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

#include "ObjectsHolder.h"
#include "../Models/MDLFile.h"
#include "ContentComp.h"

ObjectsHolder::ObjectsHolder(ObjController& objController_)
: objController(objController_),
  mdlFile(nullptr)
{
	setSize(100,100);
}

ObjectsHolder::~ObjectsHolder()
{
	if(mdlFile != nullptr)
	{
		mdlFile->removeChangeListener(this);
	}
}

void ObjectsHolder::paint(Graphics& g)
{
	g.fillAll (Colours::white);
	g.drawText("This will be the object component", 10, 10 ,
			getWidth()-20, getHeight()-20, Justification::centred, false);

}

//void ObjComp::resized()
//{
//
//}

void ObjectsHolder::changeListenerCallback(ChangeBroadcaster*)
{
//	ContentComp* cc = findParentComponentOfClass<ContentComp>();
//	if(cc != nullptr)
//		cc->updateMainAppWindowTitle("");
}

void ObjectsHolder::setMDLFile(MDLFile* newMDLFile)
{
	if(newMDLFile != mdlFile && newMDLFile != nullptr)
	{
		mdlFile = newMDLFile;
		mdlFile->addChangeListener(this);
	}
}
