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

#include "../Application/CommonHeaders.h"
#include "../Utilities/MDLParser.h"
#include "../Utilities/MDLWriter.h"

#include "MDLFile.h"

using namespace synthamodeler;

const char* MDLFile::mdlFileExtension = ".mdl;.mdlx";

MDLFile::MDLFile()
: FileBasedDocument(".mdl", "*.mdl;*.mdlx", "Open mdl file", "Save mdl file"),
  mdlRoot(Objects::synthamodeler), isUntitledFile(true)
{
	initMDL();
	mdlRoot.addListener(this);

}
MDLFile::MDLFile(const File& file)
: FileBasedDocument(".mdl", "*.mdl;*.mdlx", "Open mdl file", "Save mdl file"),
  mdlRoot(Objects::synthamodeler), isUntitledFile(false)
{
	initMDL();
	mdlRoot.addListener(this);

	loadDocument(file);
}

MDLFile::~MDLFile()
{
	mdlRoot.removeListener(this);
	removeAllChangeListeners();
	destroyMDL();
}

const String MDLFile::getFilePath() const
{
    return mdlRoot.getProperty(Ids::mdlPath).toString();
}

const String MDLFile::getNameWithStatus()
{
    if(getDocumentTitle().compare("Untitled") == 0 ||
       getDocumentTitle().compare("Untitled*") == 0)
        return hasChangedSinceSaved() ? getDocumentTitle()+"*" : getDocumentTitle();
    else
        return hasChangedSinceSaved() ? getFilePath()+"*" : getFilePath();
}

bool MDLFile::perform (UndoableAction* const action, const String& actionName)
{
	return undoMgr.perform(action, actionName);
}

void MDLFile::initMDL()
{
	mdlRoot = ValueTree(Objects::synthamodeler);
	mdlRoot.setProperty(Ids::mdlName, "Untitled", nullptr);
	mdlRoot.setProperty(Ids::mdlPath, String::empty, nullptr);
	setChangedFlag(false);
}

bool MDLFile::isEmpty()
{
	if(getDocumentTitle().compare("Untitled") == 0 && mdlRoot.getNumChildren() == 0)
	{
		return true;
	}
	return false;
}

bool MDLFile::isUntiled()
{
    return isUntitledFile;
}
void MDLFile::destroyMDL()
{
	mdlRoot = ValueTree();
}

void MDLFile::newMDL()
{
	destroyMDL();
	initMDL();
    isUntitledFile = true;
}

void MDLFile::close()
{
	newMDL();
}

String MDLFile::getDocumentTitle()
{
    return mdlRoot.getProperty(Ids::mdlName).toString();
}
Result MDLFile::loadDocument (const File& file)
{
	destroyMDL();
	initMDL();
	MDLParser pa(*this);
    bool parseOk;
    String ext = file.getFileExtension().trimCharactersAtStart(".");
    if(file.hasFileExtension(".mdlx"))
    {
        parseOk = pa.parseMDLX(file);
    }
    else
    {
        parseOk = pa.parseMDL(file);
    }
    if (parseOk)
    {
        // success
        SAM_LOG("Opened " + ext.toUpperCase() + " file: " + getFilePath());
        setFile(file);
        setChangedFlag(false);
        isUntitledFile = false;
        md5 = MD5(file);

        return Result::ok();
    }
    else
    {
        // fail
        String errorMsg = "ERROR: could not load " + ext + " file.";
        SAM_LOG(errorMsg);
        return Result::fail(errorMsg);
    }
}
Result MDLFile::saveDocument (const File& file)
{
    bool saveOk;
    String errorMsg;
	this->setChangedFlag(false);
	MDLWriter wr(*this);
    String ext;
    if(file.hasFileExtension(".mdlx"))
    {
        saveOk = wr.writeMDLX(file);
    }
    else
    {
        saveOk = wr.writeMDL(file);
    }
    ext = file.getFileExtension().trimCharactersAtStart(".");
	if(saveOk)
	{
		SAM_LOG("Saved "+ ext.toUpperCase() + " file: "+file.getFullPathName());
        setFile(file);
        setChangedFlag(false);
        md5 = MD5(file);
	}
	else
	{
		errorMsg = "ERROR: could not save "+ ext + " file.";
		SAM_LOG(errorMsg);
	}
    if(StoredSettings::getInstance()->getIsUsingMDLX()
        && ! file.hasFileExtension(".mdlx"))
    {
        String savePath = file.getParentDirectory().getFullPathName() + "/"
            + file.getFileNameWithoutExtension() + ".mdlx";
        File f(savePath);
        if(wr.writeMDLX(f))
        {
            SAM_LOG("Saved MDLX file: " + f.getFullPathName());
        }
        else
        {
            errorMsg = "ERROR: could not save mdl file.";
            SAM_LOG(errorMsg);
        }
    }
    if(isUntitledFile)
    {
        loadDocument(file);
        StoredSettings::getInstance()->recentFiles.addFile(file);
        isUntitledFile = false;
    }
    if(saveOk)
    {
        return Result::ok();
    }
    else
    {
        return Result::fail(errorMsg);
    }
}

File MDLFile::lastDocumentOpened;
File MDLFile::getLastDocumentOpened()
{
	return lastDocumentOpened;
}
void MDLFile::setLastDocumentOpened (const File& file)
{
	lastDocumentOpened = file;
	StoredSettings::getInstance()->setLastDocument(file.getFullPathName());
}
void MDLFile::mdlChanged()
{
	changed();
//    DBG(mdlRoot.toXmlString());
}
//==============================================================================
void MDLFile::valueTreePropertyChanged (ValueTree& /*tree*/, const Identifier& /*property*/)
{
    mdlChanged();
}

void MDLFile::valueTreeChildAdded (ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenAdded*/)
{
	mdlChanged();
}

void MDLFile::valueTreeChildRemoved (ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenRemoved*/)
{
	mdlChanged();
}

void MDLFile::valueTreeChildOrderChanged (ValueTree& /*parentTree*/)
{
	mdlChanged();
}

void MDLFile::valueTreeParentChanged (ValueTree& /*tree*/)
{
}

//==============================================================================

ValueTree MDLFile::getObjectWithName(const String& objName)
{
	for (int i = 0; i < mdlRoot.getNumChildren(); ++i) {
		for (int j = 0; j < mdlRoot.getChild(i).getNumChildren(); ++j) {
			ValueTree ch = mdlRoot.getChild(i).getChild(j);
			if(ch[Ids::identifier].toString().compare(objName) == 0)
			{
				return ch;
			}
		}
	}
	return ValueTree::invalid;
}

String MDLFile::toString()
{
    String mdlStr = mdlRoot.toXmlString();
    return mdlStr;
}

bool MDLFile::changedOutside()
{
    if(checkIfChecksumChanged())
    {
        int res = AlertWindow::showYesNoCancelBox(AlertWindow::WarningIcon,
                                                  "MDL file changed outside!",
                                                  "",
                                                  "Load from disk",
                                                  "Overwrite",
                                                  "Cancel");
        switch (res)
        {
        case 0:
            return true;
        case 1:
            loadDocument(getFile());
            return false;
        case 2:
            return false;
        default:
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool MDLFile::checkIfChecksumChanged()
{
    MD5 tmpMD5 = MD5(getFile());
    if(md5 != tmpMD5)
        return true;
    else
        return false;
}

//==============================================================================
#if UNIT_TESTS

#include "../../Testsuite/MDLFile_test.h"

#endif

