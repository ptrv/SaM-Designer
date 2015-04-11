/*
  ==============================================================================

    SAMCmd.h
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

#ifndef __SAMCMD_H_1206E4C5__
#define __SAMCMD_H_1206E4C5__

#include "JuceHeader.h"

namespace synthamodeler
{
class MDLFile;

/**
 * This class encapsulates the external commands for generating FAUST code
 * and other externals.
 */
class SAMCmd : public ThreadPoolJob
{
public:
    enum CmdType
    {
        FAUSTCODE,
        BINARY,
        NONE
    };

	SAMCmd();
	~SAMCmd();

    void setupJob(const String& input, CmdType cmdType_,
                  bool runSAMBeforeBinary = false);
    
    const Array<StringArray> generateFaustCodeCmd(const String& inPath_,
                                                  const String& outPath_);

    bool generateFaustCodeProcess(const StringArray& args, String& output);

    const StringArray generateExternalCmd(const String& mdlPath,
                                          const String& exporter);
    bool generateExternalProcess(const StringArray& args, String& output);

//    const String& getProcessOutput() const { return processOutput; }

    JobStatus runJob();

    CmdType getCurrentType() const { return cmdType; }

    static Time lastFaustCodeGenTime;

private:
    const StringArray getPerlScriptCmd(const String& script,
                                       const String& inPath_,
                                       const String& outPath_);

    bool runProcess(const StringArray& args, String& output);

    bool copyInfileToDataDirIfNeeded(String& inPath_);

    const String getOutPath(const String& inPath_);

    void generate(CmdType type);

    String processOutput;

    CmdType cmdType;
    String inPath, outPath, exporter;
    bool compileBeforeExportBinary;
};
}


#endif  // __SAMCMD_H_1206E4C5__
