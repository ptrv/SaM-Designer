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

#include "SAMCmd.h"

#include "Application/CommonHeaders.h"

#include "Models/MDLFile.h"

#include "Utilities/CmdHelper.h"


using namespace synthamodeler;

Time SAMCmd::lastFaustCodeGenTime = Time();

SAMCmd::SAMCmd()
: ThreadPoolJob("SAMCmd job"), cmdType(NONE), inPath(String::empty),
    outPath(String::empty), exporter(String::empty)
{
    setJobName("SAMCmd job " + Time::getCurrentTime().formatted("%F %T"));
}

SAMCmd::~SAMCmd()
{
}

void SAMCmd::setupJob(const String& input, CmdType cmdType_, bool runSAMBeforeBinary)
{
    inPath = input;
    outPath = getOutPath(inPath);
    cmdType = cmdType_;
    compileBeforeExportBinary = runSAMBeforeBinary;

    if (cmdType == BINARY)
    {
        String currentExporter = StoredSettings::getInstance()->getCurrentExporter();
        exporter = StoredSettings::getInstance()->getExporters()
            .getValue(currentExporter, "");
    }
}


bool SAMCmd::generateFaustCodeProcess(const StringArray& args, String& output)
{
    return runProcess(args, output);
}
const Array<StringArray> SAMCmd::generateFaustCodeCmd(const String& inPath_,
                                                      const String& outPath_)
{
    File fileOut(outPath_);
    File dataDir = StoredSettings::getInstance()->getDataDir();
    String pathMDX = dataDir.getChildFile(
        fileOut.getFileName()).withFileExtension("mdx").getFullPathName();

    Array<StringArray> cmdArgs;
    cmdArgs.add(getPerlScriptCmd("SAM-preprocessor", inPath_, pathMDX));
    cmdArgs.add(getPerlScriptCmd("Synth-A-Modeler", pathMDX, outPath_));
    return cmdArgs;
}

const StringArray SAMCmd::generateExternalCmd(const String& mdlPath,
                                              const String& exporter_)
{
    String dataDir = Utils::fixPath(StoredSettings::getInstance()->getDataDir());
    File mdlFile = mdlPath;
    String mdlName = mdlFile.getFileNameWithoutExtension();
    String faustDir = Utils::fixPath(StoredSettings::getInstance()->getFaustDir());

    StringArray args;
    args.add("/bin/bash");
    args.add("-c");
    String processStr = "export PATH=${PATH}:";
    processStr << faustDir;
    processStr << " && cd " << dataDir;
    processStr << " && " << exporter_;
    processStr = processStr.replace("$(DATA_DIR)", dataDir, true);
    processStr = processStr.replace("$(MDL_NAME)", mdlName, true);
    processStr = processStr.replace("$(FAUST_DIR)",faustDir, true);

    args.add(processStr);

    return args;
}
bool SAMCmd::generateExternalProcess(const StringArray& args, String& output)
{
    return runProcess(args, output);
}

const StringArray SAMCmd::getPerlScriptCmd(const String& script,
                                           const String& inPath_,
                                           const String& outPath_)
{
    String cmdPerl = StoredSettings::getInstance()->getCmdPerl();
    File dataDir = StoredSettings::getInstance()->getDataDir();
    String scriptPath = dataDir.getChildFile(script).getFullPathName();

    StringArray args;
    args.add(cmdPerl);
    args.add(Utils::fixPath(scriptPath));
    args.add(Utils::fixPath(inPath_));
    args.add(Utils::fixPath(outPath_));

    return args;
}

bool SAMCmd::runProcess(const StringArray& args, String& output)
{
    File tmpWorkDir = File::getCurrentWorkingDirectory();

    File dataDir = StoredSettings::getInstance()->getDataDir();
    dataDir.setAsCurrentWorkingDirectory();

    ChildProcess child;
    bool execOk = true;
    if(child.start(args))
	{
		output = child.readAllProcessOutput();
		child.waitForProcessToFinish (60 * 1000);
	}
	else
	{
		output = "failed to start process";
        execOk = false;

	}
    tmpWorkDir.setAsCurrentWorkingDirectory();
    return execOk;
}

