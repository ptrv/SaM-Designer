/*
  ==============================================================================

    SAMLookAndFeel.h
    Created: 18 May 2012 7:55:55pm
    Author:  peter

  ==============================================================================
*/

#ifndef __SAMLOOKANDFEEL_H_F27BC606__
#define __SAMLOOKANDFEEL_H_F27BC606__

#include "JuceHeader.h"

namespace synthamodeler
{

/**
 * This class is the theme for the application's widgets (buttons, text, etc...)
 */
class SAMLookAndFeel : public LookAndFeel_V3
{
public:
    void drawPopupMenuItem(Graphics& g,
                           const Rectangle<int>& area,
                           bool isSeparator,
                           bool isActive,
                           bool isHighlighted,
                           bool isTicked,
                           bool hasSubMenu,
                           const String& text,
                           const String& shortcutKeyText,
                           const Drawable* icon,
                           const Colour * const textColour);
};
}

#endif  // __SAMLOOKANDFEEL_H_F27BC606__
