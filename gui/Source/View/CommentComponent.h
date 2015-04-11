/*
  ==============================================================================

    CommentComponent.h
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

#ifndef __COMMENTCOMPONENT_H_FE5F7A49__
#define __COMMENTCOMPONENT_H_FE5F7A49__


#include "JuceHeader.h"

#include "BaseObjectComponent.h"
#include "SelectableObject.h"

namespace synthamodeler
{
class ObjController;
class CommentEditor;

class CommentComponent : public BaseObjectComponent,
                         public ChangeListener,
                         public ChangeBroadcaster,
                         public SelectableObject,
                         public TextEditor::Listener,
                         public ValueTree::Listener
{
public:
    CommentComponent(ObjController& owner_, ValueTree data_);
    ~CommentComponent();

    bool hitTest(int x, int y);
    void resized();
    void paint(Graphics& g);
    void mouseDown(const MouseEvent& e);
    void mouseDrag(const MouseEvent& e);
    void mouseUp(const MouseEvent& e);
    void update();

    void setSelected(bool shouldBeSelected);

    void toggleSelected();

    void setPosition(Point<int> newPos, bool undoable);
    void setActualPosition(Point<int> pos);

    Point<int> getActualPos() const { return actualPos; }

    void textEditorTextChanged(TextEditor&);
    void textEditorReturnKeyPressed(TextEditor&);
    void textEditorEscapeKeyPressed(TextEditor&);
    void textEditorFocusLost(TextEditor&);

    void valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged,
                                  const Identifier& property);
    void valueTreeChildAdded(ValueTree& parentTree,
                             ValueTree& childWhichHasBeenAdded);
    void valueTreeChildRemoved(ValueTree& parentTree,
                               ValueTree& childWhichHasBeenRemoved,
                               int indexFromWhichChildWasRemoved);
    void valueTreeChildOrderChanged(ValueTree& parentTree,
                                    int oldIndex, int newIndex);
    void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged);

    void setOldPos(const Point<int>& p) { oldPos = p; }
    const Point<int>& getOldPos() const { return oldPos; }


private:
    void changeListenerCallback (ChangeBroadcaster*);

    CommentEditor* textField;
    DropShadowEffect shadow;

    Point<int> actualPos;
    Point<int> oldPos;

    bool dragging, mouseDownSelectStatus;

    ChangeBroadcaster selfChangeListenerList;

    Colour commentColor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CommentComponent);
};
}


#endif  // __COMMENTCOMPONENT_H_FE5F7A49__
