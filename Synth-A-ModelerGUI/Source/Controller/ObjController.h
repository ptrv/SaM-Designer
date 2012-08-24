/*
  ==============================================================================

    ObjController.h
    Created: 13 Apr 2012 12:06:25am
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

#ifndef __OBJCONTROLLER_H_A98EC6A3__
#define __OBJCONTROLLER_H_A98EC6A3__


class MDLController;
class BaseObjectComponent;
class ObjectComponent;
class ObjectsHolder;
class LinkComponent;

/**
 * The ObjController controlls all ObjectComponents.
 */
class ObjController
{
public:
	ObjController(MDLController& owner_);
	~ObjController();

	/**
	 * Performs the action with the UndoManger.
	 *
	 * @param action		the action to perform
	 * @param actionName	the action name
	 * @return
	 */
	bool perform (UndoableAction* const action, const String& actionName);

	/**
	 * Adds and object to the patch.
	 *
	 * @param holder 		the holder conponent which consists the objects.
	 * @param objId			object id (i.e. mass or link)
	 * @param posX			initial x position of the object
	 * @param posY			initial y position of the object
	 */
	ObjectComponent* addObject(ObjectsHolder* holder, ValueTree objValues, int index, bool undoable);

    void addNewObject(ObjectsHolder* holder, ValueTree objValues);
    
    void addComponent(ObjectComponent* comp, int index);
    
	/**
	 * Removes selected objects from the patch
	 *
	 * @param holder		the holder component which consists the objects.
	 */
	void removeSelectedObjects(ObjectsHolder* holder);

    void removeObject(ObjectComponent* objComp, bool undoable, ObjectsHolder* holder);

    LinkComponent* addLink(ObjectsHolder* holder, ValueTree linkValues, int index, bool undoable);
    
    void addNewLinkIfPossible(ObjectsHolder* holder, ValueTree linkValues);
    
    void addNewLink(ObjectsHolder* holder, ValueTree linkValues);
    
    void addLinkComponent(LinkComponent* comp, int index);

	void removeSelectedLinks(ObjectsHolder* holder);

    void removeLink(LinkComponent* linkComp, bool undoable, ObjectsHolder* holder);
	/**
	 * Loads the object components of a patch when a mdl file is opened.
	 * @param holder
	 */
	void loadComponents(ObjectsHolder* holder);

	/**
	 * Marks all object as selected or deselects.
	 *
	 * @param shouldBeSelected		true for selected, false for deselected
	 */
	void selectAll(bool shouldBeSelected);

	/**
	 * Edits the properties of an object.
	 *
	 * @param oc			the object component to edit
	 * @param undoManager	the MDLFile's UndoManager
	 */
	void editObjectProperties(BaseObjectComponent* oc, UndoManager* undoManager);
    
    SelectedItemSet <ObjectComponent*>& getSelectedObjects() throw()
    { 
        return selectedObjects; 
    }
    SelectedItemSet <LinkComponent*>& getSelectedLinks() throw()
    {
        return selectedLinks;
    }
    
    void startDragging (const Rectangle<int>& parentArea);
    void dragSelectedComps (int dxFromDragStart, int dyFromDragStart, const Rectangle<int>& parentArea);
    void endDragging();
    
    UndoManager* getUndoManager();

    ObjectComponent* getObject(int index) const throw() { return objects[index]; }
    int indexOfObject (ObjectComponent* e) const throw() { return objects.indexOf (e); }

    LinkComponent* getLink(int index) const throw() { return links[index]; }
    int indexOfLink (LinkComponent* e) const throw() { return links.indexOf (e); }
    
    void changed();
    
    ObjectComponent* getObjectForId(String idString) const throw();
    
    void reverseLinkDirection();
private:
    
    bool checkIfLinkExitsts(ValueTree linkTree);
    bool checkIfObjectHasLink(ObjectComponent* objComp, int& linkIndex);
	MDLController& owner;
	OwnedArray<ObjectComponent> objects;
    OwnedArray<LinkComponent> links;
    SelectedItemSet<ObjectComponent*> selectedObjects;
    SelectedItemSet<LinkComponent*> selectedLinks;


};


#endif  // __OBJCONTROLLER_H_A98EC6A3__
