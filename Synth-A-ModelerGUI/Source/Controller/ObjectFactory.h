/*
  ==============================================================================

    ObjectFactory.h
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

#ifndef __ONJECTFACTORY_H_E3D01814__
#define __ONJECTFACTORY_H_E3D01814__

#include "../Models/Types.h"

class ObjectFactory
{
public:

	static Mass* createNewMass();
	static Ground* createNewGround();
	static Resonator* createNewResonator();
	static Port* createNewPort();
	static Link* createNewLink();
	static Pluck* createNewPluck();
	static Touch* createNewTouch();
	static LabelObject* createNewLabelObject();
	static AudioObject* createNewAudioObject();

private:
	ObjectFactory(){}
	~ObjectFactory(){}
};


#endif  // __ONJECTFACTORY_H_E3D01814__
