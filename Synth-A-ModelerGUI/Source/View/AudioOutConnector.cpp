/*
  ==============================================================================

    AudioOutConnector.cpp
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

#include "../Application/CommonHeaders.h"
#include "../View/ObjectComponent.h"
#include "../Controller/ObjController.h"

#include "AudioOutConnector.h"

AudioOutConnector::AudioOutConnector(ObjController& owner_, 
                                     ObjectComponent* objComp_,
                                     ObjectComponent* audioOutComp_)
    :
    owner(owner_),
    selected(false),
    mouseDownSelectStatus(false),
    objComp(objComp_),
    audioOutComp(audioOutComp_)
{
//    audioOutComp
//    ValueTree auSources = audioOutComp->getData().getChildWithName(Ids::sources);
//    ValueTree source(Ids::audiosource);
//    source.setProperty(Ids::value, objComp->getData()[Ids::identifier], nullptr);
//    source.setProperty(Ids::gain, "1.0", nullptr);
//    auSources.addChild(source, -1, nullptr);
    owner.getSelectedAudioConnections().addChangeListener(this);
}

AudioOutConnector::~AudioOutConnector()
{
    owner.getSelectedAudioConnections().removeChangeListener(this);
}

void AudioOutConnector::resized()
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
    linePath.startNewSubPath(x1, y1);
    linePath.lineTo(x2, y2);

    PathStrokeType wideStroke(8.0f);
    wideStroke.createStrokedPath(hitPath, linePath);

    float dashLengths[] = {5, 5};
    PathStrokeType stroke(2.5f);
    stroke.createDashedStroke(linePath, linePath, dashLengths, 2);
//    stroke.createStrokedPath(linePath, linePath);
    
    const float arrowW = 7.0f;
    const float arrowL = 5.0f;

    Path arrow;
    arrow.addTriangle(-arrowL, arrowW,
                      -arrowL, -arrowW,
                      arrowL, 0.0f);

    arrow.applyTransform(AffineTransform::identity
                         .rotated(float_Pi * 0.5f - (float) atan2(x2 - x1, y2 - y1))
                         .translated((x1 + x2) * 0.5f,
                                     (y1 + y2) * 0.5f));
//    arrow.applyTransform(AffineTransform::translation((x2 - x1) * 0.3f, (y2 - y1) * 0.3f));

    linePath.addPath(arrow);
    linePath.setUsingNonZeroWinding(true);

}

void AudioOutConnector::paint(Graphics& g)
{
    if(selected)
    {
        g.setColour(Colours::darkred);
    }
    else
    {
        g.setColour(Colours::darkgrey);
    }
    
    g.fillPath(linePath);
}

void AudioOutConnector::update()
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

void AudioOutConnector::resizeToFit()
{
    float x1, y1, x2, y2;
    getPoints(x1, y1, x2, y2);

    const Rectangle<int> newBounds((int) jmin(x1, x2) - 4,
                                   (int) jmin(y1, y2) - 4,
                                   (int) fabsf(x1 - x2) + 8,
                                   (int) fabsf(y1 - y2) + 8);

    if (newBounds != getBounds())
        setBounds(newBounds);
    else
        resized();

    repaint();
    
}

bool AudioOutConnector::hitTest(int x, int y)
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

void AudioOutConnector::getPoints(float& x1, float& y1, float& x2, float& y2) const
{
    x1 = lastInputX;
    y1 = lastInputY;
    x2 = lastOutputX;
    y2 = lastOutputY;

    if (objComp != nullptr)
    {
        Point<int> startPos = objComp->getActualPos();
        x1 = startPos.x;
        y1 = startPos.y;
    }

    if (audioOutComp != nullptr)
    {
        Point<int> endPos = audioOutComp->getActualPos();
        x2 = endPos.x;
        y2 = endPos.y;
    }
}

void AudioOutConnector::changeListenerCallback (ChangeBroadcaster*)
{
    const bool nowSelected = owner.getSelectedAudioConnections().isSelected (this);

    if (selected != nowSelected)
    {
        selected = nowSelected;
        repaint();
    }
    update();
}

void AudioOutConnector::mouseDown(const MouseEvent& e)
{
    mouseDownSelectStatus = owner.getSelectedAudioConnections().addToSelectionOnMouseDown (this, e.mods);
}

void AudioOutConnector::mouseDrag(const MouseEvent& e)
{
    
}

void AudioOutConnector::mouseUp(const MouseEvent& e)
{
    owner.getSelectedAudioConnections().addToSelectionOnMouseUp (this, e.mods, false,
                                                                 mouseDownSelectStatus);
    update();
}