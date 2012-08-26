/*
  ==============================================================================

    ObjectComponent.cpp
    Created: 16 May 2012 9:29:32pm
    Author:  peter

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

#include "../Application/CommonHeaders.h"
#include "../Controller/ObjController.h"
#include "ObjectsHolder.h"
#include "ObjectPropertiesPanel.h"
#include "../Models/ObjectActions.h"

#include "BaseObjectComponent.h"
#include "ObjectComponent.h"

ObjectComponent::ObjectComponent(ObjController& owner_, ValueTree data_)
: BaseObjectComponent(owner_, data_),
  isSelected(false)
{
	shadow.setShadowProperties (
			DropShadow (Colours::black.withAlpha (0.5f), 3, Point<int> (0, 1)));
	setComponentEffect (&shadow);

	setSize(50, 50);
	originalPos.setXY(data.getProperty(Ids::posX), data.getProperty(Ids::posY));
	actualPos.setXY(data.getProperty(Ids::posX), data.getProperty(Ids::posY));
	icon = dynamic_cast<DrawableComposite*> (ResourceLoader::getInstance()->getDrawableForId(data.getType()));
    
    owner.getSelectedObjects().addChangeListener (this);
    selfChangeListenerList.addChangeListener (this);
}
ObjectComponent::~ObjectComponent()
{
    selfChangeListenerList.removeChangeListener(this);
    owner.getSelectedObjects().removeChangeListener (this);
}

bool ObjectComponent::hitTest (int x, int y)
{
	for (int i = getNumChildComponents(); --i >= 0;)
		if (getChildComponent(i)->getBounds().contains (x, y))
			return true;

	return x >= 4 && x < getWidth() - 4 && y >= 4 && y < getHeight() - 4;
}

void ObjectComponent::paint(Graphics& g)
{
    Colour c(1.0f, 1.0f, 1.0f, 0.0f);
	g.setColour (c);

	const int x = 4;
	const int y = 4;
	const int w = getWidth() - x * 2;
	const int h = getHeight() - y * 2;

	g.fillRect (x, y, w, h);

//	g.setColour (Colours::grey);
//	g.drawRect (x, y, w, h);
	Rectangle<float> rect(x+2, y+2, w-4, h-4);
	icon->drawWithin(g, rect, RectanglePlacement::centred, 1.0f);

	if(isSelected)
	{
//		Colour col(0x88228b22);
		g.setColour(Colours::black);
		g.drawRect(x, y, w, h, 2);
	}
}

void ObjectComponent::setOriginalPosition()
{
	originalPos = localPointToGlobal (Point<int>());

}

void ObjectComponent::mouseDown (const MouseEvent& e)
{
	originalPos = localPointToGlobal (Point<int>());

	toFront (true);

    dragging = false;

    if (e.mods.isPopupMenu() && owner.getSelectedObjects().getNumSelected() == 2)
    {
        String startObj;
        String endObj;
        if (owner.getSelectedObjects().getNumSelected() == 2)
        {
            startObj = owner.getSelectedObjects().getItemArray()[0]->getData().getProperty(Ids::identifier).toString();
            endObj = owner.getSelectedObjects().getItemArray()[1]->getData().getProperty(Ids::identifier).toString();
            DBG(String("Link: ") + startObj + String(", ") + endObj);
            getObjectsHolder()->showLinkPopupMenu(startObj, endObj);
        }

    }
    else if (e.mods.isPopupMenu())
    {
        showContextMenu();
        return; // this may be deleted now..
    }

//    if(! e.mods.isAnyModifierKeyDown())
//    {
//        owner.getSelectedObjects().deselectAll();
//    }
    mouseDownSelectStatus = owner.getSelectedObjects().addToSelectionOnMouseDown (this, e.mods);

}

void ObjectComponent::mouseDrag (const MouseEvent& e)
{
    
    if (! e.mods.isPopupMenu())
    {
        jassert (dynamic_cast <ObjectsHolder*> (getParentComponent()) != 0);
//        const Rectangle<int> area (((ObjectsHolder*) getParentComponent())->getComponentArea());
        
        if (isSelected && ! dragging)
        {
            dragging = ! e.mouseWasClicked();

            if (dragging)
            {
                owner.startDragging ();
            }
        }

        if (dragging)
        {
            owner.dragSelectedComps (e.getDistanceFromDragStartX(),
                                      e.getDistanceFromDragStartY());
        }
        update();
    }
}

void ObjectComponent::mouseUp (const MouseEvent& e)
{
    if (dragging)
        owner.endDragging();

	if (e.mouseWasClicked() && e.getNumberOfClicks() == 2)
	{
		getObjectsHolder()->editObjectProperties(this);
	}

    owner.getSelectedObjects().addToSelectionOnMouseUp (this, e.mods, dragging, mouseDownSelectStatus);

    update();
}

void ObjectComponent::update()
{
	setCentrePosition((float) actualPos.x, (float) actualPos.y);
}

void ObjectComponent::setPosition(Point<int> newPos, bool undoable)
{
    if (actualPos != newPos)
    {
        if (undoable)
        {
            owner.getUndoManager()->perform(new MoveObjectAction(getObjectsHolder(),
                                                                 this, newPos, &owner),
                                            "Move object");
        }
        else
        {
            data.setProperty(Ids::posX, newPos.getX(), nullptr);
            data.setProperty(Ids::posY, newPos.getY(), nullptr);
            setActualPosition(newPos);
            getObjectsHolder()->updateComponents();
        }
    }
}
void ObjectComponent::setActualPosition(Point<int> pos)
{
	actualPos = pos;
}

void ObjectComponent::setData(ValueTree dataTree)
{
	data = dataTree;
}

void ObjectComponent::setSelected(bool shouldBeSelected)
{
	isSelected = shouldBeSelected;
	if(isSelected)
    {
        owner.getSelectedObjects().addToSelection(this);
    }
	else
    {
        owner.getSelectedObjects().deselect(this);
    }
   	repaint();
}

void ObjectComponent::toggleSelected()
{
	if(isSelected)
		setSelected(false);
	else
		setSelected(true);
	repaint();
}

void ObjectComponent::changeListenerCallback (ChangeBroadcaster*)
{
    const bool nowSelected = owner.getSelectedObjects().isSelected (this);

    if (isSelected != nowSelected)
    {
        isSelected = nowSelected;
        repaint();
    }
}
