/*
  ==============================================================================

    Application.cpp
    Created: 11 Apr 2012 1:28:10pm
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
	return false;
}

void SynthAModelerApplication::anotherInstanceStarted (const String& commandLine)
{

}

