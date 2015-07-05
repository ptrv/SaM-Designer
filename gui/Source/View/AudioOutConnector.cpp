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

#include "AudioOutConnector.h"

#include "Application/CommonHeaders.h"

#include "Controller/ObjController.h"

#include "Graph/Node.h"

#include "BaseObjectComponent.h"
#include "ObjectComponent.h"
#include "LinkComponent.h"
#include "ObjectsHolder.h"

#include "Utilities/ObjectsHelper.h"


using namespace synthamodeler;

AudioOutConnector::AudioOutConnector(ObjController& owner_,
                                     BaseObjectComponent* objComp_,
                                     ObjectComponent* audioOutComp_)
    : BaseConnector(owner_)
    , sourceComp(objComp_)
    , audioOutComp(audioOutComp_)
{
    audioOutComp->addChangeListener(this);

    if (ObjectComponent* oc = dynamic_cast<ObjectComponent*>(sourceComp.get()))
    {
        objectComp = oc;
        objectComp->addChangeListener(this);
    }
    else if (LinkComponent* lc = dynamic_cast<LinkComponent*>(sourceComp.get()))
    {
        linkComp = lc;
        linkComp->addChangeListener(this);
    }

    setComponentID("aoc_"
        + sourceComp->getData()[Ids::identifier].toString()
        + audioOutComp->getData()[Ids::identifier].toString());
}

AudioOutConnector::~AudioOutConnector()
{
    audioOutComp->removeChangeListener(this);

    if (objectComp)
    {
        objectComp->removeChangeListener(this);
    }
    else if (linkComp)
    {
        linkComp->removeChangeListener(this);
    }
}

void AudioOutConnector::getPoints(float& x1, float& y1, float& x2, float& y2) const
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

    if (!audioOutComp.wasObjectDeleted())
    {
        Point<int> endPos = audioOutComp->getPinPos();
        x2 = endPos.x;
        y2 = endPos.y;
    }
}

void AudioOutConnector::changeListenerCallback (ChangeBroadcaster* const source)
{
    if (source == objectComp || source == linkComp || source == audioOutComp)
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
