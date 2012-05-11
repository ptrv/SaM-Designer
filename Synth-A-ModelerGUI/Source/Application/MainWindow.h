/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic outline for a simple desktop window.

    Author: Peter Vasil

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

#ifndef __MAINWINDOW_H_3C77CBD5__
#define __MAINWINDOW_H_3C77CBD5__

//#include "../../JuceLibraryCode/JuceHeader.h"
//#include "../View/DebugWindow.h"
#include "../Controller/AppController.h"

#include "../Application/CommonHeaders.h"
#include "../Models/MDLFile.h"

//class DebugWindow;
//class AppController;

class ContentComp;
//==============================================================================
class MainAppWindow   : public DocumentWindow,
						public ApplicationCommandTarget
{
public:
    //==============================================================================
    MainAppWindow(AppController& appController_);
    ~MainAppWindow();

    void closeButtonPressed();

    bool mdlCheckAndSave();

    void makeVisible();
    void restoreWindowPosition();
    bool closeMDLFile(MDLFile* mdlFile);
    bool closeCurrentMDLFile();

    //==============================================================================
//    const StringArray getMenuBarNames ();
//    const PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName);
//    void menuItemSelected (int menuItemID, int topLevelMenuIndex);
//==============================================================================
    ApplicationCommandTarget* getNextCommandTarget();
    void getAllCommands (Array <CommandID>& commands);
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
    bool isCommandActive (const CommandID commandID);
    bool perform (const InvocationInfo& info);

    void updateTitle (const String& documentName);

    MDLFile* getMDLFile();
    void setMDLFile(MDLFile* newMDLFile);

    // the command manager object used to dispatch command events
//    ApplicationCommandManager commandManager;
    ContentComp* getMDLFileContentComponent() const;
private:

    String getProjectWindowPosName() const
    {
        jassert (mdlController->getMDLFile() != nullptr);
        if (mdlController->getMDLFile() == nullptr)
            return String::empty;

        return "projectWindowPos_" + mdlController->getMDLFile()->getName();
    }


    AppController& appController;
    ScopedPointer<MDLController> mdlController;

    void createMDLFileContentCompIfNeeded();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainAppWindow)

};


#endif  // __MAINWINDOW_H_3C77CBD5__
