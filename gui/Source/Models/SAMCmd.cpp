/*
  ==============================================================================

    SAMCmd.cpp
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
#include "../Models/MDLFile.h"

#include "SAMCmd.h"

SAMCmd::SAMCmd()
{

}

SAMCmd::~SAMCmd()
{
}

bool SAMCmd::isSynthAModelerCmdAvailable()
{
	File samCompiler(StoredSettings::getInstance()->getDataDir()+"/Synth-A-Modeler");
	if(samCompiler.existsAsFile())
		return true;
	else
		return false;
}
bool SAMCmd::isSAMpreprocessorCmdAvailable()
{
	File samCompiler(StoredSettings::getInstance()->getDataDir()+"/SAM-preprocessor");
	if(samCompiler.existsAsFile())
		return true;
	else
		return false;
}

bool SAMCmd::isCmdAvailable(const String& cmdStr)
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

bool SAMCmd::isPerlAvailable()
{
	String cmdPerl = StoredSettings::getInstance()->getCmdPerl();
	return isCmdAvailable(cmdPerl);
}

bool SAMCmd::isFaustAvailable()
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
    while(!feof(pipe))
    {
        if (fgets(buffer, 128, pipe) != NULL)
        {
            result += String (CharPointer_UTF8 (buffer));
        }
    }
    pclose(pipe);
    return result;
}
const String SAMCmd::generateFaustCode(const String& inPath, const String& outPath)
{
    File fileOut(outPath);
    String dataDir = StoredSettings::getInstance()->getDataDir();
    String pathMDX = dataDir;
    pathMDX << "/" << fileOut.getFileNameWithoutExtension() << ".mdx";
	String processoutput = runPerlScript("SAM-preprocessor", inPath, pathMDX);
    processoutput << runPerlScript("Synth-A-Modeler", pathMDX, outPath);
	return processoutput;
}

const String SAMCmd::generateExternal()
{
    String currentExporter = StoredSettings::getInstance()->getCurrentExporter();
    String exporterValue = StoredSettings::getInstance()->getExporters().getValue(currentExporter, "");
	String processStr = "/bin/bash -c \"export PATH=${PATH}:";
	processStr << Utils::fixPath(StoredSettings::getInstance()->getFaustDir());
	processStr << " ; ";
    processStr << exporterValue;
	processStr = processStr.replace("$(DATA_DIR)", Utils::fixPath(StoredSettings::getInstance()->getDataDir()), true);
	processStr << " 2>&1\" 2>&1";

	SAM_LOG("Export command: " + processStr);
    SAM_CONSOLE_ADD_LINE(processStr+"\n", true);
	String processoutput = execProcess(processStr.toUTF8().getAddress());
	return processoutput;
}

const String SAMCmd::runPerlScript(const String& script,
                                      const String& inPath,
                                      const String& outPath)
{
    String cmdPerl = StoredSettings::getInstance()->getCmdPerl();
	String processStr = "/bin/bash -c \"";
    processStr << "cd " << Utils::fixPath(StoredSettings::getInstance()->getDataDir()) << ";";
	processStr << cmdPerl << " " << Utils::fixPath(StoredSettings::getInstance()->getDataDir());
	processStr << "/" << script << " " << Utils::fixPath(inPath) << " " << Utils::fixPath(outPath) << " 2>&1\" 2>&1";

	SAM_LOG(script + " command: " + processStr);
    SAM_CONSOLE_ADD_LINE(processStr+"\n", true);
	return execProcess(processStr.toUTF8().getAddress());
//	return runChildProcess(processStr.toUTF8().getAddress());

}

//==============================================================================
#if UNIT_TESTS

#include "../../Testsuite/SAMCmd_test.h"

#endif
