/*
  ==============================================================================

    SAMLookAndFeel.h
    Created: 18 May 2012 7:55:55pm
    Author:  peter

  ==============================================================================
*/

#ifndef __SAMLOOKANDFEEL_H_F27BC606__
#define __SAMLOOKANDFEEL_H_F27BC606__

/**
 * This class is the theme for the application's widgets (buttons, text, etc...)
 */
class SAMLookAndFeel : public LookAndFeel {
public:
	SAMLookAndFeel();

    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown);

private:
};


#endif  // __SAMLOOKANDFEEL_H_F27BC606__
