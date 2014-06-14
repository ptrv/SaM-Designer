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


namespace synthamodeler
{
class SelectableObject;
class BaseObjectComponent;
class ObjController;
class ObjectComponent;

class LinkComponent : public BaseObjectComponent,
                      public ChangeListener,
                      public ChangeBroadcaster,
                      public SettableTooltipClient,
                      public SelectableObject
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
    
    void changeListenerCallback (ChangeBroadcaster* source);
    
    bool sameStartEnd(ValueTree linkTree) const;
    
    void reverseDirection();
    
    Rectangle<int> getIntersectioBounds() const;
    
    Point<int> getPinPos() const;

    void setSegmented(bool isSegmented) { segmented = isSegmented; resized(); }

    void addChangeListener (ChangeListener* listener);
    void removeChangeListener (ChangeListener* listener);
    void removeAllChangeListeners();

private:
    void drawPath(Graphics& g);

    float lastInputX, lastInputY, lastOutputX, lastOutputY;
    float curInputX, curInputY, curOutputX, curOutputY;
    Path linePath, hitPath, iconPath;
    
    bool segmented;
    bool mouseDownSelectStatus;
    
    Point<float> iconPos;
    int iconWidth;
    int iconHeight;

    WeakReference<ObjectComponent> startComp;
    WeakReference<ObjectComponent> endComp;

    int numListener;

    Colour color;
    Colour colorSelected;
    Colour currentColor;
    void getDistancesFromEnds (int x, int y, double& distanceFromStart, double& distanceFromEnd) const
    {
        float x1, y1, x2, y2;
        getPoints (x1, y1, x2, y2);

        distanceFromStart = juce_hypot (x - (x1 - getX()), y - (y1 - getY()));
        distanceFromEnd = juce_hypot (x - (x2 - getX()), y - (y2 - getY()));
    }

    WeakReference<LinkComponent>::Master masterReference;
    friend class WeakReference<LinkComponent>;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinkComponent);
};
}


#endif  // __LINKCOMPONENT_H_4CCE4B86__
