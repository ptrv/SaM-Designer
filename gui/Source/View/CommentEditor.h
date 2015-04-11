/*
  ==============================================================================

    CommentEditor.h
    Created: 22 Oct 2012 4:40:15pm
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

#ifndef __COMMENTEDITOR_H_D81136D5__
#define __COMMENTEDITOR_H_D81136D5__


#include "JuceHeader.h"

namespace synthamodeler
{
class CommentComponent;
class CommentEditor : public TextEditor
{
public:
    CommentEditor(CommentComponent& p, float fontHeight, Colour textColour_);
    virtual ~CommentEditor();

    void mouseDown(const MouseEvent& e);
    void mouseDrag(const MouseEvent& e);
    void mouseUp(const MouseEvent& e);
    void mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& wheel);
    void setFontHeight(float newHeight);
    void applyFont();
    void addPopupMenuItems (PopupMenu& m, const MouseEvent* mouseClickEvent);
    void performPopupMenuAction (int menuItemID);
    void selectTextColour();
    bool keyPressed(const KeyPress& key);

    static int numCommentEditor;
private:
    CommentComponent& parent;
    Font font;
};
}


#endif  // __COMMENTEDITOR_H_D81136D5__
