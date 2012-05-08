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

#include "../../JuceLibraryCode/JuceHeader.h"
#include "MDLController.h"
#include "../Models/MDLFile.h"
#include "../Models/OutputCmd.h"
#include "../Application/CommonHeaders.h"
#include "../View/ExportPanel.h"
#include "AppController.h"
#include "../Models/ObjectIDs.h"


MDLController::MDLController(AppController& owner_)
: owner(owner_)
{
	currentMdl = new MDLFile();
	outCmd = new OutputCmd();
}

MDLController::~MDLController()
{

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
	if(currentMdl->save(true, true) != FileBasedDocument::savedOk)
	{
		DBG("Something went wrong saving the mdl file.")
	}
}
void MDLController::saveAs()
{
	if(currentMdl->saveAsInteractive(true) != FileBasedDocument::savedOk)
	{
		DBG("Something went wrong saving the mdl file.")
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

const String MDLController::generateFaust()
{
	if(! outCmd->isPerlAvailable())
	{
		Alerts::missingPerl();
		return "Missing Perl";
	}
	if(! outCmd->isSynthAModelerCmdAvailable())
	{
		Alerts::missingSAM();
		return "Missing Synth-A-Modeler.plx";
	}

	bool r = true;
	if(StoredSettings::getInstance()->getIsExportConfirm())
		r = Alerts::confirmExport("Really export faust");

	if (r)
	{
		File result(currentMdl->getFilePath());
		String outFileName= result.getFileNameWithoutExtension();
		outFileName << ".dsp";

		String outPath = StoredSettings::getInstance()->getDataDir();
		outPath << "/" << outFileName;
		return outCmd->generateFaustCode(currentMdl->getFilePath(), outPath);
	}
	return String::empty;
}

const String MDLController::generateExternal()
{
	if(currentMdl->getName().compare("Untitled") == 0)
		return "No mdl file\n\n";

	if(! outCmd->isFaustAvailable())
	{
		Alerts::missingFaust();
		return "Missing faust executable";
	}

	bool r = true;
	if(StoredSettings::getInstance()->getIsExportConfirm())
		r = Alerts::confirmExport("Really export faust");

	if (r)
	{
		return outCmd->generateExternal();
	}
	return String::empty;
}

const String MDLController::getMDLName()
{
	return currentMdl->getName();
}

UndoManager* MDLController::getUndoManager()
{
	if(currentMdl != nullptr)
		return &currentMdl->getUndoMgr();
	else
		return nullptr;
}

bool MDLController::perform (UndoableAction* const action, const String& actionName)
{
	return owner.perform(action, actionName);
}

ValueTree MDLController::getMDLTree()
{
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
