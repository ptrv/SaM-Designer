/*
  ==============================================================================

    WaveguideObject.cpp
    Created: 24 Apr 2012 7:48:52pm
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

#include "../../JuceLibraryCode/JuceHeader.h"
#include "WaveguideObject.h"

WaveguideObject::WaveguideObject(ObjectType objType_)
: BaseObject(objType_)
{

}

WaveguideObject::~WaveguideObject()
{

}

void WaveguideObject::addParameter(float value)
{
	parameters.add(value);
}

void WaveguideObject::addLabel(const String& label)
{
	labels.add(label);
}

void WaveguideObject::setObjectLeft(const String& objLeft_)
{
	objLeft = objLeft_;
}

void WaveguideObject::setObjectRight(const String& objRight_)
{
	objRight = objRight_;
}

TerminationObject::TerminationObject(ObjectType objType_)
: BaseObject(objType_)
{

}

TerminationObject::~TerminationObject()
{

}

void TerminationObject::addParameter(const String& value)
{
	parameters.add(value);
}

void TerminationObject::addLabel(const String& label)
{
	labels.add(label);
}


JunctionObject::JunctionObject(ObjectType objType_)
: BaseObject(objType_)
{

}

JunctionObject::~JunctionObject()
{

}

void JunctionObject::addParameter(float value)
{
	parameters = value;
}

void JunctionObject::addLabel(const String& label)
{
	labels.add(label);
}

