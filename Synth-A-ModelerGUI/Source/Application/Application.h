/*
  ==============================================================================

    Application.h
    Created: 11 Apr 2012 1:28:10pm
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

#ifndef __APPLICATION_H_BBAD6F5C__
#define __APPLICATION_H_BBAD6F5C__


#include "MainWindow.h"
#include "../Controller/AppController.h"
//==============================================================================
class SynthAModelerApplication  : public JUCEApplication
{
public:
    //==============================================================================
    SynthAModelerApplication();
    ~SynthAModelerApplication();

    //==============================================================================
    void initialise (const String& commandLine);
    void shutdown();
    //==============================================================================
    void systemRequestedQuit();
    void closeWindow(MainAppWindow* w);
    //==============================================================================
    const String getApplicationName();
    const String getApplicationVersion();
    bool moreThanOneInstanceAllowed();
    void anotherInstanceStarted (const String& commandLine);

    static SynthAModelerApplication* getApp();


    ApplicationCommandTarget* getNextCommandTarget()
    {
        return findFirstTargetParentComponent();

    }

    void getAllCommands (Array <CommandID>& commands);
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
    bool perform (const InvocationInfo& info);
    bool isCommandActive (const CommandID commandID);

    //==============================================================================
    class MainMenuModel  : public MenuBarModel
    {
    public:
        MainMenuModel();
        const StringArray getMenuBarNames();

        const PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& /*menuName*/);
        void menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/);
    };

    ScopedPointer<MainMenuModel> menuModel;

    bool openFile(const File& file);

    virtual MainAppWindow* createNewMainWindow();

    void creatNewMDLDocument();
    void askUserToOpenFile();
    bool closeAllDocuments (bool askUserToSave);
    void updateRecentProjectList();

    void writeToDebugConsole(const String& textToWrite);

    private:
    friend class AppController;

    OwnedArray<MainAppWindow> mainWindows;

    ScopedPointer<DebugWindow> debugWindow;
//    ScopedPointer<AppController> appController;

    MainAppWindow* getOrCreateFrontmostWindow();
    MainAppWindow* getOrCreateEmptyWindow();
    void avoidSuperimposedWindows (MainAppWindow* const mw);

    class AsyncQuitRetrier  : public Timer
    {
    public:
    	AsyncQuitRetrier();
    	void timerCallback();
    	JUCE_DECLARE_NON_COPYABLE (AsyncQuitRetrier);
    };

    //==============================================================================

};



#endif  // __APPLICATION_H_BBAD6F5C__
