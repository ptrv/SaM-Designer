/*
  ==============================================================================

    LinkComponent.cpp
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

#include "../Application/CommonHeaders.h"
#include "../Controller/ObjController.h"
#include "ObjectComponent.h"

#include "LinkComponent.h"

LinkComponent::LinkComponent(ObjController& owner_, ValueTree linkTree)
: owner(owner_),
    linkId(linkTree.getType()),
    data(linkTree),
    segmented(false),
    selected(false),
    lastInputX (0),
    lastInputY (0),
    lastOutputX (0),
    lastOutputY (0)
{
    startComp = owner.getObjectForId(data.getProperty(Ids::startVertex).toString());
    endComp = owner.getObjectForId(data.getProperty(Ids::endVertex).toString());
    
    DBG(data.toXmlString());
    
    icon = dynamic_cast<DrawableComposite*> (ResourceLoader::getInstance()->getDrawableForId(linkId));
    
    owner.getSelectedLinks().addChangeListener(this);
    setAlwaysOnTop (true);
    
    DBG(startComp->getPosition().x);
    DBG(startComp->getPosition().y);
    
    update();
//    toFront(false);
    DBG(getBounds().toString());
//    setVisible(true);
}

LinkComponent::~LinkComponent()
{
    owner.getSelectedLinks().removeChangeListener(this);
}
void LinkComponent::update()
{
    float x1, y1, x2, y2;
    getPoints(x1, y1, x2, y2);

    if (lastInputX != x1
        || lastInputY != y1
        || lastOutputX != x2
        || lastOutputY != y2)
    {
        resizeToFit();
    }
    DBG(getBounds().toString());
}

void LinkComponent::resizeToFit()
{
    float x1, y1, x2, y2;
    getPoints(x1, y1, x2, y2);

    const Rectangle<int> newBounds((int) jmin(x1, x2),
                                   (int) jmin(y1, y2),
                                   (int) fabsf(x1 - x2),
                                   (int) fabsf(y1 - y2));

    if (newBounds != getBounds())
        setBounds(newBounds);
    else
        resized();

    repaint();
}
void LinkComponent::resized()
{
    float x1, y1, x2, y2;
    getPoints(x1, y1, x2, y2);

    lastInputX = x1;
    lastInputY = y1;
    lastOutputX = x2;
    lastOutputY = y2;

    x1 -= getX();
    y1 -= getY();
    x2 -= getX();
    y2 -= getY();
    linePath.clear();
//    linePath.
    linePath.startNewSubPath(x1, y1);
//    linePath.cubicTo(x1, y1 + (y2 - y1) * 0.33f,
//                     x2, y1 + (y2 - y1) * 0.66f,
//                     x2, y2);

    linePath.lineTo(x2, y2);
//    PathStrokeType wideStroke(8.0f);
//    wideStroke.createStrokedPath(hitPath, linePath);

    PathStrokeType stroke(2.5f);
    stroke.createStrokedPath(linePath, linePath);

//    linePath.setUsingNonZeroWinding (true);
    
    linePath.addPath(new DrawablePath());
    iconPos = linePath.getPointAlongPath(linePath.getLength()/2);
    DBG(iconPos.toString());
}

void LinkComponent::paint(Graphics& g)
{
    g.setColour(Colours::green);
    
//    g.strokePath(linePath, PathStrokeType (5.0f));
    g.fillPath (linePath);
    
    Rectangle<float> rect;
    rect.setPosition(iconPos);
    rect.setSize(20.0f, 20.0f);
    icon->drawWithin(g, rect, RectanglePlacement::centred, 1.0f);
    
//    g.fillRect(20, 20, 100, 100);
    
}

void LinkComponent::mouseDown(const MouseEvent& e)
{
    
}

void LinkComponent::mouseDrag(const MouseEvent& e)
{
    
}

void LinkComponent::mouseUp(const MouseEvent& e)
{
    
}

bool LinkComponent::hitTest(int x, int y)
{
    if (hitPath.contains((float) x, (float) y))
    {
        double distanceFromStart, distanceFromEnd;
        getDistancesFromEnds(x, y, distanceFromStart, distanceFromEnd);

        // avoid clicking the connector when over a pin
        return distanceFromStart > 7.0 && distanceFromEnd > 7.0;
    }

    return false;
}

void LinkComponent::setSelected(bool shouldBeSelected)
{
    selected = shouldBeSelected;
    if(selected)
    {
        owner.getSelectedLinks().addToSelection(this);
    }
    else
    {
        owner.getSelectedLinks().deselect(this);
    }
}

void LinkComponent::getPoints(float& x1, float& y1, float& x2, float& y2) const
{
    x1 = lastInputX;
    y1 = lastInputY;
    x2 = lastOutputX;
    y2 = lastOutputY;

    if (startComp != nullptr)
    {
        Point<int> startPos = startComp->getPosition();
        x1 = startPos.x;
        y1 = startPos.y;
    }

    if (endComp != nullptr)
    {
        Point<int> endPos = endComp->getPosition();
        x2 = endPos.x;
        y2 = endPos.y;
    }
}

void LinkComponent::changeListenerCallback (ChangeBroadcaster*)
{
    update();
}