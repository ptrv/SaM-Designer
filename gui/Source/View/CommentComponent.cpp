/*
  ==============================================================================

    CommentComponent.cpp
    Created: 17 Oct 2012 4:56:15pm
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

#include "CommentComponent.h"

#include "Application/CommonHeaders.h"

#include "Controller/ObjController.h"

#include "Graph/Node.h"

#include "Models/ObjectActions.h"

#include "LinkComponent.h"
#include "ObjectsHolder.h"
#include "CommentEditor.h"


using namespace synthamodeler;

CommentComponent::CommentComponent(ObjController& owner_, ValueTree data_)
: BaseObjectComponent(owner_, data_)
{
    // shadow.setShadowProperties (
    //         DropShadow (Colours::black.withAlpha (0.5f), 3, Point<int> (0, 1)));
    // setComponentEffect (&shadow);

    const String commentColorStr = data.getProperty(Ids::commentColour, Colours::black.toString());
    commentColor = Colour::fromString(commentColorStr);
    textField = new CommentEditor(*this, data.getProperty(Ids::fontSize, 16.0f), commentColor);
    textField->setText(data[Ids::value].toString(), false);
    textField->addListener(this);
    addAndMakeVisible(textField);

    const Point<int> currentPos(data.getProperty(Ids::posX), data.getProperty(Ids::posY));

    setCentrePosition(currentPos.x, currentPos.y);
    actualPos = currentPos;
    oldPos = currentPos;

    owner.getSelectedObjects().addChangeListener (this);
    selfChangeListenerList.addChangeListener (this);

    data.addListener(this);

    setComponentID("comment_" + data[Ids::identifier].toString());
}

CommentComponent::~CommentComponent()
{
    selfChangeListenerList.removeChangeListener(this);
    owner.getSelectedObjects().removeChangeListener (this);

    deleteAllChildren();
}

bool CommentComponent::hitTest (int x, int y)
{
	for (int i = getNumChildComponents(); --i >= 0;)
		if (getChildComponent(i)->getBounds().contains (x, y))
			return true;

	return x >= 4 && x < getWidth() - 4 && y >= 4 && y < getHeight() - 4;
}

void CommentComponent::resized()
{
    if(textField->getText().isEmpty())
    {
        setSize(100, textField->getTextHeight()+10);
    }
    else
    {
        setSize(textField->getTextWidth()+10, textField->getTextHeight()+10);
    }
    textField->setBounds(5, 5, getWidth()-10, getHeight() -10);
    
    repaint();
}

void CommentComponent::paint(Graphics& g)
{
    Colour c(0.0f, 0.0f, 0.0f, 0.0f);
	g.setColour (c);

	const int x = 4;
	const int y = 4;
	const int w = getWidth() - x * 2;
	const int h = getHeight() - y * 2;

	g.fillRect (x, y, w, h);

//	g.setColour (Colours::grey);
//	g.drawRect (x, y, w, h);
	Rectangle<float> rect(x+2, y+2, w-4, h-4);

	if(selected)
	{
//		Colour col(0x88228b22);
		g.setColour(Colours::black);
		g.drawRect(x, y, w, h, 2);
	}
}

void CommentComponent::mouseDown (const MouseEvent& e)
{
    toFront (true);

    dragging = false;

    mouseDownSelectStatus = owner.getSelectedObjects().addToSelectionOnMouseDown (this, e.mods);

}

void CommentComponent::mouseDrag (const MouseEvent& e)
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
            owner.dragSelectedComps (e.getDistanceFromDragStartX(),
                                     e.getDistanceFromDragStartY());
        }
    }
}

void CommentComponent::mouseUp (const MouseEvent& e)
{
    if (dragging)
        owner.endDragging();

    owner.getSelectedObjects().addToSelectionOnMouseUp (
        this, e.mods, dragging, mouseDownSelectStatus);
}

void CommentComponent::update()
{
    resized();
    setCentrePosition((float) actualPos.x, (float) actualPos.y);
    oldPos = getCenter();
}

void CommentComponent::setPosition(Point<int> newPos, bool undoable)
{
    if (getOldPos() != newPos)
    {
        if (undoable)
        {
            owner.getUndoManager().perform(new MoveCommentAction(this, newPos, &owner),
                                           "Move comment");
        }
        else
        {
            data.setProperty(Ids::posX, newPos.getX(), nullptr);
            data.setProperty(Ids::posY, newPos.getY(), nullptr);
            setActualPosition(newPos);
            setOldPos(getCenter());
        }
    }
}
void CommentComponent::setActualPosition(Point<int> pos)
{
    actualPos = pos;
    setCentrePosition(pos.x, pos.y);
}

void CommentComponent::setSelected(bool shouldBeSelected)
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

void CommentComponent::toggleSelected()
{
	if(selected)
		setSelected(false);
	else
		setSelected(true);
	repaint();
}

void CommentComponent::changeListenerCallback (ChangeBroadcaster* const source)
{
    if (ColourSelector* cs = dynamic_cast <ColourSelector*> (source))
    {
        commentColor = cs->getCurrentColour();
        textField->setColour (TextEditor::textColourId, commentColor);
        textField->applyFont();
    }
    else
    {
        const bool nowSelected = owner.getSelectedObjects().isSelected (this);

        if (selected != nowSelected)
        {
            selected = nowSelected;
            repaint();
        }
    }
}

void CommentComponent::textEditorTextChanged(TextEditor&)
{
    resized();
}
void CommentComponent::textEditorReturnKeyPressed(TextEditor&)
{

}
void CommentComponent::textEditorEscapeKeyPressed(TextEditor&)
{
    getObjectsHolder()->grabKeyboardFocus();
}
void CommentComponent::textEditorFocusLost(TextEditor&)
{
    Range<int> r(0,0);
    textField->setHighlightedRegion(r);

    if(data[Ids::value].toString().compare(textField->getText()) != 0)
    {
        owner.getUndoManager().beginNewTransaction("Set comment text");
        data.setProperty(Ids::value, textField->getText(), &owner.getUndoManager());
    }
    if(float(data[Ids::fontSize]) != textField->getFont().getHeight())
    {
        owner.getUndoManager().beginNewTransaction("Set comment font height");
        data.setProperty(Ids::fontSize, textField->getFont().getHeight(), &owner.getUndoManager());
    }
    if(data[Ids::commentColour].toString().compare(commentColor.toString()) != 0)
    {
        owner.getUndoManager().beginNewTransaction("Set comment text color");
        data.setProperty(Ids::commentColour, commentColor.toString(), &owner.getUndoManager());
    }

}

void CommentComponent::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged,
                                                const Identifier& property)
{
    if(treeWhosePropertyHasChanged == data && property == Ids::value)
    {
        if(data[Ids::value].toString().compare(textField->getText()) != 0)
            textField->setText(data[Ids::value].toString());
    }
    if(treeWhosePropertyHasChanged == data && property == Ids::fontSize)
    {
        if (float(data[Ids::fontSize]) != textField->getFont().getHeight())
        {
            textField->setFontHeight(data[Ids::fontSize]);
            textField->applyFont();
        }
    }
    if(treeWhosePropertyHasChanged == data && property == Ids::commentColour)
    {
        Colour textColor = Colour::fromString(data[Ids::commentColour].toString());
        if(textColor != commentColor)
        {
            commentColor = textColor;
            textField->setColour(TextEditor::textColourId, commentColor);
            textField->applyFont();
        }
    }
}
void CommentComponent::valueTreeChildAdded(ValueTree& /*parentTree*/,
                                           ValueTree& /*childWhichHasBeenAdded*/)
{

}
void CommentComponent::valueTreeChildRemoved (ValueTree& /*parentTree*/,
                                              ValueTree& /*childWhichHasBeenRemoved*/,
                                              int /*indexFromWhichChildWasRemoved*/)
{

}

void CommentComponent::valueTreeChildOrderChanged (ValueTree& /*parentTree*/,
                                                   int /*oldIndex*/, int /*newIndex*/)
{

}
void CommentComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/)
{
    
}
