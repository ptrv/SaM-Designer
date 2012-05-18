/*
  ==============================================================================

    OutputCmd.h
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

#ifndef __OUTPUTCMD_H_1206E4C5__
#define __OUTPUTCMD_H_1206E4C5__

/**
 * This class encapsulates the external commands for generating FAUST code
 * and other externals.
 */
class OutputCmd
{
public:
	OutputCmd();
	~OutputCmd();

	/**
	 * Checks wether the Synth-A-Modeler script is available.
	 *
	 * @return			true if Synth-A-Modeler is found
	 */
	bool isSynthAModelerCmdAvailable();
	/**
	 * Checks wether a specifdied command is available.
	 *
	 * @param cmdStr	command string
	 * @return			true if specifdied command is available
	 */
	bool isCmdAvailable(const String& cmdStr);
	/**
	 * Checks wether perl is available.
	 *
	 * @return			true if the perl executable is found
	 */
	bool isPerlAvailable();
	/**
	 * Checks wether faust executable is available.
	 *
	 * @return			true if faust executable is found
	 */
	bool isFaustAvailable();
	/**
	 * Generates FAUST code for the current mdl file.
	 *
	 * @param inPath	path of the mdl file
	 * @param outPath	path of the output file
	 * @return			an empty string if generation of faust code succeeded,
	 * 					else a string with the error message.
	 */
	const String generateFaustCode(const String& inPath, const String& outPath);
	/**
	 * Generates external using the FAUST compiler.
	 *
	 * @return			an empty string if generation succeeded, else a string
	 * 					with error message
	 */
	const String generateExternal();
};


#endif  // __OUTPUTCMD_H_1206E4C5__
