/*
  ==============================================================================

    SAMLookAndFeel.cpp
    Created: 18 May 2012 7:55:55pm
    Author:  peter

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"

#include "SAMLookAndFeel.h"

static const Colour menuColourInsertMass(0xffff0000);
//static const Colour menuColourInsertGround(0xff800000);
static const Colour menuColourInsertGround(0xff935632);
static const Colour menuColourInsertResonator(0xff660066);
//static const Colour menuColourInsertPort(0xffae0600);
static const Colour menuColourInsertPort(0xffc67b2d);
static const Colour menuColourInsertLink(0xff00b700);
static const Colour menuColourInsertTouch(0xff006f00);
static const Colour menuColourInsertPluck(0xff006f00);
static const Colour menuColourInsertAudioOut(0xff000000);
static const Colour menuColourInsertAudioConnection(0xff333333);
static const Colour menuColourInsertWaveguide(0xff000090);
static const Colour menuColourInsertJunction(0xff000090);
static const Colour menuColourInsertTermination(0xff000090);

SAMLookAndFeel::SAMLookAndFeel()
{
    setColour(TextButton::buttonColourId, Colour(0xffeeeeff));
}

void SAMLookAndFeel::drawButtonBackground (Graphics& g,
                                                  Button& button,
                                                  const Colour& backgroundColour,
                                                  bool isMouseOverButton,
                                                  bool isButtonDown)
{
    const bool flatOnLeft   = button.isConnectedOnLeft();
    const bool flatOnRight  = button.isConnectedOnRight();
    const bool flatOnTop    = button.isConnectedOnTop();
    const bool flatOnBottom = button.isConnectedOnBottom();

    const float width  = button.getWidth();
    const float height = button.getHeight();

    const float x = 0.5f;
    const float y = 0.5f;
    const float w = width  - 1.0f;
    const float h = height - 1.0f;
    const float cornerSize = 4.0f;

    Colour baseColour (backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true)
                                                                      ? 4.3f : 0.9f)
                                       .withMultipliedAlpha (button.isEnabled() ? 0.9f : 0.5f));

    if (isButtonDown)           baseColour = baseColour.contrasting (0.2f);
    else if (isMouseOverButton) baseColour = baseColour.contrasting (0.1f);

    const float mainBrightness = baseColour.getBrightness();
    const float mainAlpha = baseColour.getFloatAlpha();

    Path outline;
    outline.addRoundedRectangle (x, y, w, h, cornerSize, cornerSize,
                                 ! (flatOnLeft  || flatOnTop),
                                 ! (flatOnRight || flatOnTop),
                                 ! (flatOnLeft  || flatOnBottom),
                                 ! (flatOnRight || flatOnBottom));

    g.setGradientFill (ColourGradient (baseColour.brighter (0.2f), 0.0f, 0.0f,
                                       baseColour.darker (0.25f), 0.0f, height, false));
    g.fillPath (outline);

    g.setColour (Colours::white.withAlpha (0.4f * mainAlpha * mainBrightness * mainBrightness));
    g.strokePath (outline, PathStrokeType (1.0f), AffineTransform::translation (0.0f, 1.0f)
                                                        .scaled (1.0f, (h - 1.6f) / h));

    g.setColour (Colours::black.withAlpha (0.4f * mainAlpha));
    g.strokePath (outline, PathStrokeType (1.0f));
}


void SAMLookAndFeel::drawPopupMenuItem(Graphics& g,
                                       int width, int height,
                                       const bool isSeparator,
                                       const bool isActive,
                                       const bool isHighlighted,
                                       const bool isTicked,
                                       const bool hasSubMenu,
                                       const String& text,
                                       const String& shortcutKeyText,
                                       Image* image,
                                       const Colour * const textColourToUse)
{
    const float halfH = height * 0.5f;

    if (isSeparator)
    {
        const float separatorIndent = 5.5f;

        g.setColour (Colour (0x33000000));
        g.drawLine (separatorIndent, halfH, width - separatorIndent, halfH);

        g.setColour (Colour (0x66ffffff));
        g.drawLine (separatorIndent, halfH + 1.0f, width - separatorIndent, halfH + 1.0f);
    }
    else
    {
        Colour textColour (findColour (PopupMenu::textColourId));

        if (textColourToUse != nullptr)
            textColour = *textColourToUse;

        if (isHighlighted)
        {
            g.setColour (findColour (PopupMenu::highlightedBackgroundColourId));
            g.fillRect (1, 1, width - 2, height - 2);

            g.setColour (findColour (PopupMenu::highlightedTextColourId));
        }
        else
        {
            if(text.compare("Mass") == 0)
                textColour = menuColourInsertMass;
            else if(text.compare("Ground") == 0)
                textColour = menuColourInsertGround;
            else if(text.compare("Resonator") == 0)
                textColour = menuColourInsertResonator;
            else if(text.compare("Port") == 0)
                textColour = menuColourInsertPort;
            else if(text.compare("Linear Link") == 0)
                textColour = menuColourInsertLink;
            else if(text.compare("Touch Link") == 0)
                textColour = menuColourInsertTouch;
            else if(text.compare("Pluck Link") == 0)
                textColour = menuColourInsertPluck;
            else if(text.compare("Audio Out") == 0)
                textColour = menuColourInsertAudioOut;
            else if(text.compare("Audio Connection") == 0)
                textColour = menuColourInsertAudioConnection;
            else if(text.compare("Waveguide") == 0)
                textColour = menuColourInsertWaveguide;
            else if(text.compare("Junction") == 0)
                textColour = menuColourInsertJunction;
            else if(text.compare("Termination") == 0)
                textColour = menuColourInsertTermination;
            g.setColour (textColour);
        }

        if (! isActive)
            g.setOpacity (0.3f);

        Font font (getPopupMenuFont());

        if (font.getHeight() > height / 1.3f)
            font.setHeight (height / 1.3f);

        g.setFont (font);

        const int leftBorder = (height * 5) / 4;
        const int rightBorder = 4;

        if (image != nullptr)
        {
            g.drawImageWithin (*image,
                               2, 1, leftBorder - 4, height - 2,
                               RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, false);
        }
        else if (isTicked)
        {
            const Path tick (getTickShape (1.0f));
            const float th = font.getAscent();
            const float ty = halfH - th * 0.5f;

            g.fillPath (tick, tick.getTransformToScaleToFit (2.0f, ty, (float) (leftBorder - 4),
                                                             th, true));
        }

        g.drawFittedText (text,
                          leftBorder, 0, width - (leftBorder + rightBorder), height,
                          Justification::centredLeft, 1);

        if (shortcutKeyText.isNotEmpty())
        {
            Font f2 (font);
            f2.setHeight (f2.getHeight() * 0.75f);
            f2.setHorizontalScale (0.95f);
            g.setFont (f2);

            g.drawText (shortcutKeyText,
                        leftBorder, 0, width - (leftBorder + rightBorder + 4), height,
                        Justification::centredRight,
                        true);
        }

        if (hasSubMenu)
        {
            const float arrowH = 0.6f * getPopupMenuFont().getAscent();
            const float x = width - height * 0.6f;

            Path p;
            p.addTriangle (x, halfH - arrowH * 0.5f,
                           x, halfH + arrowH * 0.5f,
                           x + arrowH * 0.6f, halfH);

            g.fillPath (p);
        }
    }
}
