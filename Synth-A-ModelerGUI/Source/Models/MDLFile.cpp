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
#include "../Utilities/StoredSettings.h"
#include "ObjectIDs.h"


MDLFile::MDLFile()
: FileBasedDocument(".mdl", "*.mdl", "Open mdl...", "Save mdl..."),
  isModified(false),
  mdlRoot(Objects::mdlRoot)
{
	initMDL();
}

MDLFile::~MDLFile()
{
	destroyMDL();
}

void MDLFile::initMDL()
{
	mdlRoot = ValueTree(Objects::mdlRoot);
	isInit = true;
	setChangedFlag(false);
	mdlPath = String::empty;
	mdlName = "Untitled";
}

void MDLFile::destroyMDL()
{
	mdlRoot = ValueTree();
}

void MDLFile::newMDL()
{
	destroyMDL();
	initMDL();
}


void MDLFile::close()
{
	newMDL();
}

const String MDLFile::getDocumentTitle()
{
	return mdlName;
}
const String MDLFile::loadDocument (const File& file)
{
	destroyMDL();
	initMDL();
	mdlPath = file.getFullPathName();
	MDLParser pa(*this);
	if(pa.parseMDL())
	{
		// success
		DBG("Opened MDL file: "+String(mdlPath));
		isInit = false;
		File mf(mdlPath);
		mdlName = mf.getFileName();
		return String::empty;
	}
	else
	{
		// fail
		return "ERROR: could not load mdl file.";
	}
}
const String MDLFile::saveDocument (const File& file)
{
	this->setChangedFlag(false);
	MDLWriter wr(*this);
	if(wr.writeMDL(file))
	{
		DBG("Saved MDL file: "+file.getFullPathName());
		return String::empty;
	}
	else
	{
		return "ERROR: could not save mdl file.";
	}
}

File MDLFile::lastDocumentOpened;
const File MDLFile::getLastDocumentOpened()
{
//	return File(StoredSettings::getInstance()->getLastDocument());
	return lastDocumentOpened;
}
void MDLFile::setLastDocumentOpened (const File& file)
{
	lastDocumentOpened = file;
	StoredSettings::getInstance()->setLastDocument(file.getFullPathName());
}


//==============================================================================
#if UNIT_TESTS

#include "../../Testsuite/MDLFile_test.h"

#endif
