/*
  ==============================================================================

    RedrawOptionsPanel.h
    Created: 18 Dec 2012 3:55:46pm
    Author:  peter

  ==============================================================================
*/

#ifndef __REDRAWOPTIONSPANEL_H_E7478E88__
#define __REDRAWOPTIONSPANEL_H_E7478E88__

namespace synthamodeler
{
//==============================================================================
/*
*/
class RedrawOptionsPanel    : public DialogWindow
{
public:
    RedrawOptionsPanel();
    ~RedrawOptionsPanel();

    void closeButtonPressed();

    /**
     * Opens the DialogWindow for editing defined variables.
     *
     * @param data 			the data of the editing object
     * @param undoManager   the UndoManager of the MDLFile consisting this object.
     */
    static void show();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RedrawOptionsPanel)
};

}

#endif  // __REDRAWOPTIONSPANEL_H_E7478E88__
