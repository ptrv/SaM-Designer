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

#include "../Application/CommonHeaders.h"
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
	File samCompiler(StoredSettings::getInstance()->getDataDir()+"/Synth-A-Modeler");
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
	String cmdFaust = StoredSettings::getInstance()->getFaustDir() + "/faust";
	return isCmdAvailable(cmdFaust);
}
//const String OutputCmd::runChildProcess(const String& processStr)
//{
//	ChildProcess child;
//	if(child.start(processStr))
//	{
//		const String result (child.readAllProcessOutput());
//
//		child.waitForProcessToFinish (60 * 1000);
//
//		return result;
//	}
//	else
//	{
//		return "failed to start process";
//	}
//}
static String execProcess(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    String result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
    }
    pclose(pipe);
    return result;
}
const String OutputCmd::generateFaustCode(const String& inPath, const String& outPath)
{
	String cmdPerl = StoredSettings::getInstance()->getCmdPerl();
	String processStr = "/bin/bash -c \"";
	processStr << cmdPerl << " " << StoredSettings::getInstance()->getDataDir();
	processStr << "/Synth-A-Modeler " << inPath << " " << outPath << " 2>&1\" 2>&1";

	SAM_LOG("Synth-A-Modeler command: " + processStr);
	String processoutput = execProcess(processStr.toUTF8().getAddress());
	return processoutput;
}

const String OutputCmd::generateExternal()
{
	String processStr = "/bin/bash -c \"export PATH=${PATH}:";
	processStr << StoredSettings::getInstance()->getFaustDir();
	processStr << " ; ";
	processStr << StoredSettings::getInstance()->getCmdExporter();
	processStr = processStr.replace("$(DATA_DIR)", StoredSettings::getInstance()->getDataDir(), true);
	processStr << " 2>&1\" 2>&1";

	SAM_LOG("Export command: " + processStr);
	String processoutput = execProcess(processStr.toUTF8().getAddress());
	return processoutput;
}

//==============================================================================
#if UNIT_TESTS

#include "../../Testsuite/OutputCmd_test.h"

#endif
