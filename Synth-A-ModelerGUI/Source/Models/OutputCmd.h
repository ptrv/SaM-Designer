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

#include "../../JuceLibraryCode/JuceHeader.h"

class OutputCmd
{
public:
	OutputCmd();
	~OutputCmd();

	bool isSynthAModelerCmdAvailable();
	bool isCmdAvailable(const String& cmdStr);
	bool isPerlAvailable();
	const String generateFaustCode(const String& inPath, const String& outPath);
	const String generateExternal();
};


#endif  // __OUTPUTCMD_H_1206E4C5__
