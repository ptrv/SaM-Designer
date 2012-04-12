/*
  ==============================================================================

    OutputCmd.h
    Created: 11 Apr 2012 3:17:58pm
    Author:  Peter Vasil

  ==============================================================================
*/

#ifndef __OUTPUTCMD_H_1206E4C5__
#define __OUTPUTCMD_H_1206E4C5__

#include "../JuceLibraryCode/JuceHeader.h"

class OutputCmd {
public:
	OutputCmd();
	~OutputCmd();

	bool isSynthAModelerCmdAvailable();
	bool isCmdAvailable(const String& cmdStr);
	bool isPerlAvailable();
	bool isFaustAvailable();
	bool isFaust2puredataAvailable();
	bool isFaust2supercolliderAvailable();
	const String generateFaustCode(const String& inPath, const String& outPath);
private:
};


#endif  // __OUTPUTCMD_H_1206E4C5__
