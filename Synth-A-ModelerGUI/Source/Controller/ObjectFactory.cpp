/*
  ==============================================================================

    ObjectFactory.cpp
    Created: 17 Apr 2012 11:55:19pm
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

#include "ObjectFactory.h"

Mass* ObjectFactory::createNewMass()
{
	Mass* mass = new Mass(MassType);

	return mass;
}
Ground* ObjectFactory::createNewGround()
{
	Ground* ground = new Ground(GroundType);
	return ground;
}
Port* ObjectFactory::createNewResonator()
{
	Resonator* resonator = new Resonator(ResonatorType);
	return resonator;
}
Port* ObjectFactory::createNewPort()
{
	Port* port = new Port(PortType);
	return port;
}
Link* ObjectFactory::createNewLink()
{
	Link* link = new Link(LinkType);
	return link;
}
Pluck* ObjectFactory::createNewPluck()
{
	Pluck* pluck = new Pluck(PluckType);
	return pluck;
}

Touch* ObjectFactory::createNewTouch()
{
	Touch* contact = new Touch(TouchType);
	return contact;
}

LabelObject* ObjectFactory::createNewLabelObject()
{
	LabelObject* label = new LabelObject(LabelType);
	return label;
}

AudioObject* ObjectFactory::createNewAudioObject()
{
	AudioObject* audioObj = new AudioObject(AudioObjectType);
	return audioObj;
}

