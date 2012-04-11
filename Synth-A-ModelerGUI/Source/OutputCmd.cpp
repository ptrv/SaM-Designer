/*
  ==============================================================================

    OutputCmd.cpp
    Created: 11 Apr 2012 3:17:58pm
    Author:  peter

  ==============================================================================
*/

#include "OutputCmd.h"
#include "../JuceLibraryCode/JuceHeader.h"

OutputCmd::OutputCmd()
{

}

OutputCmd::~OutputCmd()
{

}

bool OutputCmd::isSynthAModelerCmdAvailable()
{
	return false;
}

bool OutputCmd::isFaustAvailable()
{
	ChildProcess child;
	const bool ok = child.start ("which faust")
					 && child.readAllProcessOutput().trim().isNotEmpty();

	child.waitForProcessToFinish (60 * 1000);
	return ok;
}


