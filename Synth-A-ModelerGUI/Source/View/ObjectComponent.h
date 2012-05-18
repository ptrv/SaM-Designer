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

class ObjectsHolder;
class ObjController;

/**
 * The object component.
 */
class ObjectComponent : public Component
{
public:
	ObjectComponent(const Identifier& objId_, int x, int y);
	~ObjectComponent();
    bool hitTest (int x, int y);
	void paint(Graphics& g);
	void mouseDown (const MouseEvent& e);
	void mouseDragPassive (const Point<int> offset);
	void mouseDrag (const MouseEvent& e);
	void mouseUp (const MouseEvent& e);
	void update();
	void setData(ValueTree dataTree);
	ValueTree getData();

	void setSelected(bool shouldBeSelected);
	bool selected() const { return isSelected; }
	void toggleSelected();

	void setActualPosition(Point<int> pos);

	void setOriginalPosition();

	static ObjectComponent* isLastClicked;

	const Identifier& getId() const { return objId; }
private:

	void showLinkPopupMenu();
	void showContextMenu();

	const Identifier& objId;
	DrawableComposite* icon;
    DropShadowEffect shadow;

	Point<int> actualPos;
	Point<int> originalPos;

    ObjectsHolder* getObjectsHolder() const noexcept;
    ValueTree data;

    bool isSelected;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ObjectComponent);
};


#endif  // __BASEOBJECTCOMPONENT_H_4AC10D4B__
