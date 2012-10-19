/*
  ==============================================================================

    SnapGridPainter.cpp
    Created: 4 Oct 2012 2:48:21pm
    Author:  Peter Vasil

  ==============================================================================

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Controller/ObjController.h"
#include "ObjectsHolder.h"

#include "SnapGridPainter.h"

using namespace synthamodeler;
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

        g.setColour (col.withAlpha (0.2f));
        g.setPixel (0, 0);
    }

    if (backgroundFill.isValid())
    {
        g.setTiledImageFill (backgroundFill, 0, 0, 1.0f);
        g.fillAll();
    }
}
