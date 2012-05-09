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

#include "MainWindow.h"
//#include "../Application/CommonHeaders.h"
#include "../View/DebugWindow.h"
#include "../Controller/AppController.h"
#include "../View/ContentComp.h"

//==============================================================================
MainAppWindow::MainAppWindow()
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                      Colours::lightgrey,
                      DocumentWindow::allButtons)
{
    debugWindow = new DebugWindow();
    appController = new AppController(*this, *debugWindow);
    ContentComp* contentComp = new ContentComp(*this, *appController.get());

    setContentOwned(contentComp, true);
//    multiDocHolder = new MultiDocHolder();
//    multiDocHolder->addDocument(contentComp, Colours::azure, true);

    setApplicationCommandManagerToWatch (&commandManager);

    setUsingNativeTitleBar(true);
    centreWithSize (getWidth(), getHeight());

    // restore the last size and position from our settings file..
	restoreWindowStateFromString (StoredSettings::getInstance()->getProps()
									.getValue ("lastMainWindowPos"));

	commandManager.registerAllCommandsForTarget(this);
    commandManager.registerAllCommandsForTarget (contentComp);
    commandManager.registerAllCommandsForTarget (JUCEApplication::getInstance());

    // this lets the command manager use keypresses that arrive in our window to send
    // out commands
    addKeyListener (commandManager.getKeyMappings());

#if JUCE_MAC
    setMacMainMenu (this);
#else
 	setMenuBar (this);
#endif

    setVisible (true);
    setWantsKeyboardFocus (false);
    setResizable (true, true); // resizability is a property of ResizableWindow

	if(StoredSettings::getInstance()->getShowCompilerWindow())
		debugWindow->makeVisible ();

//	getContentComponent()->grabKeyboardFocus();
}

MainAppWindow::~MainAppWindow()
{
#if JUCE_MAC
	setMacMainMenu (0);
#else
	setMenuBar (0);
#endif
	removeKeyListener (commandManager.getKeyMappings());

    // save the current size and position to our settings file..
    StoredSettings::getInstance()->getProps()
        .setValue ("lastMainWindowPos", getWindowStateAsString());

    clearContentComponent();

}

void MainAppWindow::closeButtonPressed()
{
//	appController->c
    JUCEApplication::getInstance()->systemRequestedQuit();
}

//==============================================================================
const StringArray MainAppWindow::getMenuBarNames()
{
    const char* const names[] = { "File", "Edit", "Insert",
    		"Generate", "Tools", "Help", nullptr };

    return StringArray (names);
}

const PopupMenu MainAppWindow::getMenuForIndex (int topLevelMenuIndex,
                                             const String& menuName)
{
//    ApplicationCommandManager* commandManager = &(commandManager);

    PopupMenu menu;

    if (topLevelMenuIndex == 0)
    {
        menu.addCommandItem (&commandManager, CommandIDs::newFile);
        menu.addCommandItem (&commandManager, CommandIDs::open);

        PopupMenu recentFiles;
        StoredSettings::getInstance()->recentFiles
        		.createPopupMenuItems (recentFiles, 100, true, true);

        menu.addSubMenu ("Open recent file", recentFiles);

        menu.addSeparator();
        menu.addCommandItem (&commandManager, CommandIDs::closeDocument);
        menu.addCommandItem (&commandManager, CommandIDs::saveDocument);
        menu.addCommandItem (&commandManager, CommandIDs::saveDocumentAs);
        menu.addSeparator();
        menu.addCommandItem(&commandManager, CommandIDs::showPrefs);

#if ! JUCE_MAC
        menu.addSeparator();
        menu.addCommandItem (&commandManager, StandardApplicationCommandIDs::quit);
#endif

    }
    else if (topLevelMenuIndex == 1)
    {
        menu.addCommandItem (&commandManager, CommandIDs::undo);
        menu.addCommandItem (&commandManager, CommandIDs::redo);
        menu.addSeparator();
        menu.addCommandItem (&commandManager, StandardApplicationCommandIDs::cut);
        menu.addCommandItem (&commandManager, StandardApplicationCommandIDs::copy);
        menu.addCommandItem (&commandManager, StandardApplicationCommandIDs::paste);
        menu.addSeparator();
        menu.addCommandItem (&commandManager, StandardApplicationCommandIDs::selectAll);
        menu.addCommandItem (&commandManager, StandardApplicationCommandIDs::deselectAll);
        menu.addSeparator();
        menu.addCommandItem (&commandManager, CommandIDs::defineVariables);
        menu.addSeparator();
        menu.addCommandItem (&commandManager, CommandIDs::segmentedConnectors);
        menu.addCommandItem (&commandManager, CommandIDs::zoomIn);
        menu.addCommandItem (&commandManager, CommandIDs::zoomOut);
        menu.addCommandItem (&commandManager, CommandIDs::zoomNormal);
        menu.addCommandItem(&commandManager, CommandIDs::reverseDirection);

    }
    else if (topLevelMenuIndex == 2)
    {
    	menu.addCommandItem(&commandManager, CommandIDs::insertMass);
    	menu.addCommandItem(&commandManager, CommandIDs::insertGround);
    	menu.addCommandItem(&commandManager, CommandIDs::insertResonator);
    	menu.addCommandItem(&commandManager, CommandIDs::insertPort);
    	menu.addSeparator();
    	menu.addCommandItem(&commandManager, CommandIDs::insertLink);
    	menu.addCommandItem(&commandManager, CommandIDs::insertTouch);
    	menu.addCommandItem(&commandManager, CommandIDs::insertPluck);
    	menu.addSeparator();
    	menu.addCommandItem(&commandManager, CommandIDs::insertAudioOutput);
    	menu.addSeparator();
    	menu.addCommandItem(&commandManager, CommandIDs::insertWaveguide);
    	menu.addCommandItem(&commandManager, CommandIDs::insertTermination);
    }
    else if (topLevelMenuIndex == 3)
    {
    	menu.addCommandItem(&commandManager, CommandIDs::generateFaust);
    	menu.addCommandItem(&commandManager, CommandIDs::generateExternal);
    }
    else if (topLevelMenuIndex == 4)
    {
    	menu.addCommandItem(&commandManager, CommandIDs::showOutputConsole);
    	menu.addCommandItem(&commandManager, CommandIDs::clearOutputConsole);
    	menu.addSeparator();
    	menu.addCommandItem(&commandManager, CommandIDs::openDataDir);
    }
    else if (topLevelMenuIndex == 5)
    {
    	menu.addCommandItem(&commandManager, CommandIDs::showHelp);
    }

    return menu;
}

