/*
  ==============================================================================

    TextConsole.h
    Created: 4 Feb 2012 5:37:55pm
    Author:  peter

  ==============================================================================
 */

#ifndef __TEXTCONSOLE_H_16CA25D5__
#define __TEXTCONSOLE_H_16CA25D5__


#include "../JuceLibraryCode/JuceHeader.h"

class TextConsole : public TextEditor {
public:
	TextConsole() : TextEditor("Console")
	{
		setMultiLine (true);
		setReadOnly (true);
	}

	void addLine (const String& text)
	{
		setCaretPosition (getText ().length ());
		insertTextAtCaret(text);
	}


private:
};


#endif  // __TEXTCONSOLE_H_16CA25D5__
