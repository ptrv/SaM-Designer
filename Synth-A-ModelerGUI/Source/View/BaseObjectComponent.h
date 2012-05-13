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

class BaseObjectComponent : public Component
{
public:
	BaseObjectComponent(const Identifier& objId_, int x, int y)
	: objId(objId_)
	{
		shadow.setShadowProperties (5.5f, 0.5f, -1, 0);
		setComponentEffect (&shadow);

		setSize(50, 50);
//		originalPos.setXY(100, 100);
		actualPos.setXY(x, y);
		icon = dynamic_cast<DrawableComposite*> (ResourceLoader::getInstance()->getDrawableForId(objId));

	}
	~BaseObjectComponent()
	{
		icon = nullptr;
	}

	void paint(Graphics& g)
	{
		g.setColour (Colours::lightgrey);

		const int x = 4;
		const int y = 4;
		const int w = getWidth() - x * 2;
		const int h = getHeight() - y * 2;

		g.fillRect (x, y, w, h);

		g.setColour (Colours::black);
//		g.setFont (font);
//		g.drawFittedText (getName(),
//						  x + 4, y + 2, w - 8, h - 4,
//						  Justification::centred, 2);
//
		g.setColour (Colours::grey);
		g.drawRect (x, y, w, h);
//		Rectangle<float> rect(0.0f, 0.0f, (float)getWidth(), (float)getWidth());
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

		actualPos.x = (pos.getX() + getWidth() /2);// / (double)(getParentWidth());
		actualPos.y = (pos.getY() + getHeight() /2);// / (double)(getParentHeight());
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
	            // object changed
		}
	}

	void update()
	{
        setCentrePosition((float) actualPos.x, (float) actualPos.y);
	}
private:
	const Identifier& objId;
	ScopedPointer<DrawableComposite> icon;
    DropShadowEffect shadow;

	Point<int> actualPos;
	Point<int> originalPos;

    ObjectsHolder* getGraphPanel() const noexcept
    {
        return findParentComponentOfClass<ObjectsHolder>();
    }

};


#endif  // __BASEOBJECTCOMPONENT_H_4AC10D4B__
