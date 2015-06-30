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

#include "ObjectComponent.h"

#include "Application/CommonHeaders.h"

#include "Controller/ObjController.h"

#include "Graph/Node.h"

#include "Models/ObjectActions.h"

#include "ObjectsHolder.h"
#include "IdLabel.h"

#include "Utilities/ContextMenus.h"


using namespace synthamodeler;

ObjectComponent::ObjectComponent(ObjController& owner_, ValueTree data_)
: BaseObjectComponent(owner_, data_), dragging(false), mouseDownSelectStatus(false)
{
    // shadow.setShadowProperties (
    //         DropShadow (Colours::black.withAlpha (0.5f), 3, Point<int> (0, 1)));
    // setComponentEffect (&shadow);

    if(data.getType() == Ids::mass)
        setSize(35, 35);
    else if(data.getType() == Ids::port)
        setSize(75, 75);
    else if(data.getType() == Ids::audioout)
        setSize(60,60);
    else if(data.getType() == Ids::termination)
        setSize(55,55);
    else if(data.getType() == Ids::junction)
        setSize(25,25);
    else
        setSize(50, 50);

    const Point<int> currentPos(data.getProperty(Ids::posX), data.getProperty(Ids::posY));

    setCentrePosition(currentPos.x, currentPos.y);
    actualPos = currentPos;
    oldPos = currentPos;

    icon = dynamic_cast<DrawableComposite*> (ResourceLoader::getInstance()->getDrawableForObjectId(data.getType()));

    owner.getSelectedObjects().addChangeListener (this);
    selfChangeListenerList.addChangeListener (this);

    setComponentID("Object_" + data[Ids::identifier].toString());
}
ObjectComponent::~ObjectComponent()
{
    masterReference.clear();
    selfChangeListenerList.removeChangeListener(this);
    owner.getSelectedObjects().removeChangeListener (this);
}

bool ObjectComponent::hitTest (int x, int y)
{
    for (int i = getNumChildComponents(); --i >= 0;)
    {
        if (getChildComponent(i)->getBounds().contains(x, y))
        {
            return true;
        }
    }

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

	if(selected)
	{
//		Colour col(0x88228b22);
		g.setColour(Colours::black);
		g.drawRect(x, y, w, h, 2);
	}
}

