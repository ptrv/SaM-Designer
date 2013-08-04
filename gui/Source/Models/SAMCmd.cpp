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

using namespace synthamodeler;

SAMCmd::SAMCmd()
: Thread("SAM Cmd")
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
	File samCompiler(StoredSettings::getInstance()->getDataDir()+ "/SAM-preprocessor");
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
#elif JUCE_MAC || JUCE_WINDOWS
	if(! File::isAbsolutePath(cmdStr))
	{
		cmdStrTmp = File::getCurrentWorkingDirectory().getChildFile(cmdStr).getFullPathName();
	}
	File cmdFile(cmdStrTmp);
#ifdef JUCE_WINDOWS
	if(cmdFile.exists())
#else
	if(cmdFile.existsAsFile())
#endif
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
#ifdef JUCE_WINDOWS
	String cmdFaust = StoredSettings::getInstance()->getFaustDir() + "/faust.exe";
#else
	String cmdFaust = StoredSettings::getInstance()->getFaustDir() + "/faust";
#endif
	return isCmdAvailable(cmdFaust);
}

bool runExternalGen = false;
bool faustProcessWaits = false;
void SAMCmd::generateFaustCode(const String& inPath,
                               const String& outPath)
{
    if(isThreadRunning())
        return;

    runExternalGen = false;
    cmdArgsF = {inPath, outPath};
    startThread();
}
const String SAMCmd::generateFaustCodeProcess(const StringArray& args)
{
    String resultStr = runProcess(args);

    return resultStr;
}
const StringArray SAMCmd::generateFaustCodeCmd(const String& cmdStr,
                                               const String& inPath,
                                               const String& outPath)
{
    File fileOut(outPath);
    String dataDir = StoredSettings::getInstance()->getDataDir();
    String pathMDX = dataDir;
    pathMDX << "/" << fileOut.getFileNameWithoutExtension() << ".mdx";

    if(cmdStr.compare("SAM-preprocessor") == 0)
        return getPerlScriptCmd(cmdStr, inPath, pathMDX);
    else if (cmdStr.compare("Synth-A-Modeler") == 0)
        return getPerlScriptCmd(cmdStr, pathMDX, outPath);
}

void SAMCmd::generateExternal(const String& mdlPath,
                              const String& exporter)
{
    if(isThreadRunning())
        return;

    runExternalGen = true;
    cmdArgsE = {mdlPath, exporter};
    startThread();
}
const StringArray SAMCmd::generateExternalCmd(const String& mdlPath,
                                              const String& exporter)
{
    StringArray args;
    args.add("/bin/bash");
    args.add("-c");
    String processStr = "export PATH=${PATH}:";
    processStr << Utils::fixPath(StoredSettings::getInstance()->getFaustDir());
    processStr << " && cd " << Utils::fixPath(StoredSettings::getInstance()->getDataDir());
    processStr << " && " << exporter;
    processStr = processStr.replace("$(DATA_DIR)", Utils::fixPath(StoredSettings::getInstance()->getDataDir()), true);
    File mdlFile(mdlPath);
    processStr = processStr.replace("$(MDL_NAME)", mdlFile.getFileNameWithoutExtension(), true);
    processStr = processStr.replace("$(FAUST_DIR)", Utils::fixPath(StoredSettings::getInstance()->getFaustDir()), true);

    args.add(processStr);

    return args;
}
const String SAMCmd::generateExternalProcess(const StringArray& args)
{
    return runProcess(args);
}

const StringArray SAMCmd::getPerlScriptCmd(const String& script,
                                           const String& inPath,
                                           const String& outPath)
{
    String cmdPerl = StoredSettings::getInstance()->getCmdPerl();
    StringArray args;
    args.add(cmdPerl);
    args.add(Utils::fixPath(StoredSettings::getInstance()->getDataDir()) + "/" + script);
    args.add(Utils::fixPath(inPath));
    args.add(Utils::fixPath(outPath));

    return args;
}

const String SAMCmd::runProcess(StringArray args)
{
    File tmpWorkDir = File::getCurrentWorkingDirectory();

    File dataDir(StoredSettings::getInstance()->getDataDir());
    dataDir.setAsCurrentWorkingDirectory();

    String resultStr;
    ChildProcess child;
    if(child.start(args))
	{
		resultStr = child.readAllProcessOutput();
		child.waitForProcessToFinish (60 * 1000);
	}
	else
	{
		return "failed to start process";
	}
    tmpWorkDir.setAsCurrentWorkingDirectory();
    return resultStr;
}

void SAMCmd::run()
{
    if (runExternalGen)
    {
        StringArray args = generateExternalCmd(cmdArgsE.mdlPath,
                                               cmdArgsE.exporter);
        postWindow->postLocked("Command: ", args.joinIntoString(" "), true);

        postWindow->postLocked("Log: ", "Run generate binary...", false);

        processOutput = generateExternalProcess(args);

        postWindow->postLocked("Output: ", processOutput, false);

        postWindow->postLocked("Log: ", "Done!", false);
    }
    else
    {
        StringArray args = generateFaustCodeCmd("SAM-preprocessor",
                                                cmdArgsF.inPath,
                                                cmdArgsF.outPath);
        postWindow->postLocked("Command:", args.joinIntoString(" "), true);

        postWindow->postLocked("Log: ", "Run SAM-preprocessor...", false);

        processOutput = generateFaustCodeProcess(args);

        postWindow->postLocked("Output: ", processOutput, false);

        postWindow->postLocked("Log: ", "Done!", false);

        args = generateFaustCodeCmd("Synth-A-Modeler",
                                    cmdArgsF.inPath,
                                    cmdArgsF.outPath);

        postWindow->postLocked("Command: ", args.joinIntoString(" "), true);

        postWindow->postLocked("Log: ", "Run Synth-A-Modeler...", false);

        processOutput = generateFaustCodeProcess(args);

        if(true)
        {

            File samLogFile(StoredSettings::getInstance()->getDataDir()
                            + "/" + "SAM-debug-compilation.txt");
            processOutput = samLogFile.loadFileAsString();
        }

        postWindow->postLocked("Output: ", processOutput, false);

        postWindow->postLocked("Log: ", "Done!", false);
    }
}

//==============================================================================
#if UNIT_TESTS

#include "../../Testsuite/SAMCmd_test.h"

#endif
