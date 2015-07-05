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


#include "JuceHeader.h"
#include "SelectableObject.h"
#include "BaseConnector.h"

namespace synthamodeler
{
class ObjController;
class BaseObjectComponent;
class ObjectComponent;
class LinkComponent;

class AudioOutConnector : public BaseConnector
{
public:
    AudioOutConnector(ObjController& owner_,
                      BaseObjectComponent* objComp_,
                      ObjectComponent* audioOutComp_);
    virtual ~AudioOutConnector();

    virtual void getPoints(float& x1, float& y1, float& x2, float& y2) const override;

    virtual void changeListenerCallback (ChangeBroadcaster* source) override;

    BaseObjectComponent* getSourceObject()             { return sourceComp; }
    const BaseObjectComponent* getSourceObject() const { return sourceComp; }
    ObjectComponent* getAudioObject()                  { return audioOutComp; }
    const ObjectComponent* getAudioObject() const      { return audioOutComp; }
private:
    WeakReference<BaseObjectComponent> sourceComp;
    WeakReference<ObjectComponent> audioOutComp;

    WeakReference<ObjectComponent> objectComp;
    WeakReference<LinkComponent> linkComp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioOutConnector);
};
}


#endif  // __AUDIOOUTCONNECTOR_H_D0DAD273__
