/*
  ==============================================================================

    LinkObject.h
    Created: 16 Apr 2012 1:23:34am
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

#ifndef __LINKOBJECT_H_9C798D41__
#define __LINKOBJECT_H_9C798D41__

#include "BaseObject.h"

class LinkObject : public BaseObject
{
public:
	LinkObject(ObjectType objType_= LinkType);
	virtual ~LinkObject();

	void addParameter(float value);
	Array<float>& getParameters() { return parameters; }
	void addLabel(const String& label);
	Array<String>& getLabels() { return labels; }
	void setStartVertex(const String& startVert);
	void setEndVertex(const String& endVert);
	String& getStartVertex() { return startingVertex; }
	String& getEndVertex() { return endVertex; }
private:
	Array<float> parameters;
	Array<String> labels;
	String startingVertex;
	String endVertex;

};

typedef LinkObject Link;
typedef LinkObject Touch;
typedef LinkObject Pluck;

#endif  // __LINKOBJECT_H_9C798D41__
