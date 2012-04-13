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
	if(! File::isAbsolutePath(cmdSAM))
	{
		cmdSAM = File::getCurrentWorkingDirectory().getChildFile(cmdSAM).getFullPathName();
	}
	File samCompiler(cmdSAM);
	if(samCompiler.existsAsFile())
		return true;
	else
		return false;
}

bool OutputCmd::isCmdAvailable(const String& cmdStr)
{
	String cmdStrTmp = cmdStr;
#if JUCE_LINUX
	cmdStrTmp = "which "+cmdStr;

	ChildProcess child;
	const bool ok = child.start (cmdStrTmp)
			&& child.readAllProcessOutput().trim().isNotEmpty();

	child.waitForProcessToFinish (60 * 1000);
	return ok;
#elif JUCE_MAC
	if(! File::isAbsolutePath(cmdStr))
	{
		cmdStrTmp = File::getCurrentWorkingDirectory().getChildFile(cmdStr).getFullPathName();
	}
	File cmdFile(cmdStrTmp);
	if(cmdFile.existsAsFile())
		return true;
	else
		return false;
#else
	return false;
#endif
}

bool OutputCmd::isPerlAvailable()
{
	String cmdPerl = StoredSettings::getInstance()->getCmdPerl();
	return isCmdAvailable(cmdPerl);
}

bool OutputCmd::isFaustAvailable()
{
	String cmdFaust = StoredSettings::getInstance()->getCmdFaust();
	return isCmdAvailable(cmdFaust);
}

bool OutputCmd::isFaust2puredataAvailable()
{
	String cmdFaust2puredata = StoredSettings::getInstance()->getCmdFaust2puredata();
	return isCmdAvailable(cmdFaust2puredata);
}

bool OutputCmd::isFaust2supercolliderAvailable()
{
	String cmdFaust2supercollider = StoredSettings::getInstance()->getCmdFaust2supercollider();
	return isCmdAvailable(cmdFaust2supercollider);
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
