/*
  ==============================================================================

    BaseConnector.h
    Created: 5 Jul 2015 5:03:54pm
    Author:  Peter Vasil

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

  ==============================================================================
*/

#ifndef BASECONNECTOR_H_INCLUDED
#define BASECONNECTOR_H_INCLUDED

#include "JuceHeader.h"
#include "SelectableObject.h"

namespace synthamodeler
{
class ObjController;
class BaseObjectComponent;
class ObjectComponent;
class LinkComponent;

class BaseConnector : public Component,
                      public ChangeListener,
                      public SelectableObject
{
public:
    BaseConnector(ObjController& owner_);
    virtual ~BaseConnector();

    void resized();
    void paint(Graphics& g);

    void update();

    void resizeToFit();

    bool hitTest(int x, int y);

    virtual void getPoints(float& x1, float& y1, float& x2, float& y2) const = 0;

    virtual void changeListenerCallback (ChangeBroadcaster*) {}

    void mouseDown(const MouseEvent& e);
    void mouseDrag(const MouseEvent& e);
    void mouseUp(const MouseEvent& e);

    Rectangle<int> getIntersectioBounds();

    void setSegmented(bool isSegmented) { segmented = isSegmented; resized(); }
protected:
    ObjController& owner;
    float lastInputX, lastInputY, lastOutputX, lastOutputY;
    Path linePath, hitPath;

    bool segmented;
    bool mouseDownSelectStatus;

    void getDistancesFromEnds (int x, int y, double& distanceFromStart, double& distanceFromEnd) const
    {
        float x1, y1, x2, y2;
        getPoints (x1, y1, x2, y2);

        distanceFromStart = juce_hypot (x - (x1 - getX()), y - (y1 - getY()));
        distanceFromEnd = juce_hypot (x - (x2 - getX()), y - (y2 - getY()));
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseConnector);
};
}




#endif  // BASECONNECTOR_H_INCLUDED
