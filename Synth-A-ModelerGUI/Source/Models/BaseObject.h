/*
  ==============================================================================

    BaseObject.h
    Created: 16 Apr 2012 1:22:04am
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

#ifndef __BASEOBJECT_H_F4381D18__
#define __BASEOBJECT_H_F4381D18__

#include "../../JuceLibraryCode/JuceHeader.h"

enum ObjectType
{
	MassType,
	GroundType,
	ResonatorType,
	PortType,
	LinkType,
	TouchType,
	PluckType,
	WaveguideType,
	TerminationType,
	JunctionType,
	LabelType,
	AudioObjectType
};

class BaseObject
{
public:
	BaseObject(ObjectType objType_)
	: objType(objType_), name(String::empty)
	{
		switch (objType) {
			case MassType:
				typeString = "mass";
				break;
			case GroundType:
				typeString = "ground";
				break;
			case ResonatorType:
				typeString = "resonator";
				break;
			case PortType:
				typeString = "port";
				break;
			case LinkType:
				typeString = "link";
				break;
			case TouchType:
				typeString = "touch";
				break;
			case PluckType:
				typeString = "pluck";
				break;
			case LabelType:
				typeString = "label";
				break;
			case WaveguideType:
				typeString = "waveguide";
				break;
			case TerminationType:
				typeString = "termination";
				break;
			case JunctionType:
				typeString = "junction";
				break;
			case AudioObjectType:
				typeString = "audioout";
				break;

			default:
				break;
		}
	}
	virtual ~BaseObject(){}

	const String& getAttributeNames();

	const ObjectType& getType() const { return objType; }
	const String& getTypeString() const { return typeString; }

	const String& getName() const { return name; }
	void setName(const String& name_) { name = name_; }

protected:
	ObjectType objType;
	String typeString;
	String name;

private:
};


#endif  // __BASEOBJECT_H_F4381D18__
