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
const StringArray SAMCmd::generateFaustCodeCmd(const String& cmdStr,
                                               const String& inPath_,
                                               const String& outPath_)
{
    File fileOut(outPath_);
    String dataDir = StoredSettings::getInstance()->getDataDir();
    String pathMDX = dataDir;
    pathMDX << "/" << fileOut.getFileNameWithoutExtension() << ".mdx";

    if(cmdStr.compare("SAM-preprocessor") == 0)
        return getPerlScriptCmd(cmdStr, inPath_, pathMDX);
    else if (cmdStr.compare("Synth-A-Modeler") == 0)
        return getPerlScriptCmd(cmdStr, pathMDX, outPath_);
}

const StringArray SAMCmd::generateExternalCmd(const String& mdlPath,
                                              const String& exporter_)
{
    StringArray args;
    args.add("/bin/bash");
    args.add("-c");
    String processStr = "export PATH=${PATH}:";
    processStr << Utils::fixPath(StoredSettings::getInstance()->getFaustDir());
    processStr << " && cd " << Utils::fixPath(StoredSettings::getInstance()->getDataDir());
    processStr << " && " << exporter_;
    processStr = processStr.replace("$(DATA_DIR)", Utils::fixPath(StoredSettings::getInstance()->getDataDir()), true);
    File mdlFile(mdlPath);
    processStr = processStr.replace("$(MDL_NAME)", mdlFile.getFileNameWithoutExtension(), true);
    processStr = processStr.replace("$(FAUST_DIR)", Utils::fixPath(StoredSettings::getInstance()->getFaustDir()), true);

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
    StringArray args;
    args.add(cmdPerl);
    args.add(Utils::fixPath(StoredSettings::getInstance()->getDataDir()) + "/" + script);
    args.add(Utils::fixPath(inPath_));
    args.add(Utils::fixPath(outPath_));

    return args;
}

bool SAMCmd::runProcess(const StringArray& args, String& output)
{
    File tmpWorkDir = File::getCurrentWorkingDirectory();

    File dataDir(StoredSettings::getInstance()->getDataDir());
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

        StringArray args = generateFaustCodeCmd("SAM-preprocessor", tmpInPath, tmpOutPath);

        postWindow->postLocked("Command: " + args.joinIntoString(" "), PostLevel::ALL, true);

        postWindow->postLocked("Run SAM-preprocessor...");

        String outputStr;
        if(! generateFaustCodeProcess(args, outputStr))
        {
            postWindow->postLocked("Something went wrong!", PostLevel::ERROR);
            return;
        }
        processOutput = outputStr;

        if(processOutput.isNotEmpty())
            postWindow->postLocked("Output: " + processOutput);

        postWindow->postLocked("Done!");

        args = generateFaustCodeCmd("Synth-A-Modeler", tmpInPath, tmpOutPath);

        postWindow->postLocked("Command: " + args.joinIntoString(" "), PostLevel::ALL, true);

        postWindow->postLocked("Run Synth-A-Modeler...");

        if(! generateFaustCodeProcess(args, outputStr))
        {
            postWindow->postLocked("Something went wrong!", PostLevel::ERROR);
            return;
        }
        processOutput = outputStr;

        if(false)
        {
            File samLogFile(StoredSettings::getInstance()->getDataDir()
                            + "/" + "SAM-debug-compilation.txt");
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
    }
    else if (type == BINARY)
    {
        String tmpInPath = inPath;

        bool saveInDataDir = copyInfileToDataDirIfNeeded(tmpInPath);

        StringArray args = generateExternalCmd(tmpInPath, exporter);
        postWindow->postLocked("Command: "+ args.joinIntoString(" "), PostLevel::ALL, true);

        postWindow->postLocked("Run generate binary...");

        String outputStr;
        if(! generateExternalProcess(args, outputStr))
        {
            postWindow->postLocked("Something went wrong!", PostLevel::ERROR);
            return;
        }
        processOutput = outputStr;

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
    String dataDir = StoredSettings::getInstance()->getDataDir();

    File inDataDir(dataDir + "/" + in.getFileName());
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
    String outFileName = in.getFileNameWithoutExtension();
    outFileName << ".dsp";

    String dataDir = StoredSettings::getInstance()->getDataDir();
    String outPath_ = dataDir;
    outPath_ << "/" << outFileName;

    return outPath_;
}

//==============================================================================
#if UNIT_TESTS

#include "../../Testsuite/SAMCmd_test.h"

#endif
