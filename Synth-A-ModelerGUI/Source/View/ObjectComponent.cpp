/*
  ==============================================================================

    ObjectComponent.cpp
    Created: 16 May 2012 9:29:32pm
    Author:  peter

  ==============================================================================
*/

#include "../Application/CommonHeaders.h"
#include "ObjectsHolder.h"
#include "ObjectPropertiesPanel.h"

#include "ObjectComponent.h"

ObjectComponent::ObjectComponent(const Identifier& objId_, int x, int y)
: objId(objId_),
  isSelected(false)
{
	shadow.setShadowProperties (5.5f, 0.5f, -1, 0);
	setComponentEffect (&shadow);

	setSize(50, 50);
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
		g.setColour(col);
		g.drawRect(x, y, w, h, 4);
	}
}

void ObjectComponent::mouseDown (const MouseEvent& e)
{
	originalPos = localPointToGlobal (Point<int>());

	toFront (true);
}

void ObjectComponent::mouseDrag (const MouseEvent& e)
{
	Point<int> pos (originalPos + Point<int> (e.getDistanceFromDragStartX(), e.getDistanceFromDragStartY()));

	if (getParentComponent() != nullptr)
		pos = getParentComponent()->getLocalPoint (nullptr, pos);

	actualPos.x = (pos.getX() + getWidth() /2);
	actualPos.y = (pos.getY() + getHeight() /2);
	getGraphPanel()->updateComponents();

}

void ObjectComponent::mouseUp (const MouseEvent& e)
{
	if (e.mouseWasClicked() && e.getNumberOfClicks() == 2)
	{
		ObjectPropertiesPanel::show(this);
	}
	else if( e.mouseWasClicked() && e.getNumberOfClicks() == 1)
	{
		toggleSelected();
	}
	else if (! e.mouseWasClicked())
	{
		// object changed / mouse realeased after drag
		// TODO: Implement moving object
	}
}

void ObjectComponent::update()
{
	setCentrePosition((float) actualPos.x, (float) actualPos.y);
}

void ObjectComponent::setData(ValueTree dataTree)
{
	data = dataTree;
}

ObjectsHolder* ObjectComponent::getGraphPanel() const noexcept
{
    return findParentComponentOfClass<ObjectsHolder>();
}

void ObjectComponent::setSelected(bool shouldBeSelected)
{
	isSelected = shouldBeSelected;
	repaint();
}

void ObjectComponent::toggleSelected()
{
	isSelected = !isSelected;
	repaint();
}
