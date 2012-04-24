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
#include "../Utilities/MDLWriter.h"

MDLFile::MDLFile()
: isModified(false)
{
	initMDL();
}

MDLFile::~MDLFile()
{
	destroyMDL();
}

void MDLFile::initMDL()
{
	isInit = true;
	isModified = false;
	mdlPath = String::empty;
	mdlName = "Untitled";
}

void MDLFile::destroyMDL()
{
	allObjects.clear();
	for (int i = 0; i < masses.size(); ++i) {
		delete masses[i];
	}
	masses.clear();
	for (int i = 0; i < links.size(); ++i) {
		delete links[i];
	}
	links.clear();
	for (int i = 0; i < labelObjs.size(); ++i) {
		delete labelObjs[i];
	}
	labelObjs.clear();
	for (int i = 0; i < audioObjects.size(); ++i) {
		delete audioObjects[i];
	}
	audioObjects.clear();
	for (int i = 0; i < waveguides.size(); ++i) {
		delete waveguides[i];
	}
	waveguides.clear();
	for (int i = 0; i < terminations.size(); ++i) {
		delete terminations[i];
	}
	terminations.clear();
	for (int i = 0; i < junctions.size(); ++i) {
		delete junctions[i];
	}
	junctions.clear();

}

void MDLFile::newMDL()
{
	destroyMDL();
	initMDL();
}

bool MDLFile::openMDL(const char* mdlPath_)
{
	destroyMDL();
	initMDL();
	mdlPath = mdlPath_;
	MDLParser pa(*this);
	if(pa.parseMDL())
	{
		// success
		DBG("Opened MDL file: "+String(mdlPath));
		isInit = false;
		File mf(mdlPath);
		mdlName = mf.getFileName();
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
	labelObjs.add(obj);
	allObjects.set(obj->getName(), obj);
}

void MDLFile::addAudioObject(AudioObject* obj)
{
	audioObjects.add(obj);
	allObjects.set(obj->getName(), obj);
}

void MDLFile::addWaveguideObject(WaveguideObject* obj)
{
	waveguides.add(obj);
	allObjects.set(obj->getName(), obj);
}

void MDLFile::addTerminationObject(TerminationObject* obj)
{
	terminations.add(obj);
	allObjects.set(obj->getName(), obj);
}

void MDLFile::addJunctionObject(JunctionObject* obj)
{
	junctions.add(obj);
	allObjects.set(obj->getName(), obj);
}

bool MDLFile::needsSaving()
{
	return isModified;
}

bool MDLFile::save(const String& savePath)
{
	isModified = false;
	MDLWriter wr(*this);
	if(wr.writeMDL(savePath))
	{
		DBG("Saved MDL file: "+savePath);
		return true;
	}
	else
	{
		return false;
	}
}

void MDLFile::close()
{
	newMDL();
}
//==============================================================================
#if UNIT_TESTS

#include "../../Testsuite/MDLFile_test.h"

#endif
