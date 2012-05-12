/*
  ==============================================================================

    ObjectsHolder.h
    Created: 11 Apr 2012 5:10:20pm
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

#ifndef __OBJCOMP_H_F3604232__
#define __OBJCOMP_H_F3604232__

class BaseObjectComponent;
class MDLFile;
class ObjController;

class ObjectsHolder : public Component,
						public ChangeListener
{
public:
	ObjectsHolder(ObjController& objController_);
	~ObjectsHolder();

	void paint(Graphics& g);
//	void resized();

	void setMDLFile(MDLFile* newMDLFile);

	void changeListenerCallback(ChangeBroadcaster*);
private:
	OwnedArray<BaseObjectComponent> objects;
	ObjController& objController;
	MDLFile* mdlFile;
};


#endif  // __OBJCOMP_H_F3604232__
