/*
  ==============================================================================

    SnapGridPainter.cpp
    Created: 4 Oct 2012 2:48:21pm
    Author:  peter

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Controller/ObjController.h"
#include "ObjectsHolder.h"

#include "SnapGridPainter.h"

//==============================================================================
SnapGridPainter::SnapGridPainter()
    : snapGridSize (-1),
      snapShown (false),
      backgroundFill (0)
{
}

SnapGridPainter::~SnapGridPainter()
{
}

bool SnapGridPainter::updateFromDesign (ObjectsHolder& design)
{
    if (snapGridSize != design.getSnappingGridSize()
         || snapShown != (design.isSnapShown() && design.isSnapActive (false)))
    {
        snapGridSize = design.getSnappingGridSize();
        snapShown = design.isSnapShown() && design.isSnapActive (false);

        backgroundFill = Image();

        return true;
    }

    return false;
}

void SnapGridPainter::updateColour()
{
    backgroundFill = Image();
}

void SnapGridPainter::draw (Graphics& g)
{
    if (backgroundFill.isNull() && snapShown)
    {
        backgroundFill = Image (Image::ARGB, snapGridSize, snapGridSize, true);

        Graphics g (backgroundFill);

        Colour col (Colours::black);

        if (snapGridSize > 2)
        {
            g.setColour (col.withAlpha (0.1f));
            g.drawRect (0, 0, snapGridSize + 1, snapGridSize + 1);
        }

        g.setColour (col.withAlpha (0.35f));
        g.setPixel (0, 0);
    }

    if (backgroundFill.isValid())
    {
        g.setTiledImageFill (backgroundFill, 0, 0, 1.0f);
        g.fillAll();
    }
}
