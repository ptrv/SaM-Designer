/*
  ==============================================================================

    SAMLookAndFeel.cpp
    Created: 18 May 2012 7:55:55pm
    Author:  peter

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"

#include "SAMLookAndFeel.h"

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
                                                                      ? 1.3f : 0.9f)
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