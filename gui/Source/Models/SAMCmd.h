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

	SAMCmd(const String& input, CmdType cmdType_,
        bool runSAMBeforeBinary = false);
	~SAMCmd();

	/**
	 * Checks wether the Synth-A-Modeler script is available.
	 *
	 * @return			true if Synth-A-Modeler is found
	 */
	static bool isSynthAModelerCmdAvailable();
	/**
	 * Checks wether the SAM-preprocessor script is available.
	 *
	 * @return			true if SAM-preprocessor is found
	 */
	static bool isSAMpreprocessorCmdAvailable();
	/**
	 * Checks wether a specifdied command is available.
	 *
	 * @param cmdStr	command string
	 * @return			true if specifdied command is available
	 */
	static bool isCmdAvailable(const String& cmdStr);
	/**
	 * Checks wether perl is available.
	 *
	 * @return			true if the perl executable is found
	 */
	static bool isPerlAvailable();
	/**
	 * Checks wether faust executable is available.
	 *
	 * @return			true if faust executable is found
	 */
	static bool isFaustAvailable();
    
    const StringArray generateFaustCodeCmd(const String& cmdStr,
                                           const String& inPath_,
                                           const String& outPath_);

    const String generateFaustCodeProcess(const StringArray& args);

    const StringArray generateExternalCmd(const String& mdlPath,
                                         const String& exporter);
    const String generateExternalProcess(const StringArray& args);

    const String& getProcessOutput() const { return processOutput; }

    JobStatus runJob();

private:
    const StringArray getPerlScriptCmd(const String& script,
                                       const String& inPath_,
                                       const String& outPath_);

    const String runProcess(StringArray args);

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
