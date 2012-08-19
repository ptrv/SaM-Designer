/*
  ==============================================================================

    ObjectComponent.cpp
    Created: 16 May 2012 9:29:32pm
    Author:  peter

  ==============================================================================
*/

#include "../Application/CommonHeaders.h"
#include "../Controller/ObjController.h"
#include "ObjectsHolder.h"
#include "ObjectPropertiesPanel.h"
#include "../Models/ObjectActions.h"

#include "ObjectComponent.h"

ObjectComponent* ObjectComponent::isLastClicked = nullptr;
ObjectComponent::ObjectComponent(ObjController& _owner, const Identifier& objId_, int x, int y)
: owner(_owner),
    objId(objId_),
  isSelected(false)
{
	shadow.setShadowProperties (
			DropShadow (Colours::black.withAlpha (0.5f), 3, Point<int> (0, 1)));
	setComponentEffect (&shadow);

	setSize(50, 50);
	originalPos.setXY(x, y);
	actualPos.setXY(x, y);
	icon = dynamic_cast<DrawableComposite*> (ResourceLoader::getInstance()->getDrawableForId(objId));
    
    owner.getSelectedElements().addChangeListener (this);
    selfChangeListenerList.addChangeListener (this);
}
ObjectComponent::~ObjectComponent()
{
//    selfChangeListenerList.removeChangeListener(this);
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
	g.setColour (Colours::lightgrey);

	const int x = 4;
	const int y = 4;
	const int w = getWidth() - x * 2;
	const int h = getHeight() - y * 2;

	g.fillRect (x, y, w, h);

	g.setColour (Colours::grey);
	g.drawRect (x, y, w, h);
	Rectangle<float> rect(x+2, y+2, w-4, h-4);
	icon->drawWithin(g, rect, RectanglePlacement::centred, 1.0f);

	if(isSelected)
	{

		Colour col(0x88228b22);
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

	isLastClicked = this;

    dragging = false;

    if (e.mods.isPopupMenu() && owner.getSelectedElements().getNumSelected() == 2)
    {
    	showLinkPopupMenu();
    }
    if (e.mods.isPopupMenu())
    {
        showContextMenu();
        return; // this may be deleted now..
    }

    if(! e.mods.isShiftDown())
    {
        owner.getSelectedElements().deselectAll();
    }
    mouseDownSelectStatus = owner.getSelectedElements().addToSelectionOnMouseDown (this, e.mods);

}

void ObjectComponent::mouseDrag (const MouseEvent& e)
{
    
    if (! e.mods.isPopupMenu())
    {
        jassert (dynamic_cast <ObjectsHolder*> (getParentComponent()) != 0);
        const Rectangle<int> area (((ObjectsHolder*) getParentComponent())->getComponentArea());

        if (isSelected && ! dragging)
        {
            dragging = ! e.mouseWasClicked();

            if (dragging)
            {
                owner.startDragging (area);
            }
        }

        if (dragging)
        {
            owner.dragSelectedComps (e.getDistanceFromDragStartX(),
                                      e.getDistanceFromDragStartY(),
                                      area);
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
    
    owner.getSelectedElements().addToSelectionOnMouseUp (this, e.mods, dragging, mouseDownSelectStatus);

    update();
}

void ObjectComponent::update()
{
	setCentrePosition((float) actualPos.x, (float) actualPos.y);
}

void ObjectComponent::setPosition(Point<int> newPos)
{
    owner.getUndoManager()->perform(new MoveObjectAction(getObjectsHolder(), this, newPos), "Move object");
//    owner.perform()
}
void ObjectComponent::setActualPosition(Point<int> pos)
{
	actualPos = pos;
}

void ObjectComponent::setData(ValueTree dataTree)
{
	data = dataTree;
}

ValueTree ObjectComponent::getData()
{
	return data;
}

ObjectsHolder* ObjectComponent::getObjectsHolder() const noexcept
{
    return findParentComponentOfClass<ObjectsHolder>();
}

void ObjectComponent::setSelected(bool shouldBeSelected)
{
	isSelected = shouldBeSelected;
	if(isSelected)
    {
        owner.getSelectedElements().addToSelection(this);
    }
	else
    {
        owner.getSelectedElements().deselect(this);
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

void ObjectComponent::showLinkPopupMenu()
{
	PopupMenu m;
	m.addItem (1, "Add link");
	m.addItem (2, "Add touch");
	m.addItem (3, "Add pluck");
	m.addSeparator();
	m.addItem (4, "Connect");
	const int r = m.show();

	if (r == 1)
	{
		DBG("Add link");
		return;
	}
	else if (r == 2)
	{
		DBG("Add touch");
	}
	else if (r == 3)
	{
		DBG("Add pluck");
	}
	else if (r == 4)
	{
		DBG("Add connect");
	}
}
void ObjectComponent::showContextMenu()
{
	PopupMenu m;
	m.addItem (1, "Edit");
	m.addSeparator();
	m.addItem (2, "Help");

	const int r = m.show();

	if (r == 1)
	{
		getObjectsHolder()->editObjectProperties(this);
	}
	else if (r == 2)
	{
        Utils::openHelpUrl(objId);
		DBG("open help for " + objId.toString());
	}

}

void ObjectComponent::changeListenerCallback (ChangeBroadcaster*)
{
    const bool nowSelected = owner.getSelectedElements().isSelected (this);

    if (isSelected != nowSelected)
    {
        isSelected = nowSelected;
        repaint();
    }
}
