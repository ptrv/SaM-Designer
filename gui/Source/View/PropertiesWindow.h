/*
  ==============================================================================

    PropertiesWindow.h
    Created: 23 Jul 2013 10:12:43pm
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

#ifndef __PROPERTIESWINDOW_H_FBFE568F__
#define __PROPERTIESWINDOW_H_FBFE568F__

namespace synthamodeler
{
class ContentComp;
class SelectableObject;
class MainAppWindow;

class PropertiesWindow : public DocumentWindow,
                         public ChangeListener,
                         public ValueTree::Listener,
                         public ApplicationCommandTarget
{
public:
    PropertiesWindow();
    ~PropertiesWindow();

    void makeVisible(bool shouldBeVisible=true);

    void changeListenerCallback(ChangeBroadcaster* source);
    
    void closeButtonPressed();

    bool keyPressed(const KeyPress& kp);

    void valueTreePropertyChanged (ValueTree& tree, const Identifier& property);
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded);
    void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved);
    void valueTreeChildOrderChanged(ValueTree& parentTree, int oldIndex, int newIndex);
    void valueTreeParentChanged (ValueTree& tree);

    void setCurrentActiveWindow(MainAppWindow& maw);

    ApplicationCommandTarget* getNextCommandTarget();
    void getAllCommands (Array <CommandID>& commands);
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
    bool perform (const InvocationInfo& info);

    void showEmptyComponent(const String& textToShow=String::empty);

private:
    ContentComp* currentContentComp;
    SelectedItemSet<SelectableObject*>* currentSelection;

    void updateProperties();

    void mdlChanged();
};

}


#endif  // __PROPERTIESWINDOW_H_FBFE568F__
