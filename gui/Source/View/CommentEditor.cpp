/*
  ==============================================================================

    CommentEditor.cpp
    Created: 23 Oct 2012 11:21:14am
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

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Application/CommandIDs.h"
#include "SelectableObject.h"
#include "CommentComponent.h"

#include "CommentEditor.h"

using namespace synthamodeler;

CommentEditor::CommentEditor(CommentComponent& p, float fontHeight, Colour textColour_)
: TextEditor("CommentEditor"), parent(p)
{
//    MemoryInputStream mis(BinaryData::UbuntuR_ttf, BinaryData::UbuntuR_ttfSize, false);
//    CustomTypeface* ctf = new CustomTypeface(mis);
//    ctf->setCharacteristics("Ubuntu", 0.7f, false, false, 'a');
//    font = Font(ctf);
    font.setHeight(fontHeight);
    applyFontToAllText(font);
//    setFont(font);
    setTextToShowWhenEmpty("(comment)", Colours::grey);
    setColour(TextEditor::backgroundColourId, Colour(0x00000000));
    setColour(TextEditor::shadowColourId, Colour(0x00000000));

    setColour(TextEditor::textColourId, textColour_);

    BorderSize<int> b(0, 0, 0, 0);
    setBorder(b);

    setScrollToShowCursor(false);

    setMultiLine(true, false);
    setReturnKeyStartsNewLine(true);
}

CommentEditor::~CommentEditor()
{
}

void CommentEditor::mouseDown(const MouseEvent& e)
{
    parent.mouseDown(e);
    TextEditor::mouseDown(e);
}

void CommentEditor::mouseDrag(const MouseEvent& e)
{
    parent.mouseDrag(e);
    TextEditor::mouseDrag(e);
}

void CommentEditor::mouseUp(const MouseEvent& e)
{
    parent.mouseUp(e);
    TextEditor::mouseUp(e);
}

void CommentEditor::mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& wheel)
{
    if (e.mods.isCommandDown() && hasKeyboardFocus(true))
    {
        float fontH = font.getHeight();
        if (wheel.deltaY < 0.0f)
        {
            fontH -= 0.5f;
        }
        else if (wheel.deltaY > 0.0f)
        {
            fontH += 0.5f;
        }
        font.setHeight(fontH);
        applyFontToAllText(font);
        parent.resized();
    }
    else
    {
        TextEditor::mouseWheelMove(e, wheel);
    }
}

void CommentEditor::setFontHeight(float newHeight)
{
    font.setHeight(newHeight);
}

void CommentEditor::applyFont()
{
    applyFontToAllText(font);
}

void CommentEditor::addPopupMenuItems(PopupMenu& m, const MouseEvent* mouseClickEvent)
{
    TextEditor::addPopupMenuItems(m, mouseClickEvent);

    m.addSeparator();
    m.addItem(CommandIDs::selectTextColour, "Text Colour");
}

void CommentEditor::performPopupMenuAction(int menuItemID)
{
    switch (menuItemID)
    {
    case CommandIDs::selectTextColour:
        selectTextColour();
        break;
    default:
        TextEditor::performPopupMenuAction(menuItemID);
        break;
    }
}

void CommentEditor::selectTextColour()
{
    ColourSelector* colourSelector = new ColourSelector();
    colourSelector->setName("comment colour");
    colourSelector->setCurrentColour(findColour(TextEditor::textColourId));
    colourSelector->addChangeListener(&parent);
    colourSelector->setColour(ColourSelector::backgroundColourId, Colours::transparentBlack);
    colourSelector->setSize(300, 400);

    CallOutBox::launchAsynchronously(colourSelector, getScreenBounds(), nullptr);
}

bool CommentEditor::keyPressed(const KeyPress& key)
{
    if (hasKeyboardFocus(true) && key.getModifiers().isAltDown())
    {
        float fontH = font.getHeight();
        bool res = false;
        if (key.getKeyCode() == 'o')
        {
            fontH += 0.5f;
            res = true;
        }
        else if (key.getKeyCode() == 'i')
        {
            fontH -= 0.5f;
            res = true;
        }
        else if (key.getKeyCode() == 'p')
        {
            fontH = 16.0f;
            res = true;
        }
        if (res)
        {
            font.setHeight(fontH);
            applyFontToAllText(font);
            parent.resized();
            return res;
        }
    }
    return TextEditor::keyPressed(key);
}