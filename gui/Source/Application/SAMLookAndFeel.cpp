/*
  ==============================================================================

    SAMLookAndFeel.cpp
    Created: 18 May 2012 7:55:55pm
    Author:  peter

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"

#include "../Models/ObjectIDs.h"
#include "../Utilities/ResourceLoader.h"

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
    Drawable* objectIcon = nullptr;

    if (!isSeparator)
    {
        if (text.compare("Mass") == 0)
        {
            textColour = new Colour(menuColourInsertMass);
            objectIcon = ResourceLoader::getInstance()->getDrawableForId(Ids::mass);
        }
        else if (text.compare("Ground") == 0)
        {
            textColour = new Colour(menuColourInsertGround);
            objectIcon = ResourceLoader::getInstance()->getDrawableForId(Ids::ground);
        }
        else if (text.compare("Resonator") == 0)
        {
            textColour = new Colour(menuColourInsertResonator);
            objectIcon = ResourceLoader::getInstance()->getDrawableForId(Ids::resonators);
        }
        else if (text.compare("Port") == 0)
        {
            textColour = new Colour(menuColourInsertPort);
            objectIcon = ResourceLoader::getInstance()->getDrawableForId(Ids::port);
        }
        else if (text.compare("Linear Link") == 0)
        {
            textColour = new Colour(menuColourInsertLink);
            objectIcon = ResourceLoader::getInstance()->createSVGDrawable("link-no-connectors.svg");
        }
        else if (text.compare("Touch Link") == 0)
        {
            textColour = new Colour(menuColourInsertTouch);
            objectIcon = ResourceLoader::getInstance()->createSVGDrawable("touch-no-connectors.svg");
        }
        else if (text.compare("Pulsetouch Link") == 0)
        {
            textColour = new Colour(menuColourInsertPulsetouch);
        }
        else if (text.compare("Pluck Link") == 0)
        {
            textColour = new Colour(menuColourInsertPluck);
            objectIcon = ResourceLoader::getInstance()->createSVGDrawable("pluck-no-connectors.svg");
        }
        else if (text.compare("Audio Out") == 0
            || text.compare("Audio Output") == 0)
        {
            textColour = new Colour(menuColourInsertAudioOut);
            objectIcon = ResourceLoader::getInstance()->getDrawableForId(Ids::audioout);
        }
        else if (text.compare("Audio Connection") == 0)
        {
            textColour = new Colour(menuColourInsertAudioConnection);
        }
        else if (text.compare("Waveguide") == 0)
        {
            textColour = new Colour(menuColourInsertWaveguide);
            objectIcon = ResourceLoader::getInstance()->createSVGDrawable("waveguide-short.svg");
        }
        else if (text.compare("Junction") == 0)
        {
            textColour = new Colour(menuColourInsertJunction);
            objectIcon = ResourceLoader::getInstance()->getDrawableForId(Ids::junction);
        }
        else if (text.compare("Termination") == 0)
        {
            textColour = new Colour(menuColourInsertTermination);
            objectIcon = ResourceLoader::getInstance()->getDrawableForId(Ids::termination);
        }
    }

    LookAndFeel_V3::drawPopupMenuItem(g, area, isSeparator, isActive,
                                      isHighlighted, isTicked, hasSubMenu, text,
                                      shortcutKeyText,
                                      objectIcon ? objectIcon : icon,
                                      textColour ? textColour.get() : textColourToUse);
}
