/*
  ==============================================================================

    SAMLookAndFeel.h
    Created: 18 May 2012 7:55:55pm
    Author:  peter

  ==============================================================================
*/

#ifndef __SAMLOOKANDFEEL_H_F27BC606__
#define __SAMLOOKANDFEEL_H_F27BC606__

namespace synthamodeler
{
/**
 * This class is the theme for the application's widgets (buttons, text, etc...)
 */
class SAMLookAndFeel : public LookAndFeel {
public:
	SAMLookAndFeel();

    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown);

    void drawPopupMenuItem (Graphics& g,
                            int width, int height,
                            bool isSeparator,
                            bool isActive,
                            bool isHighlighted,
                            bool isTicked,
                            bool hasSubMenu,
                            const String& text,
                            const String& shortcutKeyText,
                            Image* image,
                            const Colour * const textColour);
private:
};
}

#endif  // __SAMLOOKANDFEEL_H_F27BC606__
