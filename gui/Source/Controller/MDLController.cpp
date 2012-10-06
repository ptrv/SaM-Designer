/*
  ==============================================================================

    MDLController.cpp
    Created: 12 Apr 2012 11:49:49pm
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
#include "../Application/MainWindow.h"
#include "../Models/MDLFile.h"
#include "../Models/SAMCmd.h"

#include "MDLController.h"


MDLController::MDLController(MainAppWindow& mainAppWindow_)
: mainAppWindow(mainAppWindow_),
  currentMdl(nullptr)
{
	samCmd = new SAMCmd();
}

MDLController::~MDLController()
{
	currentMdl = nullptr;
	samCmd = nullptr;
}

void MDLController::newFile()
{
	FileBasedDocument::SaveResult sr = currentMdl->saveIfNeededAndUserAgrees();
	if (sr != FileBasedDocument::userCancelledSave)
	{
		currentMdl->newMDL();
	}
}
void MDLController::open()
{
	FileBasedDocument::SaveResult sr = currentMdl->saveIfNeededAndUserAgrees();
	if (sr != FileBasedDocument::userCancelledSave)
	{
		bool loadOk = currentMdl->loadFromUserSpecifiedFile(true);

		if(loadOk)
		{
			StoredSettings::getInstance()->recentFiles.addFile(currentMdl->getFile());
		}
//		else
//		{
//			DBG("Something went wrong loading the mdl file.");
//		}
	}
}

void MDLController::openFromFile(const File& mdlFile)
{
	FileBasedDocument::SaveResult sr = currentMdl->saveIfNeededAndUserAgrees();
	if (sr != FileBasedDocument::userCancelledSave)
	{
		currentMdl->loadFrom(mdlFile, true);
	}
}
void MDLController::save()
{
    if(currentMdl->changedOutside())
    {

    }
    else
    {
        if (currentMdl->save(true, true) != FileBasedDocument::savedOk)
        {
            SAM_LOG("Something went wrong saving the mdl file.");
        }
    }
}
void MDLController::saveAs()
{
	if(currentMdl->saveAsInteractive(true) != FileBasedDocument::savedOk)
	{
		SAM_LOG("Something went wrong saving the mdl file.");
	}
}

void MDLController::close()
{
	FileBasedDocument::SaveResult sr = currentMdl->saveIfNeededAndUserAgrees();
	if(sr != FileBasedDocument::userCancelledSave)
	{
		currentMdl->close();
	}
}

bool MDLController::saveAsXml()
{
    return currentMdl->saveAsXml();
}

const String MDLController::generateFaust()
{
	if(! samCmd->isPerlAvailable())
	{
		Alerts::missingPerl();
		return "Missing Perl";
	}
	if(! samCmd->isSAMpreprocessorCmdAvailable())
	{
		Alerts::missingSAMpreprocessor();
		return "Missing SAM-preprocessor";
	}
	if(! samCmd->isSynthAModelerCmdAvailable())
	{
		Alerts::missingSAM();
		return "Missing Synth-A-Modeler";
	}

	bool r = true;
	if(StoredSettings::getInstance()->getIsExportConfirm())
		r = Alerts::confirmExport("Really export faust");

	if (r)
	{
        String inPath = currentMdl->getFilePath();
		File in(inPath);
		String outFileName= in.getFileNameWithoutExtension();
		outFileName << ".dsp";

        String dataDir = StoredSettings::getInstance()->getDataDir();
		String outPath = dataDir;
		outPath << "/" << outFileName;
        File inDataDir(dataDir + "/" + in.getFileName());
        bool saveInDataDir = false;
        if (in != inDataDir)
        {
            saveInDataDir = true;
            currentMdl->getFile().copyFileTo(inDataDir);
            inPath = inDataDir.getFullPathName();
        }
		String processText = samCmd->generateFaustCode(inPath, outPath);
        if(StoredSettings::getInstance()->getOpenFaustExport())
            Utils::openFileNative(outPath);

        if(saveInDataDir)
        {
            if(! inDataDir.deleteFile())
            {
                DBG("Deleting temp file failed!");
            }
        }
        return processText;
	}
	return String::empty;
}

const String MDLController::generateExternal()
{
	if(currentMdl->getName().compare("Untitled") == 0)
		return "No mdl file\n\n";

	if(! samCmd->isFaustAvailable())
	{
		Alerts::missingFaust();
		return "Missing faust executable";
	}

	bool r = true;
	if(StoredSettings::getInstance()->getIsExportConfirm())
		r = Alerts::confirmExport("Really export faust");

	if (r)
	{
        String outStr;
        if(StoredSettings::getInstance()->getRunSAMBeforeExternal())
            outStr << generateFaust();
        outStr << samCmd->generateExternal();
        return outStr;
	}
	return String::empty;
}

const String MDLController::getMDLName()
{
	return currentMdl->getName();
}

UndoManager* MDLController::getUndoManager()
{
	if(currentMdl == nullptr)
		return nullptr;

	return &currentMdl->getUndoMgr();

}

bool MDLController::perform (UndoableAction* const action, const String& actionName)
{
	if(getUndoManager() != nullptr)
	{
		getUndoManager()->beginNewTransaction(actionName);
		return getUndoManager()->perform(action, actionName);
	}
	else
	{
		return false;
	}
}

ValueTree MDLController::getMDLTree()
{
	if(currentMdl == nullptr)
		return ValueTree::invalid;

	return currentMdl->mdlRoot;
}

bool MDLController::mdlCheckAndSaveIfNeeded()
{
	FileBasedDocument::SaveResult sr = currentMdl->saveIfNeededAndUserAgrees();
	if(sr == FileBasedDocument::userCancelledSave)
		return false;
	else
		return true;

}

MDLFile* MDLController::getMDLFile() const
{
	return currentMdl.get();
}

void MDLController::setMDLFile(MDLFile* mdlFile)
{
	currentMdl = mdlFile;
	if(currentMdl != nullptr)
	{
		StoredSettings::getInstance()->recentFiles.addFile(currentMdl->getFile());
		StoredSettings::getInstance()->flush();
	}
}

void MDLController::changed()
{
    currentMdl->changed();
    mainAppWindow.updateTitle();
}

ObjectsHolder* MDLController::getHolderComponent()
{
    return mainAppWindow.getHolderComponent();
}