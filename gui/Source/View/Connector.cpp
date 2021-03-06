/*
  ==============================================================================

    Connector.cpp
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

#include "Connector.h"

#include "Application/CommonHeaders.h"

#include "Controller/ObjController.h"

#include "Graph/Node.h"

#include "BaseObjectComponent.h"
#include "ObjectComponent.h"
#include "LinkComponent.h"
#include "ObjectsHolder.h"

#include "Utilities/ObjectsHelper.h"


using namespace synthamodeler;

Connector::Connector(ObjController& owner_,
                     BaseObjectComponent* objComp_,
                     ObjectComponent* targetComp_)
    :
    owner(owner_),
    mouseDownSelectStatus(false),
    sourceComp(objComp_),
    targetComp(targetComp_)
{
    segmented = StoredSettings::getInstance()->getIsSegmentedConnectors();
    owner.getSelectedObjects().addChangeListener(this);

    targetComp->addConnector(this);

    if ((objectComp = dynamic_cast<ObjectComponent*>(sourceComp.get())))
    {
        objectComp->addConnector(this);
    }
    else if ((linkComp = dynamic_cast<LinkComponent*>(sourceComp.get())))
    {
        linkComp->addConnector(this);
    }

    setComponentID("conn_"
        + sourceComp->getData()[Ids::identifier].toString()
        + targetComp->getData()[Ids::identifier].toString());
}

Connector::~Connector()
{
    owner.getSelectedObjects().removeChangeListener(this);

    targetComp->removeChangeListener(this);

    if (objectComp)
    {
        objectComp->removeChangeListener(this);
    }
    else if (linkComp)
    {
        linkComp->removeChangeListener(this);
    }
}

void Connector::resized()
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

    float rotatVal;
    linePath.clear();
    if(segmented)
    {
        linePath.clear();
        linePath.startNewSubPath(x1, y1);
        linePath.lineTo(x1, (y2 - y1) / 2 + y1);
        linePath.lineTo(x2, (y2 - y1) / 2 + y1);
        linePath.lineTo(x2, y2);

        rotatVal = float_Pi * (x2 - x1 < 0 ? -1.0f: 2.0f);
    }
    else
    {
        linePath.startNewSubPath(x1, y1);
        linePath.lineTo(x2, y2);
        rotatVal = float_Pi * 0.5f - (float) atan2(x2 - x1, y2 - y1);
    }
    PathStrokeType wideStroke(8.0f);
    wideStroke.createStrokedPath(hitPath, linePath);

    float dashLengths[] = {5, 5};
    PathStrokeType stroke(1.5f);
    stroke.createDashedStroke(linePath, linePath, dashLengths, 2);
    //    stroke.createStrokedPath(linePath, linePath);
    const float arrowW = 7.0f;
    const float arrowL = 5.0f;

    Path arrow;
    arrow.addTriangle(-arrowL, arrowW,
                      -arrowL, -arrowW,
                      arrowL, 0.0f);

    arrow.applyTransform(AffineTransform::identity
                         .rotated(rotatVal)
                         .translated((x1 + x2) * 0.5f,
                                     (y1 + y2) * 0.5f));
    //    arrow.applyTransform(AffineTransform::translation((x2 - x1) * 0.3f, (y2 - y1) * 0.3f));

    linePath.addPath(arrow);

    linePath.setUsingNonZeroWinding(true);

}

void Connector::paint(Graphics& g)
{
    if(! isVisible())
        return;

    if(selected)
    {
        g.setColour(Colours::red);
    }
    else
    {
        g.setColour(Colours::grey);
    }
    
    g.fillPath(linePath);
}

void Connector::update()
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
}

void Connector::resizeToFit()
{
    float x1, y1, x2, y2;
    getPoints(x1, y1, x2, y2);

    const Rectangle<int> newBounds((int) jmin(x1, x2) - 7,
                                   (int) jmin(y1, y2) - 7,
                                   (int) fabsf(x1 - x2) + 14,
                                   (int) fabsf(y1 - y2) + 14);

    if (newBounds != getBounds())
        setBounds(newBounds);
    else
        resized();

    repaint();
    
}

bool Connector::hitTest(int x, int y)
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

void Connector::getPoints(float& x1, float& y1, float& x2, float& y2) const
{
    x1 = lastInputX;
    y1 = lastInputY;
    x2 = lastOutputX;
    y2 = lastOutputY;

    if (!sourceComp.wasObjectDeleted())
    {
        Point<int> startPos;
        if (objectComp)
            startPos = objectComp->getPinPos();
        else if (linkComp)
            startPos = linkComp->getPinPos();
        x1 = startPos.x;
        y1 = startPos.y;
    }

    if (!targetComp.wasObjectDeleted())
    {
        Point<int> endPos = targetComp->getPinPos();
        x2 = endPos.x;
        y2 = endPos.y;
    }
}

void Connector::changeListenerCallback (ChangeBroadcaster* const source)
{
    if (source == objectComp || source == linkComp || source == targetComp)
    {
        update();
    }
    else if (source == &owner.getSelectedObjects())
    {
        const bool nowSelected = owner.getSelectedObjects().isSelected (this);

        if (selected != nowSelected)
        {
            selected = nowSelected;
            repaint();
        }
    }
}

void Connector::mouseDown(const MouseEvent& e)
{
    owner.setAsFromtmostLink(*this);

    mouseDownSelectStatus = owner.getSelectedObjects().addToSelectionOnMouseDown (this, e.mods);
}

void Connector::mouseDrag(const MouseEvent& /*e*/)
{
    
}

void Connector::mouseUp(const MouseEvent& e)
{
    if (e.mouseWasClicked() && e.getNumberOfClicks() == 2)
	{
        propertiesWindow->makeVisible(true);
	}

    owner.getSelectedObjects().addToSelectionOnMouseUp (this, e.mods, false,
                                                        mouseDownSelectStatus);
    update();
}

Rectangle<int> Connector::getIntersectioBounds()
{
    const Rectangle<int> intersectionBounds((int) jmin(lastInputX, lastOutputX),
                                            (int) jmin(lastInputY, lastOutputY),
                                            (int) fabsf(lastInputX - lastOutputX),
                                            (int) fabsf(lastInputY - lastOutputY));
    return intersectionBounds;
}
