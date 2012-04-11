/*
  ==============================================================================

    DebugWindow.h
    Created: 11 Apr 2012 3:18:15pm
    Author:  peter

  ==============================================================================
*/

#ifndef __DEBUGWINDOW_H_3A3F156F__
#define __DEBUGWINDOW_H_3A3F156F__

#include "../JuceLibraryCode/JuceHeader.h"

class DebugWindow : public DocumentWindow {
public:
	DebugWindow();
	~DebugWindow();

	void closeButtonPressed();

	void toggleDebugWindow();

private:
};


#endif  // __DEBUGWINDOW_H_3A3F156F__