void MainAppWindow::menuItemSelected (int menuItemID,
                                   int topLevelMenuIndex)
{
    if (menuItemID >= 100 && menuItemID < 200)
    {
		// open a file from the "recent files" menu
		const File file (StoredSettings::getInstance()->recentFiles
				.getFile (menuItemID - 100));

		appController->openMDL(file);
		appController->setMainWindowTitle();
		StoredSettings::getInstance()->recentFiles.addFile(file);
    }
}

//==============================================================================
ApplicationCommandTarget* MainAppWindow::getNextCommandTarget()
{
    return 0;
}

void MainAppWindow::getAllCommands (Array <CommandID>& commands)
{
    const CommandID ids[] = { CommandIDs::newFile,
            					CommandIDs::open,
            					CommandIDs::closeDocument,
            					CommandIDs::saveDocument,
            					CommandIDs::saveDocumentAs,
            					CommandIDs::showPrefs,
                                CommandIDs::generateFaust,
                                CommandIDs::generateExternal,
                                CommandIDs::showOutputConsole,
                                CommandIDs::clearOutputConsole,
                                CommandIDs::openDataDir,
                                CommandIDs::showHelp,
    };

    commands.addArray (ids, numElementsInArray (ids));
}

void MainAppWindow::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
    switch (commandID)
    {
    case CommandIDs::newFile:
        result.setInfo("New", "Create new *.mdl file.", CommandCategories::general, 0);
        result.addDefaultKeypress('n', ModifierKeys::commandModifier);
        break;
    case CommandIDs::open:
        result.setInfo ("Open", "Open *.mdl file.", CommandCategories::general, 0);
        result.addDefaultKeypress ('o', ModifierKeys::commandModifier);
        break;
    case CommandIDs::closeDocument:
        result.setInfo("Close", "Close file.", CommandCategories::general, 0);
        result.addDefaultKeypress('w', ModifierKeys::commandModifier);
        break;
    case CommandIDs::saveDocument:
        result.setInfo ("Save", "Save file.", CommandCategories::general, 0);
        result.addDefaultKeypress ('s', ModifierKeys::commandModifier);
        break;
    case CommandIDs::saveDocumentAs:
        result.setInfo ("Save as", "Save file as.", CommandCategories::general, 0);
        result.addDefaultKeypress ('s', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
        break;
    case CommandIDs::showPrefs:
    	result.setInfo ("Preferences...", "Open preferences window",
    			CommandCategories::general, 0);
    	result.addDefaultKeypress(',', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::undo:
    	result.setInfo("Undo", "Undo last edit", CommandCategories::editing,0);
    	result.addDefaultKeypress('z', ModifierKeys::commandModifier);
    	break;

    case CommandIDs::generateFaust:
    	result.setInfo("Generic Faust Code", "", CommandCategories::generation,0);
    	result.addDefaultKeypress('g', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::generateExternal:
    	result.setInfo("External Object", "", CommandCategories::generation,0);
    	result.addDefaultKeypress('e', ModifierKeys::commandModifier);
    	break;

    case CommandIDs::showOutputConsole:
    	result.setInfo("Show compiler window", "", CommandCategories::tools,0);
    	result.setTicked(StoredSettings::getInstance()->getShowCompilerWindow());
    	result.addDefaultKeypress('k', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::clearOutputConsole:
    	result.setInfo("Clear compiler window", "", CommandCategories::tools,0);
    	result.addDefaultKeypress('k', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
    	break;

    case CommandIDs::openDataDir:
    	result.setInfo("Open data dir", "", CommandCategories::tools, 0);
    	result.addDefaultKeypress('l', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::showHelp:
    	result.setInfo("Online Help", "Open online help in web browser.", CommandCategories::help, 0);
    	break;
    default:
        break;
    };
}

bool MainAppWindow::isCommandActive (const CommandID commandID)
{
    return true;
}

bool MainAppWindow::perform (const InvocationInfo& info)
{
    return appController->menuItemWasClicked(info.commandID);
}

bool MainAppWindow::mdlCheckAndSave()
{
	return appController->mdlCheckAndSave();
}
