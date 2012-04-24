/*
  ==============================================================================

    WaveguideObject.h
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

#ifndef __WAVEGUIDEOBJECT_H_D78ED265__
#define __WAVEGUIDEOBJECT_H_D78ED265__

#include "BaseObject.h"

class WaveguideObject : public BaseObject
{
public:
	WaveguideObject(ObjectType objType_=WaveguideType);
	virtual ~WaveguideObject();

	void addParameter(float value);
	const Array<float> getParameters() const { return parameters; }
	void setObjectLeft(const String& objLeft_);
	const String& getObjectLeft() const { return objLeft; }
	void setObjectRight(const String& objRight_);
	const String& getObjectRight() const { return objRight; }
	void addLabel(const String& label);
	const Array<String>& getLabels() const { return labels; }


private:
	Array<float> parameters;
	String objLeft;
	String objRight;
	Array<String> labels;

};

class TerminationObject : public BaseObject
{
public:
	TerminationObject(ObjectType objType_=TerminationType);
	virtual ~TerminationObject();

	void addParameter(const String& value);
	const Array<String>& getParameters() const { return parameters; }
	void addLabel(const String& label);
	const Array<String>& getLabels() const { return labels; }

private:
	Array<String> parameters;
	Array<String> labels;
};
class JunctionObject : public BaseObject
{
public:
	JunctionObject(ObjectType objType_=JunctionType);
	virtual ~JunctionObject();

	void addParameter(float value);
	float getParameters() const { return parameters; }
	void addLabel(const String& label);
	const Array<String>& getLabels() const { return labels; }

private:
	float parameters;
	Array<String> labels;
};


#endif  // __WAVEGUIDEOBJECT_H_D78ED265__