void ObjectComponent::mouseDown (const MouseEvent& e)
{
    toFront (true);

    if (idLabel)
    {
        idLabel->toFront(false);
    }

    dragging = false;

    SelectedItemSet<SelectableObject*>& sis = owner.getSelectedObjects();

    if (owner.getIsReflowing())
    {
        sis.selectOnly(this);
    }

    if (e.mods.isPopupMenu() && sis.getNumSelected() == 2)
    {
        String startObj;
        String endObj;
        ObjectComponent* oc1 = dynamic_cast<ObjectComponent*>(sis.getSelectedItem(0));
        ObjectComponent* oc2 = dynamic_cast<ObjectComponent*>(sis.getSelectedItem(1));
        if (oc1 != nullptr && oc2 != nullptr)
        {
            startObj = oc1->getData().getProperty(Ids::identifier).toString();
            endObj = oc2->getData().getProperty(Ids::identifier).toString();
            DBG(String("Link: ") + startObj + String(", ") + endObj);
            ContextMenus::showLinkPopupMenu(*getObjectsHolder(), startObj, endObj);
        }
        else if (oc1 == nullptr)
        {
            LinkComponent* lc1 = dynamic_cast<LinkComponent*>(sis.getSelectedItem(0));
            if (lc1 != nullptr)
            {
                ContextMenus::showAudioConnectionPopupMenu(*getObjectsHolder());
            }
        }
        else if (oc2 == nullptr)
        {
            LinkComponent* lc2 = dynamic_cast<LinkComponent*>(sis.getSelectedItem(1));
            if (lc2 != nullptr)
            {
                ContextMenus::showAudioConnectionPopupMenu(*getObjectsHolder());
            }
        }
        return;
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
    mouseDownSelectStatus = sis.addToSelectionOnMouseDown (this, e.mods);

}

void ObjectComponent::mouseDrag (const MouseEvent& e)
{
    
    if (! e.mods.isPopupMenu())
    {
        jassert (dynamic_cast <ObjectsHolder*> (getParentComponent()) != 0);
//        const Rectangle<int> area (((ObjectsHolder*) getParentComponent())->getComponentArea());
        
        if (selected && ! dragging)
        {
            dragging = ! e.mouseWasClicked();

            if (dragging)
            {
                owner.startDragging ();
            }
        }

        if (dragging)
        {
            owner.dragSelectedComps(e.getDistanceFromDragStartX(),
                                    e.getDistanceFromDragStartY());
        }
    }
}

void ObjectComponent::mouseUp (const MouseEvent& e)
{
    if (dragging)
    {
        owner.endDragging();
    }

	if (e.mouseWasClicked() && e.getNumberOfClicks() == 2)
	{
        propertiesWindow->makeVisible(true);
	}

    owner.getSelectedObjects().addToSelectionOnMouseUp (this, e.mods, dragging, 
                                                        mouseDownSelectStatus);
}

void ObjectComponent::update()
{
    setCentrePosition((float) actualPos.x, (float) actualPos.y);
    oldPos = getCenter();
}

void ObjectComponent::setPosition(Point<int> newPos, bool undoable)
{
    if (getOldPos() != newPos)
    {
        if (undoable)
        {
            owner.getUndoManager().perform(new MoveObjectAction(this, newPos, &owner),
                                            "Move object");
        }
        else
        {
            newPos.x = jmax(0, newPos.x);
            newPos.y = jmax(0, newPos.y);

            data.setProperty(Ids::posX, newPos.getX(), nullptr);
            data.setProperty(Ids::posY, newPos.getY(), nullptr);
            setActualPosition(newPos);
            setOldPos(getCenter());
        }
    }
}
void ObjectComponent::setActualPosition(Point<int> pos, NotificationType n)
{
    actualPos = pos;
    setCentrePosition(pos.x, pos.y);

    if (n == sendNotification)
    {
        sendChangeMessage();
    }

    if (idLabel)
    {
        idLabel->updatePosition(getBounds(), getObjectsHolder()->getLocalBounds());
    }
}

void ObjectComponent::setData(ValueTree dataTree)
{
	data = dataTree;
}

void ObjectComponent::setSelected(bool shouldBeSelected)
{
	selected = shouldBeSelected;
	if(selected)
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
    setSelected(!selected);
	repaint();
}

void ObjectComponent::changeListenerCallback (ChangeBroadcaster*)
{
    const bool nowSelected = owner.getSelectedObjects().isSelected (this);

    if (selected != nowSelected)
    {
        selected = nowSelected;
        repaint();
    }
}

void ObjectComponent::addLinkToObject(LinkComponent* link)
{
    connectedLinks.add(link);
    addChangeListener(link);
}

void ObjectComponent::removeLinkFromObject(LinkComponent* link)
{
    connectedLinks.removeAllInstancesOf(link);
    removeChangeListener(link);
}

Point<int> ObjectComponent::getPinPos() const
{
    Point<int> pinPos = getActualPos();
    return pinPos + getPinOffset();
}

Point<int> ObjectComponent::getPinOffset() const
{
    Point<int> pinPos(0,0);
    if (data.getType() == Ids::ground)
    {
        pinPos.x += 3;
        pinPos.y -= 14;
    }
    else if (data.getType() == Ids::port)
    {
        pinPos.x -= 11;
        pinPos.y -= 29;
    }
    else if (data.getType() == Ids::audioout)
    {
        pinPos.x -= 19;
    }
    else if (data.getType() == Ids::termination)
    {
        pinPos.y -=10;
    }
    return pinPos;
}

bool ObjectComponent::canBeConnected(const Identifier& objId) const
{
    if (ObjectsHelper::isLink(objId))
    {
        return canBeConnectedToLinks();
    }
    else if(objId == Ids::waveguide)
    {
        return canBeConnectedToWaveguides();
    }
    else
    {
        return false;
    }
}
bool ObjectComponent::canBeConnectedToLinks() const
{
    if(ObjectsHelper::isMass(data.getType()))
    {
        return true;
    }
    else if(data.getType() == Ids::junction)
    {
        // only 1 link allowed for junctions.
        int connections = 0;
        for (int i = 0; i < connectedLinks.size(); ++i)
        {
            if(connectedLinks.getUnchecked(i)->getData().getType() != Ids::waveguide)
                ++connections;

        }
        return connections == 0;
    }
    else
    {
        return false;
    }
}
bool ObjectComponent::canBeConnectedToWaveguides() const
{
    if(data.getType() == Ids::termination || data.getType() == Ids::junction)
    {
        return true;
    }
    else
    {
        return false;
    }
}

IdLabel* ObjectComponent::createIdLabel()
{
    return new IdLabel(getIdLabelComp(), getBounds(), getObjectsHolder());
}
