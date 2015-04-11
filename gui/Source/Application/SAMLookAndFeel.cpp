/*
  ==============================================================================

    SAMLookAndFeel.cpp
    Created: 18 May 2012 7:55:55pm
    Author:  peter

  ==============================================================================
*/

#include "SAMLookAndFeel.h"

#include "Models/ObjectIDs.h"

#include "Utilities/ResourceLoader.h"


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
    ScopedPointer<Colour> customTextColor;
    Drawable* objectIcon = nullptr;

    if (!isSeparator)
    {
        if (text.compare("Mass") == 0)
        {
            customTextColor = new Colour(menuColourInsertMass);
            objectIcon = ResourceLoader::getInstance()->getDrawableForObjectId(Ids::mass);
        }
        else if (text.compare("Ground") == 0)
        {
            customTextColor = new Colour(menuColourInsertGround);
            objectIcon = ResourceLoader::getInstance()->getDrawableForObjectId(Ids::ground);
        }
        else if (text.compare("Resonator") == 0)
        {
            customTextColor = new Colour(menuColourInsertResonator);
            objectIcon = ResourceLoader::getInstance()->getDrawableForObjectId(Ids::resonators);
        }
        else if (text.compare("Port") == 0)
        {
            customTextColor = new Colour(menuColourInsertPort);
            objectIcon = ResourceLoader::getInstance()->getDrawableForObjectId(Ids::port);
        }
        else if (text.compare("Linear Link") == 0)
        {
            customTextColor = new Colour(menuColourInsertLink);
            objectIcon = ResourceLoader::getInstance()->createSVGDrawable("link-no-connectors.svg");
        }
        else if (text.compare("Touch Link") == 0)
        {
            customTextColor = new Colour(menuColourInsertTouch);
            objectIcon = ResourceLoader::getInstance()->createSVGDrawable("touch-no-connectors.svg");
        }
        else if (text.compare("Pulsetouch Link") == 0)
        {
            customTextColor = new Colour(menuColourInsertPulsetouch);
        }
        else if (text.compare("Pluck Link") == 0)
        {
            customTextColor = new Colour(menuColourInsertPluck);
            objectIcon = ResourceLoader::getInstance()->createSVGDrawable("pluck-no-connectors.svg");
        }
        else if (text.compare("Audio Out") == 0
            || text.compare("Audio Output") == 0)
        {
            customTextColor = new Colour(menuColourInsertAudioOut);
            objectIcon = ResourceLoader::getInstance()->getDrawableForObjectId(Ids::audioout);
        }
        else if (text.compare("Audio Connection") == 0)
        {
            customTextColor = new Colour(menuColourInsertAudioConnection);
        }
        else if (text.compare("Waveguide") == 0)
        {
            customTextColor = new Colour(menuColourInsertWaveguide);
            objectIcon = ResourceLoader::getInstance()->createSVGDrawable("waveguide-short.svg");
        }
        else if (text.compare("Junction") == 0)
        {
            customTextColor = new Colour(menuColourInsertJunction);
            objectIcon = ResourceLoader::getInstance()->getDrawableForObjectId(Ids::junction);
        }
        else if (text.compare("Termination") == 0)
        {
            customTextColor = new Colour(menuColourInsertTermination);
            objectIcon = ResourceLoader::getInstance()->getDrawableForObjectId(Ids::termination);
        }
    }

    if (isSeparator)
    {
        Rectangle<int> r (area.reduced (5, 0));
        r.removeFromTop (r.getHeight() / 2 - 1);

        g.setColour (Colour (0x33000000));
        g.fillRect (r.removeFromTop (1));

        g.setColour (Colour (0x66ffffff));
        g.fillRect (r.removeFromTop (1));
    }
    else
    {
        Colour textColour (findColour (PopupMenu::textColourId));

        if (customTextColor != nullptr)
        {
            textColour = *customTextColor.get();
        }
        else if (textColourToUse != nullptr)
        {
            textColour = *textColourToUse;
        }

        Rectangle<int> r (area.reduced (1));

        if (isHighlighted)
        {
            g.setColour (findColour (PopupMenu::highlightedBackgroundColourId));
            g.fillRect (r);

            g.setColour (findColour (PopupMenu::highlightedTextColourId));
        }
        else
        {
            g.setColour (textColour);
        }

        if (! isActive)
            g.setOpacity (0.3f);

        Font font (getPopupMenuFont());

        const float maxFontHeight = area.getHeight() / 1.3f;

        if (font.getHeight() > maxFontHeight)
            font.setHeight (maxFontHeight);

        g.setFont (font);

        Rectangle<float> iconArea (r.removeFromLeft ((r.getHeight() * 5) / 4).reduced (3).toFloat());

        if (objectIcon != nullptr)
        {
            objectIcon->drawWithin (g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize,
                                    isActive ? 1.0f : 0.3f);
        }
        else if (icon != nullptr)
        {
            icon->drawWithin (g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
        }
        else if (isTicked)
        {
            const Path tick (getTickShape (1.0f));
            g.fillPath (tick, tick.getTransformToScaleToFit (iconArea, true));
        }

        if (hasSubMenu)
        {
            const float arrowH = 0.6f * getPopupMenuFont().getAscent();

            const float x = (float) r.removeFromRight ((int) arrowH).getX();
            const float halfH = (float) r.getCentreY();

            Path p;
            p.addTriangle (x, halfH - arrowH * 0.5f,
                           x, halfH + arrowH * 0.5f,
                           x + arrowH * 0.6f, halfH);

            g.fillPath (p);
        }

        r.removeFromRight (3);
        g.drawFittedText (text, r, Justification::centredLeft, 1);

        if (shortcutKeyText.isNotEmpty())
        {
            Font f2 (font);
            f2.setHeight (f2.getHeight() * 0.75f);
            f2.setHorizontalScale (0.95f);
            g.setFont (f2);

            g.drawText (shortcutKeyText, r, Justification::centredRight, true);
        }
    }

}
