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
	if (currentMdl->saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
	{
		currentMdl->newMDL();
	}
}
void MDLController::open()
{
	if (currentMdl->saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
	{
		bool loadOk = currentMdl->loadFromUserSpecifiedFile(true);

		if(loadOk)
		{
			StoredSettings::getInstance()->recentFiles.addFile(currentMdl->getFile());
		}
		else
		{
			DBG("Something went wrong loading the mdl file.");
		}
	}
}

void MDLController::openFromFile(const File& mdlFile)
{
	if(currentMdl->saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
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
	if(currentMdl->saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
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
