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
#include "ObjectsHolder.h"

#include "LinkComponent.h"

LinkComponent::LinkComponent(ObjController& owner_, ValueTree linkTree)
: BaseObjectComponent(owner_, linkTree),
    lastInputX (0),
    lastInputY (0),
    lastOutputX (0),
    lastOutputY (0),
    segmented(false),
    selected(false)
{
    startComp = owner.getObjectForId(data.getProperty(Ids::startVertex).toString());
    endComp = owner.getObjectForId(data.getProperty(Ids::endVertex).toString());
    
    startComp->addLinkToObject(this);
    endComp->addLinkToObject(this);

    // TODO Better solution for this.
//    if(startComp->getData().getType() == Ids::audioout)
//    {
//        String aoSourceName = endComp->getData().getProperty(Ids::identifier).toString();
//        String aoSource = startComp->getData().getProperty(Ids::sources);
//        if(aoSource != String::empty)
//            aoSource.append("+", 1);
//        aoSource.append("1.0*", 5);
//        aoSource.append(aoSourceName, 256);
//        startComp->getData().setProperty(Ids::sources, aoSource, nullptr);
//    }
//    else if(endComp->getData().getType() == Ids::audioout)
//    {
//        String aoSourceName = startComp->getData().getProperty(Ids::identifier).toString();
//        String aoSource = endComp->getData().getProperty(Ids::sources);
//        if(aoSource != String::empty)
//            aoSource.append("+", 1);
//        aoSource.append("1.0*", 5);
//        aoSource.append(aoSourceName, 256);
//        endComp->getData().setProperty(Ids::sources, aoSource, nullptr);
//    }

    owner.getSelectedLinks().addChangeListener(this);

    update();
}

LinkComponent::~LinkComponent()
{
    startComp->removeLinkFromObject(this);
    endComp->removeLinkFromObject(this);
    
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
}

void LinkComponent::resizeToFit()
{
    float x1, y1, x2, y2;
    getPoints(x1, y1, x2, y2);

    const Rectangle<int> newBounds((int) jmin(x1, x2) - 25,
                                   (int) jmin(y1, y2) - 25,
                                   (int) fabsf(x1 - x2) + 50,
                                   (int) fabsf(y1 - y2) + 50);

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
    
    drawPath(x1, y1, x2, y2);
    
}

void LinkComponent::paint(Graphics& g)
{
    if(selected)
    {
        g.setColour(Colours::red);
    }
    else
    {
        g.setColour(Colours::green);

    }
    g.fillPath (linePath);
}

void LinkComponent::mouseDown(const MouseEvent& e)
{
    mouseDownSelectStatus = owner.getSelectedLinks().addToSelectionOnMouseDown (this, e.mods);
}

void LinkComponent::mouseDrag(const MouseEvent& e)
{
    
}

void LinkComponent::mouseUp(const MouseEvent& e)
{
    if (e.mouseWasClicked() && e.getNumberOfClicks() == 2)
	{
		getObjectsHolder()->editObjectProperties(this);
	}
    else if (e.mods.isPopupMenu())
    {
        showContextMenu();
        return; // this may be deleted now..
    }
    owner.getSelectedLinks().addToSelectionOnMouseUp (this, e.mods, false, mouseDownSelectStatus);
    update();
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
        Point<int> startPos = startComp->getActualPos();
        x1 = startPos.x;
        y1 = startPos.y;
    }

    if (endComp != nullptr)
    {
        Point<int> endPos = endComp->getActualPos();
        x2 = endPos.x;
        y2 = endPos.y;
    }
}

void LinkComponent::changeListenerCallback (ChangeBroadcaster*)
{
    const bool nowSelected = owner.getSelectedLinks().isSelected (this);

    if (selected != nowSelected)
    {
        selected = nowSelected;
        repaint();
    }
    update();
}

bool LinkComponent::sameStartEnd(ValueTree linkTree)
{
    return linkTree.getProperty(Ids::startVertex) == data.getProperty(Ids::startVertex) 
        && linkTree.getProperty(Ids::endVertex) == data.getProperty(Ids::endVertex);
}

