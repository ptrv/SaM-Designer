/*
  ==============================================================================

    OutputCmd.h
    Created: 11 Apr 2012 3:17:58pm
    Author:  peter

  ==============================================================================
*/

#ifndef __OUTPUTCMD_H_1206E4C5__
#define __OUTPUTCMD_H_1206E4C5__

class OutputCmd {
public:
	OutputCmd();
	~OutputCmd();

	bool isSynthAModelerCmdAvailable();
	bool isFaustAvailable();

private:
};


#endif  // __OUTPUTCMD_H_1206E4C5__
