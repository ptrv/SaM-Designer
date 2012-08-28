/*
  ==============================================================================

    AudioOutConnector.h
    Created: 27 Aug 2012 2:11:55pm
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

#ifndef __AUDIOOUTCONNECTOR_H_D0DAD273__
#define __AUDIOOUTCONNECTOR_H_D0DAD273__

//#include "ObjectComponent.h"


class ObjController;
class ObjectComponent;
class SelectableObject;

class AudioOutConnector : public Component,
                          public ChangeListener,
                          public SelectableObject
{
public:
    AudioOutConnector(ObjController& owner_,
                      ObjectComponent* objComp_,
                      ObjectComponent* audioOutComp_);
    virtual ~AudioOutConnector();

    void resized();
    void paint(Graphics& g);

    void update();
    
    void resizeToFit();

    bool hitTest(int x, int y);
    
    void getPoints(float& x1, float& y1, float& x2, float& y2) const;

    void changeListenerCallback (ChangeBroadcaster*);
    
    void mouseDown(const MouseEvent& e);
    void mouseDrag(const MouseEvent& e);
    void mouseUp(const MouseEvent& e);

    ObjectComponent* getSourceObject() { return objComp; }
    ObjectComponent* getAudioObject() { return audioOutComp; }
    
    Rectangle<int> getIntersectioBounds();
private:
    ObjController& owner;
    float lastInputX, lastInputY, lastOutputX, lastOutputY;
    Path linePath, hitPath;

    bool mouseDownSelectStatus;
    ObjectComponent* objComp;
    ObjectComponent* audioOutComp;
    
    void getDistancesFromEnds (int x, int y, double& distanceFromStart, double& distanceFromEnd) const
    {
        float x1, y1, x2, y2;
        getPoints (x1, y1, x2, y2);

        distanceFromStart = juce_hypot (x - (x1 - getX()), y - (y1 - getY()));
        distanceFromEnd = juce_hypot (x - (x2 - getX()), y - (y2 - getY()));
    }
    
};


#endif  // __AUDIOOUTCONNECTOR_H_D0DAD273__
