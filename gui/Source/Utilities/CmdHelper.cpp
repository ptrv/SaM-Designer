/*
  ==============================================================================

    CmdHelper.cpp
    Created: 11 Jun 2014 12:27:37am
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

#include "CmdHelper.h"

#include "Application/CommonHeaders.h"


using namespace synthamodeler;

//------------------------------------------------------------------------------

bool CmdHelper::isSynthAModelerCmdAvailable()
{
    File samCompiler(StoredSettings::getInstance()->getDataDir()+"/Synth-A-Modeler");
    if(samCompiler.existsAsFile())
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------

bool CmdHelper::isSAMpreprocessorCmdAvailable()
{
    File samCompiler(StoredSettings::getInstance()->getDataDir()+ "/SAM-preprocessor");
    if(samCompiler.existsAsFile())
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------

bool CmdHelper::isCmdAvailable(const String& cmdStr)
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

//------------------------------------------------------------------------------

bool CmdHelper::isPerlAvailable()
{
    String cmdPerl = StoredSettings::getInstance()->getCmdPerl();
    return isCmdAvailable(cmdPerl);
}

//------------------------------------------------------------------------------

bool CmdHelper::isFaustAvailable()
{
#ifdef JUCE_WINDOWS
    String cmdFaust = StoredSettings::getInstance()->getFaustDir() + "/faust.exe";
#else
    String cmdFaust = StoredSettings::getInstance()->getFaustDir() + "/faust";
#endif
    return isCmdAvailable(cmdFaust);
}

//------------------------------------------------------------------------------
