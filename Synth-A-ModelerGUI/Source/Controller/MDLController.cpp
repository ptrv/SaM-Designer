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

#ifdef JUCE_LINUX
static const bool USE_NATIVE_FILECHOOSER = false;
#else
static const bool USE_NATIVE_FILECHOOSER = true;
#endif

MDLController::MDLController()
{
	currentMdl = new MDLFile();
	outCmd = new OutputCmd();
}

MDLController::~MDLController()
{

}

void MDLController::newFile()
{
	if (saveIfNeededAndUserAgrees (currentMdl) == FileBasedDocument::savedOk)
	{
		currentMdl->newMDL();
	}
}
void MDLController::open()
{
	if (saveIfNeededAndUserAgrees (currentMdl) == FileBasedDocument::savedOk)
	{
		FileChooser fch("Choose mdl file",
				StoredSettings::getInstance()->getWorkingFolder(), "*.mdl",
				USE_NATIVE_FILECHOOSER);

		if (fch.browseForFileToOpen())
		{
			File result = fch.getResult();
			bool loadOK = currentMdl->openMDL(result.getFullPathName().toUTF8().getAddress());
			if(loadOK)
			{
				StoredSettings::getInstance()->setWorkingFolder(result.getParentDirectory().getFullPathName());
				StoredSettings::getInstance()->recentFiles.addFile(result);
			}
			else
			{
				DBG("Something went wrong loading the mdl file.");
			}
		}
    }
}

void MDLController::openFromFile(const File& mdlFile)
{
	if (saveIfNeededAndUserAgrees (currentMdl) == FileBasedDocument::savedOk)
	{
		bool loadOk = currentMdl->openMDL(mdlFile.getFullPathName().toUTF8().getAddress());
		if(loadOk)
		{
			StoredSettings::getInstance()->setWorkingFolder(mdlFile.getParentDirectory().getFullPathName());
		}
		else
		{
			DBG("Something went wrong loading the mdl file.");
		}
	}
}
void MDLController::save()
{
	if(currentMdl->hasNotBeenSavedYet())
	{
		saveAs();
	}
	else if(saveIfNeededAndUserAgrees(currentMdl) == FileBasedDocument::savedOk)
	{
		bool saveOk = currentMdl->save(currentMdl->getFilePath());
		if(! saveOk)
		{
			DBG("Something went wrong saving the mdl file.")
		}
	}
}
void MDLController::saveAs()
{
	FileChooser fch("Choose mdl file",
			StoredSettings::getInstance()->getWorkingFolder(), "*.mdl",
			USE_NATIVE_FILECHOOSER);

	if(fch.browseForFileToSave(true))
	{
		File result = fch.getResult();
		StoredSettings::getInstance()->setWorkingFolder(result.getParentDirectory().getFullPathName());
		bool saveOk = currentMdl->save(result.getFullPathName());
		if(! saveOk)
		{
			DBG("Something went wrong saving the mdl file.")
		}
	}
}

FileBasedDocument::SaveResult MDLController::saveIfNeededAndUserAgrees (MDLFile* mdl)
{
    if (! mdl->needsSaving())
        return FileBasedDocument::savedOk;

    const int r = AlertWindow::showYesNoCancelBox (AlertWindow::QuestionIcon,
                                                   TRANS("Closing document..."),
                                                   TRANS("Do you want to save the changes to \"")
                                                       + mdl->getName() + "\"?",
                                                   TRANS("save"),
                                                   TRANS("discard changes"),
                                                   TRANS("cancel"));

    if (r == 1)
    {
        // save changes
        return mdl->save(mdl->getFilePath()) ? FileBasedDocument::savedOk
                           : FileBasedDocument::failedToWriteToFile;
    }
    else if (r == 2)
    {
        // discard changes
        return FileBasedDocument::savedOk;
    }

    return FileBasedDocument::userCancelledSave;
}
void MDLController::close()
{
	if(saveIfNeededAndUserAgrees(currentMdl) == FileBasedDocument::savedOk)
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
	// TODO: implement generate external
	if(currentMdl->getName().compare("Untitled") == 0)
		return "No mdl file\n\n";


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
