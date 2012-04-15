/*
  ==============================================================================

    AppController.cpp
    Created: 12 Apr 2012 1:30:26am
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

#include "AppController.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "MainWindow.h"
#include "DebugWindow.h"
#include "Alerts.h"

AppController::AppController(MainAppWindow& maw_, DebugWindow& dw_)
: maw(maw_), dw(dw_)
{
	outCmd = new OutputCmd();
}

AppController::~AppController()
{

}

void AppController::generateFaust()
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

void AppController::generateExternal()
{

}

void AppController::debugWindowToggle()
{
	dw.toggleDebugWindow();
	maw.toFront(true);

}

void AppController::debugWindowClear()
{
	dw.clear();
}
