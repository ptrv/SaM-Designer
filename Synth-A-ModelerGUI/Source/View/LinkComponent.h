/*
  ==============================================================================

    LinkComponent.h
    Created: 21 Aug 2012 11:23:02am
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

#ifndef __LINKCOMPONENT_H_4CCE4B86__
#define __LINKCOMPONENT_H_4CCE4B86__

#include "BaseObjectComponent.h"


class ObjController;
class ObjectComponent;

class LinkComponent : public BaseObjectComponent,
                      public ChangeListener,
                      public SettableTooltipClient
{
public:
    LinkComponent(ObjController& owner_, ValueTree linkTree);
    ~LinkComponent();
    
    void resized();
    void paint(Graphics& g);
    
    void update();
    
    void mouseDown(const MouseEvent& e);
    void mouseDrag(const MouseEvent& e);
    void mouseUp(const MouseEvent& e);
    
    bool hitTest(int x, int y);
    
    void resizeToFit();
    
    void setSelected(bool shouldBeSelected);
    
    void getPoints (float& x1, float& y1, float& x2, float& y2) const;
    
    void changeListenerCallback (ChangeBroadcaster*);
    
    void showContextMenu();
    
    bool sameStartEnd(ValueTree linkTree);
    
    void reverseDirection();
    
private:
    ObjectsHolder* getObjectsHolder() const noexcept;
    void drawPath(float x1, float y1, float x2, float y2);
    
    float lastInputX, lastInputY, lastOutputX, lastOutputY;
    Path linePath, hitPath;
    
    bool segmented;
    bool selected;
    bool mouseDownSelectStatus;
    
    Point<float> iconPos;
    
    ObjectComponent* startComp;
    ObjectComponent* endComp;
    
    void getDistancesFromEnds (int x, int y, double& distanceFromStart, double& distanceFromEnd) const
    {
        float x1, y1, x2, y2;
        getPoints (x1, y1, x2, y2);

        distanceFromStart = juce_hypot (x - (x1 - getX()), y - (y1 - getY()));
        distanceFromEnd = juce_hypot (x - (x2 - getX()), y - (y2 - getY()));
    }
};


#endif  // __LINKCOMPONENT_H_4CCE4B86__
