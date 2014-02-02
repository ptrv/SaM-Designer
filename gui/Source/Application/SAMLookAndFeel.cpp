/*
  ==============================================================================

    SAMLookAndFeel.cpp
    Created: 18 May 2012 7:55:55pm
    Author:  peter

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"

#include "SAMLookAndFeel.h"

using namespace synthamodeler;

static const uint32 menuColourInsertMass = 0xffff0000;
//static const uint32 menuColourInsertGround = 0xff800000;
static const uint32 menuColourInsertGround = 0xff935632;
static const uint32 menuColourInsertResonator = 0xff660066;
//static const uint32 menuColourInsertPort = 0xffae0600;
static const uint32 menuColourInsertPort = 0xffc67b2d;
static const uint32 menuColourInsertLink = 0xff00b700;
static const uint32 menuColourInsertTouch = 0xff006f00;
static const uint32 menuColourInsertPluck = 0xff006f00;
static const uint32 menuColourInsertPulsetouch = 0xff006f00;
static const uint32 menuColourInsertAudioOut = 0xff000000;
static const uint32 menuColourInsertAudioConnection = 0xff333333;
static const uint32 menuColourInsertWaveguide = 0xff000090;
static const uint32 menuColourInsertJunction = 0xff000090;
static const uint32 menuColourInsertTermination = 0xff000090;

void SAMLookAndFeel::drawPopupMenuItem(Graphics& g,
                                       const Rectangle<int>& area,
                                       const bool isSeparator,
                                       const bool isActive,
                                       const bool isHighlighted,
                                       const bool isTicked,
                                       const bool hasSubMenu,
                                       const String& text,
                                       const String& shortcutKeyText,
                                       const Drawable* icon,
                                       const Colour * const textColourToUse)
{
    ScopedPointer<Colour> textColour;

    if (!isSeparator && !isHighlighted)
    {
        if (text.compare("Mass") == 0)
            textColour = new Colour(menuColourInsertMass);
        else if (text.compare("Ground") == 0)
            textColour = new Colour(menuColourInsertGround);
        else if (text.compare("Resonator") == 0)
            textColour = new Colour(menuColourInsertResonator);
        else if (text.compare("Port") == 0)
            textColour = new Colour(menuColourInsertPort);
        else if (text.compare("Linear Link") == 0)
            textColour = new Colour(menuColourInsertLink);
        else if (text.compare("Touch Link") == 0)
            textColour = new Colour(menuColourInsertTouch);
        else if (text.compare("Pulsetouch Link") == 0)
            textColour = new Colour(menuColourInsertPulsetouch);
        else if (text.compare("Pluck Link") == 0)
            textColour = new Colour(menuColourInsertPluck);
        else if (text.compare("Audio Out") == 0)
            textColour = new Colour(menuColourInsertAudioOut);
        else if (text.compare("Audio Connection") == 0)
            textColour = new Colour(menuColourInsertAudioConnection);
        else if (text.compare("Waveguide") == 0)
            textColour = new Colour(menuColourInsertWaveguide);
        else if (text.compare("Junction") == 0)
            textColour = new Colour(menuColourInsertJunction);
        else if (text.compare("Termination") == 0)
            textColour = new Colour(menuColourInsertTermination);
    }

    LookAndFeel_V3::drawPopupMenuItem(g, area, isSeparator, isActive,
                                      isHighlighted, isTicked, hasSubMenu, text,
                                      shortcutKeyText, icon,
                                      textColour ? textColour.get() : textColourToUse);
}
