/*
  ==============================================================================

    OutputCmd.cpp
    Created: 11 Apr 2012 3:17:58pm
    Author:  Peter Vasil

  ==============================================================================
*/

#include "OutputCmd.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "StoredSettings.h"

OutputCmd::OutputCmd()
{

}

OutputCmd::~OutputCmd()
{
}

bool OutputCmd::isSynthAModelerCmdAvailable()
{
	String cmdSAM = StoredSettings::getInstance()->getCmdSAM();
	File samCompiler(File::getCurrentWorkingDirectory().getChildFile(cmdSAM));
	if(samCompiler.existsAsFile())
		return true;
	else
		return false;
}

bool OutputCmd::isCmdAvailable(const String& cmdStr)
{
	ChildProcess child;
	const bool ok = child.start (cmdStr)
			&& child.readAllProcessOutput().trim().isNotEmpty();

	child.waitForProcessToFinish (60 * 1000);
	return ok;
}

bool OutputCmd::isPerlAvailable()
{
	String cmdPerl = StoredSettings::getInstance()->getCmdPerl();
	return isCmdAvailable("which "+cmdPerl);
}

bool OutputCmd::isFaustAvailable()
{
	String cmdFaust = StoredSettings::getInstance()->getCmdFaust();
	return isCmdAvailable("which "+cmdFaust);
}

bool OutputCmd::isFaust2puredataAvailable()
{
	String cmdFaust2puredata = StoredSettings::getInstance()->getCmdFaust2puredata();
	return isCmdAvailable("which "+cmdFaust2puredata);
}

bool OutputCmd::isFaust2supercolliderAvailable()
{
	String cmdFaust2supercollider = StoredSettings::getInstance()->getCmdFaust2supercollider();
	return isCmdAvailable("which "+cmdFaust2supercollider);
}

const String OutputCmd::generateFaustCode(const String& inPath, const String& outPath)
{
	ChildProcess child;

	String cmdPerl = StoredSettings::getInstance()->getCmdPerl();
	String cmdSAM = StoredSettings::getInstance()->getCmdSAM();
	String processStr = cmdPerl +" " + cmdSAM + " " + inPath + " " + outPath;

	if(child.start(processStr))
	{
		const String result (child.readAllProcessOutput());

		child.waitForProcessToFinish (60 * 1000);

		return result;
	}
	else
	{
		return "failed to start process";
	}
}

//==============================================================================
#if UNIT_TESTS

#include "../Testsuite/OutputCmd_test.h"

#endif
