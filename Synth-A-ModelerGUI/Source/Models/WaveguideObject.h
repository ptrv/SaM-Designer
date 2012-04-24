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

private:
};

class TerminationObject : public BaseObject
{
public:
	TerminationObject(ObjectType objType_=TerminationType);
	virtual ~TerminationObject();

private:
};
class JunctionObject : public BaseObject
{
public:
	JunctionObject(ObjectType objType_=JunctionType);
	virtual ~JunctionObject();

private:
};


#endif  // __WAVEGUIDEOBJECT_H_D78ED265__
