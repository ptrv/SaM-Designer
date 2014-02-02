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

static const Colour menuColourInsertMass(0xffff0000);
//static const Colour menuColourInsertGround(0xff800000);
static const Colour menuColourInsertGround(0xff935632);
static const Colour menuColourInsertResonator(0xff660066);
//static const Colour menuColourInsertPort(0xffae0600);
static const Colour menuColourInsertPort(0xffc67b2d);
static const Colour menuColourInsertLink(0xff00b700);
static const Colour menuColourInsertTouch(0xff006f00);
static const Colour menuColourInsertPluck(0xff006f00);
static const Colour menuColourInsertPulsetouch(0xff006f00);
static const Colour menuColourInsertAudioOut(0xff000000);
static const Colour menuColourInsertAudioConnection(0xff333333);
static const Colour menuColourInsertWaveguide(0xff000090);
static const Colour menuColourInsertJunction(0xff000090);
static const Colour menuColourInsertTermination(0xff000090);

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
