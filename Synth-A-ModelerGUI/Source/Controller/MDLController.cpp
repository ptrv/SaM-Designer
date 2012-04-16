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
#include "../Utilities/OutputCmd.h"
#include "../Application/CommonHeaders.h"

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
	FileChooser fch("Choose mdl file", File::getCurrentWorkingDirectory(),
			"*.mdl", false);

	if (fch.browseForFileToOpen())
	{
		File result = fch.getResult();
		bool loadOK = currentMdl->openMDL(result.getFullPathName().toUTF8().getAddress());
	}

}
void MDLController::open()
{

}
void MDLController::save()
{

}
void MDLController::saveAs()
{

}
void MDLController::close()
{

}

void MDLController::generateFaust()
{
	if(! outCmd->isPerlAvailable())
	{
		Alerts::missingPerl();
		return;
	}
	if(! outCmd->isSynthAModelerCmdAvailable())
	{
		Alerts::missingSAM();
		return;
	}
	if(! outCmd->isFaustAvailable())
	{
		Alerts::missingFaust();
		return;
	}
//	FileChooser fch("Choose mdl file", File::getCurrentWorkingDirectory(),"*.mdl", false);
//
//	if (fch.browseForFileToOpen())
//	{
//		File result = fch.getResult();
//		String compilerText = outCmd->generateFaustCode(result.getFullPathName(), "examplelinks.dsp");
//		dw->addText(compilerText);
//	}
}

void MDLController::generateExternal()
{

}
