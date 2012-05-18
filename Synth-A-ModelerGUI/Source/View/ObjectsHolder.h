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

class ObjectComponent;
class MDLFile;
class ObjController;

class ObjectsHolder : public Component,
						public ChangeListener
{
public:
	ObjectsHolder(ObjController& objController_);
	~ObjectsHolder();

	void paint(Graphics& g);
    void resized();
    void changeListenerCallback (ChangeBroadcaster*);
    void updateComponents();

    void mouseDown (const MouseEvent& e);
	void mouseDrag (const MouseEvent& e);
	void mouseUp (const MouseEvent& e);

	bool dispatchMenuItemClick(const ApplicationCommandTarget::InvocationInfo& info);

	void setMDLFile(MDLFile* newMDLFile);

	void moveObjectsData(Point<int> offset);
	void moveObjectComponents(Point<int> offset);

	void editObjectProperties(ObjectComponent* oc);

	void addSelectedObject(ObjectComponent* comp);
	void removeSelectedObject(ObjectComponent* comp);
	int getNumSelected() { return selectedObjects.size(); }
	bool multipleObjectsSelected() { return isMultipleSelection; }
	void deselectAllSelectedObjects();
	ObjectComponent* getSelectedObject(int index);

	void updateSelectedObjects();
private:

	ObjController& objController;
	MDLFile* mdlFile;

	bool dragging;
	Point<int> draggingStart;
	Point<int> draggingActual;

	Array<ObjectComponent*> selectedObjects;

	bool isMultipleSelection;
};


#endif  // __OBJCOMP_H_F3604232__