void SAMCmd::generate(CmdType type)
{
    if(type == FAUSTCODE)
    {
        String tmpInPath = inPath;
        String tmpOutPath = outPath;
        bool saveInDataDir = copyInfileToDataDirIfNeeded(tmpInPath);

        const Array<StringArray> args = generateFaustCodeCmd(tmpInPath, tmpOutPath);

        postWindow->postLocked(TRANS("Command") + ": " + args[0].joinIntoString(" "),
                               PostLevel::ALL, true);

        postWindow->postLocked(TRANS("Run SAM-preprocessor") + "...");

        String outputStr;
        if(! generateFaustCodeProcess(args[0], outputStr))
        {
            postWindow->postLocked(TRANS("Something went wrong!"), PostLevel::ERROR);
            return;
        }
        processOutput = outputStr;

        if(processOutput.isNotEmpty())
            postWindow->postLocked(TRANS("Output") + ": " + processOutput);

        postWindow->postLocked(TRANS("Done!"));

        postWindow->postLocked(TRANS("Command") + ": " + args[1].joinIntoString(" "),
                               PostLevel::ALL, true);

        postWindow->postLocked(TRANS("Run Synth-A-Modeler") + "...");

        if(! generateFaustCodeProcess(args[1], outputStr))
        {
            postWindow->postLocked(TRANS("Something went wrong!"), PostLevel::ERROR);
            return;
        }
        processOutput = outputStr;

        if(false)
        {
            File dataDir = StoredSettings::getInstance()->getDataDir();
            File samLogFile = dataDir.getChildFile("SAM-debug-compilation.txt");
            processOutput = samLogFile.loadFileAsString();
        }

        if(processOutput.isNotEmpty())
            postWindow->postLocked("Output: " + processOutput);

        postWindow->postLocked("Done!");

        if (saveInDataDir)
        {
            File in(tmpInPath);
            if (!in.deleteFile())
            {
                SAM_CONSOLE("Deleting temp file failed!", PostLevel::ERROR);
            }
        }

        lastFaustCodeGenTime = Time::getCurrentTime();
    }
    else if (type == BINARY)
    {
        String tmpInPath = inPath;

        bool saveInDataDir = copyInfileToDataDirIfNeeded(tmpInPath);

        StringArray args = generateExternalCmd(tmpInPath, exporter);
        postWindow->postLocked(TRANS("Command") + ": " + args.joinIntoString(" "),
                               PostLevel::ALL, true);

        postWindow->postLocked(TRANS("Run generate binary") + "...");

        String outputStr;
        if(! generateExternalProcess(args, outputStr))
        {
            postWindow->postLocked(TRANS("Something went wrong!"), PostLevel::ERROR);
            return;
        }
        processOutput = outputStr;

        if(processOutput.isNotEmpty())
            postWindow->postLocked(TRANS("Output") + ": " + processOutput);

        postWindow->postLocked(TRANS("Done!"));

        if (saveInDataDir)
        {
            File in(tmpInPath);
            if (!in.deleteFile())
            {
                SAM_CONSOLE("Deleting temp file failed!", PostLevel::ERROR);
            }
        }
    }
}
ThreadPoolJob::JobStatus SAMCmd::runJob()
{
    if(cmdType == FAUSTCODE)
    {
        generate(FAUSTCODE);
    }
    else if(cmdType == BINARY)
    {
        if(compileBeforeExportBinary)
            generate(FAUSTCODE);

        generate(BINARY);
    }
    return jobHasFinished;
}

bool SAMCmd::copyInfileToDataDirIfNeeded(String& inPath_)
{
    File in(inPath_);
    File dataDir = StoredSettings::getInstance()->getDataDir();

    File inDataDir = dataDir.getChildFile(in.getFileName());
    bool saveInDataDir = false;
    if (in != inDataDir)
    {
        saveInDataDir = true;
        in.copyFileTo(inDataDir);
        inPath_ = inDataDir.getFullPathName();
    }
    return saveInDataDir;
}

const String SAMCmd::getOutPath(const String& inPath_)
{
    File in(inPath_);
    String outFileName = in.withFileExtension("dsp").getFileName();

    File dataDir = StoredSettings::getInstance()->getDataDir();
    String outPath_ = dataDir.getChildFile(outFileName).getFullPathName();

    return outPath_;
}

//==============================================================================
#if UNIT_TESTS

#include "../../Testsuite/SAMCmd_test.h"

#endif
