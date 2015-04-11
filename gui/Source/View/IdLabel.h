/*
  ==============================================================================

    IdLabel.h
    Created: 29 Jun 2014 11:42:35pm
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

#ifndef IDLABEL_H_INCLUDED
#define IDLABEL_H_INCLUDED

#include "JuceHeader.h"

//==============================================================================
/*
*/
namespace synthamodeler
{

class IdLabel    : public CallOutBox
{
public:
    IdLabel(Component& contentComponent,
            const Rectangle<int>& areaToPointTo,
            Component* parentComponent_)
        : CallOutBox(contentComponent, areaToPointTo, parentComponent_)
    {
    }

    bool hitTest (int, int) override { return false; }
};


class IdLabelComp : public Component
{
public:
    IdLabelComp(const String& s) : labelText(s), font(14.f)
    {
        textWidth = font.getStringWidth(labelText);
        setSize(textWidth, font.getHeight());
    }

    void paint(Graphics& g)
    {
        g.setColour(Colours::white);
        g.setFont(font);
        g.drawText(labelText, 0, 0,
                   getWidth(), font.getHeight(),
                   Justification::left, false);
    }

private:
    String labelText;
    Font font;
    int textWidth;
};

}

#endif  // IDLABEL_H_INCLUDED