void LinkComponent::drawPath(float x1, float y1, float x2, float y2)
{
    int iconWidth = 25;
    int iconHeight = 25;
    
    if(data.getType() == Ids::link)
    {
        linePath.clear();
        linePath.startNewSubPath(x1, y1);
        linePath.lineTo(x2, y2);

        PathStrokeType wideStroke(8.0f);
        wideStroke.createStrokedPath(hitPath, linePath);

        PathStrokeType stroke(2.5f);
        stroke.createStrokedPath(linePath, linePath);

        Path iconPath;
        iconPath = ResourceLoader::getInstance()->getPathForLinkId(Ids::link,
                                                                   0,
                                                                   0,
                                                                   iconWidth,
                                                                   iconHeight);
        iconPath.applyTransform(AffineTransform::identity
                                .rotated(float_Pi * 0.5f - (float) atan2(x2 - x1, y2 - y1))
                                .translated((x1 + x2) * 0.5f,
                                            (y1 + y2) * 0.5f));

        PathStrokeType stroke2(0.4f);
        stroke2.createStrokedPath(iconPath, iconPath);

        linePath.addPath(iconPath);
    }
    else if(data.getType() == Ids::touch)
    {
        //This is the little bit of the path which covers the icon
        float c = (iconWidth / 2.0f) / sqrt((x2 - x1)*(x2 - x1) +  (y2 - y1)*(y2 - y1));

        linePath.clear();
        linePath.startNewSubPath(x1, y1);
        linePath.lineTo(x1 + (x2 - x1) * (0.5f-c), 
                        y1 + (y2 - y1) * (0.5f-c));
        
        linePath.startNewSubPath(x1 + (x2 - x1)*(0.5f+c), 
                                 y1 + (y2 - y1)*(0.5f+c));
//        linePath.applyTransform(AffineTransform::translation(10, 10));
        linePath.lineTo(x2, y2);
        
        PathStrokeType wideStroke(8.0f);
        wideStroke.createStrokedPath(hitPath, linePath);

        PathStrokeType stroke(2.5f);
        stroke.createStrokedPath(linePath, linePath);

        Path iconPath;
        iconPath = ResourceLoader::getInstance()->getPathForLinkId(Ids::touch,
                                                                   0,
                                                                   0,
                                                                   iconWidth,
                                                                   iconHeight);
        iconPath.applyTransform(AffineTransform::identity
                                .rotated(float_Pi * 0.5f - (float) atan2(x2 - x1, y2 - y1))
                                .translated((x1 + x2) * 0.5f,
                                            (y1 + y2) * 0.5f));

        PathStrokeType stroke2(1.4f);
        stroke2.createStrokedPath(iconPath, iconPath);

        linePath.addPath(iconPath);
    }
    else if(data.getType() == Ids::pluck)
    {
        //This is the little bit of the path which covers the icon
        float c = (iconWidth / 2.0f) / sqrt((x2 - x1)*(x2 - x1) +  (y2 - y1)*(y2 - y1));
        
        linePath.clear();
        linePath.startNewSubPath(x1, y1);
        linePath.lineTo(x1 + (x2 - x1) * (0.5f-c), 
                        y1 + (y2 - y1) * (0.5f-c));
        
        linePath.startNewSubPath((x1 + (x2 - x1) *(0.5f+c)), 
                                 (y1 + (y2 - y1) *(0.5f+c)));
//        linePath.applyTransform(AffineTransform::translation(10,10));
        linePath.lineTo(x2, y2);
        
        PathStrokeType wideStroke(8.0f);
        wideStroke.createStrokedPath(hitPath, linePath);

        PathStrokeType stroke(2.5f);
        stroke.createStrokedPath(linePath, linePath);

        Path iconPath;
        iconPath = ResourceLoader::getInstance()->getPathForLinkId(Ids::pluck,
                                                                   0,
                                                                   0,
                                                                   iconWidth,
                                                                   iconHeight);
        iconPath.applyTransform(AffineTransform::identity
                                .rotated(float_Pi * 0.5f - (float) atan2(x2 - x1, y2 - y1))
                                .translated((x1 + x2) * 0.5f,
                                            (y1 + y2) * 0.5f));

        PathStrokeType stroke2(0.8f);
        stroke2.createStrokedPath(iconPath, iconPath);

        linePath.addPath(iconPath);
    }
    const float arrowW = 5.0f;
    const float arrowL = 4.0f;

    Path arrow;
    arrow.addTriangle(-arrowL, arrowW,
                      -arrowL, -arrowW,
                      arrowL, 0.0f);

    arrow.applyTransform(AffineTransform::identity
                         .rotated(float_Pi * 0.5f - (float) atan2(x2 - x1, y2 - y1))
                         .translated((x1 + x2) * 0.5f,
                                     (y1 + y2) * 0.5f));
    arrow.applyTransform(AffineTransform::translation((x2 - x1) * 0.3f, (y2 - y1) * 0.3f));

    linePath.addPath(arrow);
    linePath.setUsingNonZeroWinding(true);

}

void LinkComponent::reverseDirection()
{
    String tempStart = data.getProperty(Ids::startVertex);
    String tempEnd = data.getProperty(Ids::endVertex);
    
    data.setProperty(Ids::startVertex, tempEnd, nullptr);
    data.setProperty(Ids::endVertex, tempStart, nullptr);
    
    startComp = owner.getObjectForId(tempEnd);
    endComp = owner.getObjectForId(tempStart);

    update();
}
