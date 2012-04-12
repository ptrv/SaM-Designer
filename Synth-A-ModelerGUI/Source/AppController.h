/*
  ==============================================================================

    AppController.h
    Created: 12 Apr 2012 1:30:25am
    Author:  Peter Vasil

  ==============================================================================
*/

#ifndef __APPCONTROLLER_H_B0B552E9__
#define __APPCONTROLLER_H_B0B552E9__

#include "../JuceLibraryCode/JuceHeader.h"
#include "OutputCmd.h"
class DebugWindow;
class MainAppWindow;

class AppController {
public:
	AppController(MainAppWindow& maw_, DebugWindow& dw_);
	~AppController();

	void generateFaust();
	void generateExternal();
	void debugWindowToggle();
	void debugWindowClear();
private:
	MainAppWindow& maw;
	DebugWindow& dw;
	ScopedPointer<OutputCmd> outCmd;
};


#endif  // __APPCONTROLLER_H_B0B552E9__
