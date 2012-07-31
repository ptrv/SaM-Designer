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

#include "ObjectComponent.h"

ObjectComponent* ObjectComponent::isLastClicked = nullptr;
ObjectComponent::ObjectComponent(const Identifier& objId_, int x, int y)
: objId(objId_),
  isSelected(false)
{
	shadow.setShadowProperties (
			DropShadow (Colours::black.withAlpha (0.5f), 3, Point<int> (0, 1)));
	setComponentEffect (&shadow);

	setSize(50, 50);
	originalPos.setXY(x, y);
	actualPos.setXY(x, y);
	icon = dynamic_cast<DrawableComposite*> (ResourceLoader::getInstance()->getDrawableForId(objId));
}
ObjectComponent::~ObjectComponent()
{
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
//bool shouldBeUnselected = false;
void ObjectComponent::mouseDown (const MouseEvent& e)
{
	originalPos = localPointToGlobal (Point<int>());

	toFront (true);

	isLastClicked = this;

    if (e.mods.isPopupMenu() && getObjectsHolder()->getNumSelected() == 2)
    {
    	showLinkPopupMenu();
    }
    else if(e.mods.isPopupMenu())
    {
    	showContextMenu();
    }
	// TODO: not perfect, implement shift click deselection

	if(! isSelected)// && ! e.mods.isShiftDown() )
	{
		setSelected(true);
		if(! e.mods.isShiftDown())
		{
			for (int i = getObjectsHolder()->getNumSelected(); --i >= 0;){
				if(getObjectsHolder()->getSelectedObject(i) != isLastClicked)
					getObjectsHolder()->getSelectedObject(i)->setSelected(false);
			}
		}
	}
	else if(! e.mods.isShiftDown() && ! getObjectsHolder()->multipleObjectsSelected())
	{
		for (int i = getObjectsHolder()->getNumSelected(); --i >= 0;)
		{
			if(getObjectsHolder()->getSelectedObject(i) != isLastClicked)
				getObjectsHolder()->getSelectedObject(i)->setSelected(false);
		}

	}
	else if(e.mods.isShiftDown())// && ! getObjectsHolder()->multipleObjectsSelected())
	{
//		setSelected(false);
//		shouldBeUnselected = true;
	}

	// set origianl position
	for (int i = 0; i < getObjectsHolder()->getNumSelected(); ++i) {
		if(getObjectsHolder()->getSelectedObject(i) != isLastClicked)
			getObjectsHolder()->getSelectedObject(i)->setOriginalPosition();
	}

}

void ObjectComponent::mouseDragPassive(const Point<int> offset)
{
	Point<int> pos (originalPos + offset);

	if (getParentComponent() != nullptr)
		pos = getParentComponent()->getLocalPoint (nullptr, pos);

	actualPos.x = (pos.getX() + getWidth() /2);
	actualPos.y = (pos.getY() + getHeight() /2);
}

void ObjectComponent::mouseDrag (const MouseEvent& e)
{
	Point<int> pos (originalPos + Point<int> (e.getDistanceFromDragStartX(), e.getDistanceFromDragStartY()));

	if (getParentComponent() != nullptr)
		pos = getParentComponent()->getLocalPoint (nullptr, pos);

	actualPos.x = (pos.getX() + getWidth() /2);
	actualPos.y = (pos.getY() + getHeight() /2);
	getObjectsHolder()->moveObjectComponents(Point<int> (e.getDistanceFromDragStartX(), e.getDistanceFromDragStartY()));
	getObjectsHolder()->updateComponents();

}

void ObjectComponent::mouseUp (const MouseEvent& e)
{
	if (e.mouseWasClicked() && e.getNumberOfClicks() == 2)
	{
		getObjectsHolder()->editObjectProperties(this);
	}
	else if( e.mouseWasClicked() && e.getNumberOfClicks() == 1)
	{
//		if(shouldBeUnselected && e.mods.isShiftDown())
//			setSelected(false);
//
	}
	else if (! e.mouseWasClicked())
	{
		// object changed / mouse realeased after drag
		getObjectsHolder()->moveObjectsData(e.getOffsetFromDragStart());
	}
}

void ObjectComponent::update()
{
	setCentrePosition((float) actualPos.x, (float) actualPos.y);
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
		getObjectsHolder()->addSelectedObject(this);
	else
		getObjectsHolder()->removeSelectedObject(this);
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
		DBG("Help is not implemented yet");
	}

}

