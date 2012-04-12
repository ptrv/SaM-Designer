/*
  ==============================================================================

    DebugWindow.h
    Created: 11 Apr 2012 3:18:15pm
    Author:  Peter Vasil

  ==============================================================================
*/

#ifndef __DEBUGWINDOW_H_3A3F156F__
#define __DEBUGWINDOW_H_3A3F156F__

#include "../JuceLibraryCode/JuceHeader.h"

class TextConsole;
class DebugWindow : public DocumentWindow {
public:
	DebugWindow();
	~DebugWindow();

	void closeButtonPressed();

	void toggleDebugWindow();

	void clear();

	void addText(const String& compilerText);

private:
	TextConsole* console;
};


#endif  // __DEBUGWINDOW_H_3A3F156F__
