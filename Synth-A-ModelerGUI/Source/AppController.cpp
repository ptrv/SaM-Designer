/*
  ==============================================================================

    AppController.cpp
    Created: 12 Apr 2012 1:30:26am
    Author:  peter

  ==============================================================================
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
