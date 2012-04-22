/*
  ==============================================================================

    OutputCmd.cpp
    Created: 11 Apr 2012 3:17:58pm
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
#include "OutputCmd.h"
#include "../Utilities/StoredSettings.h"

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
const String OutputCmd::runChildProcess(const String& processStr)
{
	ChildProcess child;
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
const String OutputCmd::generateFaustCode(const String& inPath, const String& outPath)
{
	String cmdPerl = StoredSettings::getInstance()->getCmdPerl();
	String cmdSAM = StoredSettings::getInstance()->getCmdSAM();
	String processStr = cmdPerl +" " + cmdSAM + " " + inPath + " " + outPath;

	return runChildProcess(processStr);
}

const String OutputCmd::generateExternalSC(const String& inPath)
{
	String cmdSC = StoredSettings::getInstance()->getCmdFaust2supercollider();
	String processStr = cmdSC + " " + inPath;

	return runChildProcess(processStr);
}
const String OutputCmd::generateExternalPD(const String& inPath)
{
	String cmdPD = StoredSettings::getInstance()->getCmdFaust2puredata();
	String processStr = cmdPD + " " + inPath;

	return runChildProcess(processStr);

}


//==============================================================================
#if UNIT_TESTS

#include "../../Testsuite/OutputCmd_test.h"

#endif
