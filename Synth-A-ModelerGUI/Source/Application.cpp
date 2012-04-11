/*
  ==============================================================================

    Application.cpp
    Created: 11 Apr 2012 1:28:10pm
    Author:  Peter Vasil

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Application.h"
#if UNIT_TESTS
#include "../Testsuite/TestRunner.h"
#endif



//==============================================================================

SynthAModelerApplication::SynthAModelerApplication()
{
}

SynthAModelerApplication::~SynthAModelerApplication()
{
}

//==============================================================================
void SynthAModelerApplication::initialise (const String& commandLine)
{
#if UNIT_TESTS
		if(commandLine.contains("--test"))
		{
            TestRunner::doTests();
			quit();
            return;
		}
#endif

	// Do your application's initialisation code here..
	mainWindow = new MainAppWindow();
}

void SynthAModelerApplication::shutdown()
{
	// Do your application's shutdown code here..
	mainWindow = 0;
}

//==============================================================================
void SynthAModelerApplication::systemRequestedQuit()
{
	quit();
}

//==============================================================================
const String SynthAModelerApplication::getApplicationName()
{
	return "Synth-A-Modeler";
}

const String SynthAModelerApplication::getApplicationVersion()
{
	return ProjectInfo::versionString;
}

bool SynthAModelerApplication::moreThanOneInstanceAllowed()
{
	return true;
}

void SynthAModelerApplication::anotherInstanceStarted (const String& commandLine)
{

}

