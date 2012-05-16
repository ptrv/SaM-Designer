/*
  ==============================================================================

    BaseObjectComponent.h
    Created: 5 May 2012 4:31:59pm
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

#ifndef __BASEOBJECTCOMPONENT_H_4AC10D4B__
#define __BASEOBJECTCOMPONENT_H_4AC10D4B__

#include "../Application/CommonHeaders.h"
#include "ObjectsHolder.h"
#include "ObjectPropertiesPanel.h"


class ObjectComponent : public Component
{
public:
	ObjectComponent(const Identifier& objId_, int x, int y)
	: objId(objId_)
	{
		shadow.setShadowProperties (5.5f, 0.5f, -1, 0);
		setComponentEffect (&shadow);

		setSize(50, 50);
		actualPos.setXY(x, y);
		icon = dynamic_cast<DrawableComposite*> (ResourceLoader::getInstance()->getDrawableForId(objId));
	}
	~ObjectComponent()
	{
	}

    bool hitTest (int x, int y)
    {
        for (int i = getNumChildComponents(); --i >= 0;)
            if (getChildComponent(i)->getBounds().contains (x, y))
                return true;

        return x >= 4 && x < getWidth() - 4 && y >= 4 && y < getHeight() - 4;
    }

	void paint(Graphics& g)
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
	}

	void mouseDown (const MouseEvent& e)
	{
		originalPos = localPointToGlobal (Point<int>());

		toFront (true);
	}

	void mouseDrag (const MouseEvent& e)
	{
		Point<int> pos (originalPos + Point<int> (e.getDistanceFromDragStartX(), e.getDistanceFromDragStartY()));

		if (getParentComponent() != nullptr)
			pos = getParentComponent()->getLocalPoint (nullptr, pos);

		actualPos.x = (pos.getX() + getWidth() /2);
		actualPos.y = (pos.getY() + getHeight() /2);
		getGraphPanel()->updateComponents();

	}

	void mouseUp (const MouseEvent& e)
	{
		if (e.mouseWasClicked() && e.getNumberOfClicks() == 2)
		{
			ObjectPropertiesPanel::show(this);
		}
		else if (! e.mouseWasClicked())
		{
			// object changed / mouse realeased after drag
			// TODO: Implement moving object
		}
	}

	void update()
	{
        setCentrePosition((float) actualPos.x, (float) actualPos.y);
	}

	void setData(ValueTree dataTree)
	{
		data = dataTree;
	}
private:
	const Identifier& objId;
	DrawableComposite* icon;
    DropShadowEffect shadow;

	Point<int> actualPos;
	Point<int> originalPos;

    ObjectsHolder* getGraphPanel() const noexcept
    {
        return findParentComponentOfClass<ObjectsHolder>();
    }

    ValueTree data;
};


#endif  // __BASEOBJECTCOMPONENT_H_4AC10D4B__
