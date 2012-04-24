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
	return new Mass(MassType);
}

Ground* ObjectFactory::createNewGround()
{
	return new Ground(GroundType);
}
Port* ObjectFactory::createNewResonator()
{
	return new Resonator(ResonatorType);
}
Port* ObjectFactory::createNewPort()
{
	return new Port(PortType);
}
Link* ObjectFactory::createNewLink()
{
	return new Link(LinkType);
}
Pluck* ObjectFactory::createNewPluck()
{
	return new Pluck(PluckType);
}
Touch* ObjectFactory::createNewTouch()
{
	return new Touch(TouchType);
}
LabelObject* ObjectFactory::createNewLabelObject()
{
	return new LabelObject(LabelType);
}
AudioObject* ObjectFactory::createNewAudioObject()
{
	return new AudioObject(AudioObjectType);
}
WaveguideObject* ObjectFactory::createNewWaveguideObject()
{
	return new WaveguideObject(WaveguideType);
}
TerminationObject* ObjectFactory::createNewTerminationObject()
{
	return new TerminationObject(TerminationType);
}
JunctionObject* ObjectFactory::createNewJunctionObject()
{
	return new JunctionObject(JunctionType);
}
