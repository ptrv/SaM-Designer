/*
  ==============================================================================

    BaseObjectComponent.h
    Created: 5 May 2012 4:31:59pm
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

#ifndef __OBJECTCOMPONENT_H_4AC10D4B__
#define __OBJECTCOMPONENT_H_4AC10D4B__

#include "JuceHeader.h"

#include "BaseObjectComponent.h"
#include "SelectableObject.h"

namespace synthamodeler
{
class ObjectsHolder;
class ObjController;
class LinkComponent;
class SelectableObject;
class IdLabel;

/**
 * The object component.
 */
class ObjectComponent : public BaseObjectComponent,
                        public ChangeListener,
                        public ChangeBroadcaster,
                        public SelectableObject
{
public:
    ObjectComponent(ObjController& _owner, ValueTree data_);
    ~ObjectComponent();
    bool hitTest(int x, int y);
    void paint(Graphics& g);
    void mouseDown(const MouseEvent& e);
    void mouseDrag(const MouseEvent& e);
    void mouseUp(const MouseEvent& e);
    void update();
    void setData(ValueTree dataTree);

    void setSelected(bool shouldBeSelected);

    void toggleSelected();

    void setPosition(Point<int> newPos, bool undoable);
    void setActualPosition(Point<int> pos, NotificationType n = sendNotification);

    Point<int> getActualPos() const { return actualPos; }

    void addLinkToObject(LinkComponent* link);
    void removeLinkFromObject(LinkComponent* link);

    Array<LinkComponent*> getAttachedLinks() { return connectedLinks; }

    Point<int> getPinPos() const;
    Point<int> getPinOffset() const;

    bool canBeConnected(const Identifier& objId) const;

    void setOldPos(const Point<int>& p) { oldPos = p; }
    const Point<int>& getOldPos() const { return oldPos; }

private:
    void showLinkPopupMenu();

    void changeListenerCallback (ChangeBroadcaster*);

    bool canBeConnectedToLinks() const;
    bool canBeConnectedToWaveguides() const;

    IdLabel* createIdLabel() override;

    DrawableComposite* icon;
    DropShadowEffect shadow;

    Point<int> actualPos;

    Point<int> oldPos;

    bool dragging, mouseDownSelectStatus;
    
    ChangeBroadcaster selfChangeListenerList;

    Array<LinkComponent*> connectedLinks;

    WeakReference<ObjectComponent>::Master masterReference;
    friend class WeakReference<ObjectComponent>;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ObjectComponent);
};
}


#endif  // __OBJECTCOMPONENT_H_4AC10D4B__
