/*
  ==============================================================================

    MDLFile.cpp
    Created: 11 Apr 2012 3:18:35pm
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

#include "MDLFile.h"
#include "../Utilities/MDLParser.h"

MDLFile::MDLFile()
: isModified(false)
{

}

MDLFile::~MDLFile()
{
//	for (HashMap<String, BaseObject*>::Iterator i (allObjects); i.next();)
//	{
//
//	}
	allObjects.clear();
	for (int i = 0; i < masses.size(); ++i) {
		delete masses[i];
	}
	masses.clear();
	for (int i = 0; i < links.size(); ++i) {
		delete links[i];
	}
	links.clear();
	for (int i = 0; i < labels.size(); ++i) {
		delete labels[i];
	}
	labels.clear();
	for (int i = 0; i < audioObjects.size(); ++i) {
		delete audioObjects[i];
	}
	audioObjects.clear();

}

bool MDLFile::openMDL(const char* mdlPath)
{
	MDLParser pa(*this, mdlPath);
	if(pa.parseMDL())
	{
		// success
		return true;
	}
	else
	{
		// fail
		return false;
	}
}

const int MDLFile::getNumberOfObjectsByType(ObjectType objType)
{
	int count = 0;
	for (HashMap<String, BaseObject*>::Iterator i (allObjects); i.next();)
	{
		if(i.getValue()->getType() == objType)
		++count;
	}
	return count;
}

void MDLFile::addMassObject(MassObject* obj)
{
	masses.add(obj);
	allObjects.set(obj->getName(), obj);
}

void MDLFile::addLinkObject(LinkObject* obj)
{
	links.add(obj);
	allObjects.set(obj->getName(), obj);
}

void MDLFile::addLabelObject(LabelObject* obj)
{
	labels.add(obj);
	allObjects.set(obj->getName(), obj);
}

void MDLFile::addAudioObject(AudioObject* obj)
{
	audioObjects.add(obj);
	allObjects.set(obj->getName(), obj);
}

bool MDLFile::needsSaving()
{
	return isModified;
}

//==============================================================================
#if UNIT_TESTS

#include "../../Testsuite/MDLFile_test.h"

#endif
