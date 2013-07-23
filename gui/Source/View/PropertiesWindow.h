/*
  ==============================================================================

    PropertiesWindow.h
    Created: 23 Jul 2013 10:12:43pm
    Author:  peter

  ==============================================================================
 */

#ifndef __PROPERTIESWINDOW_H_FBFE568F__
#define __PROPERTIESWINDOW_H_FBFE568F__

#include "SelectableObject.h"


namespace synthamodeler
{
class ContentComp;
class PropertiesWindow : public DocumentWindow,
                         public FocusChangeListener,
                         public ChangeListener
{
public:
    PropertiesWindow();
    ~PropertiesWindow();

    void makeVisible(bool shouldBeVisible=true);

    void globalFocusChanged (Component* focusedComponent);

    void changeListenerCallback(ChangeBroadcaster* source);
    
    void closeButtonPressed();
private:
    ContentComp* currentContentComp;
    SelectedItemSet<SelectableObject*>* currentSelection;

    void updateProperties();
};

}


#endif  // __PROPERTIESWINDOW_H_FBFE568F__
