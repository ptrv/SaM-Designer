/*
  ==============================================================================

    Application.cpp
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

#include "CommonHeaders.h"
#include "../View/PrefsPanel.h"
#include "../View/DebugWindow.h"
#include "MainWindow.h"
#include "../Models/MDLFile.h"

#include "Application.h"

#if UNIT_TESTS
#include "../../Testsuite/TestRunner.h"
#endif

//==============================================================================

SynthAModelerApplication::SynthAModelerApplication()
{
}

SynthAModelerApplication::~SynthAModelerApplication()
{
}

//==============================================================================
void SynthAModelerApplication::initialise (const String& commandLine)
{
#if UNIT_TESTS
	if(commandLine.contains("--test"))
	{
		TestRunner::doTests();
		quit();
		return;
	}
#endif

	// Do your application's initialisation code here
	Logger::setCurrentLogger(Utils::getLogger(), true);

	commandManager = new ApplicationCommandManager();
	commandManager->registerAllCommandsForTarget (this);

	menuModel = new MainMenuModel();

	debugWindow = new DebugWindow();
//	commandManager->registerAllCommandsForTarget(debugWindow);
    Array<File> mdls (StoredSettings::getInstance()->getLastFiles());

    for (int i = 0; i < mdls.size(); ++ i)
        openFile (mdls.getReference(i));

    if (mainWindows.size() == 0)
        createNewMainWindow()->makeVisible();

#if JUCE_MAC
	MenuBarModel::setMacMainMenu (menuModel);
#endif

//	if(StoredSettings::getInstance()->getShowCompilerWindow())
	{
		debugWindow->printWelcomeMessage();
		debugWindow->makeVisible ();
		getOrCreateFrontmostWindow()->toFront(true);

	}

}

void SynthAModelerApplication::shutdown()
{
	// Do your application's shutdown code here..

#if JUCE_MAC
	MenuBarModel::setMacMainMenu (nullptr);
#endif
	menuModel = nullptr;

	debugWindow = nullptr;

	StoredSettings::deleteInstance();
	mainWindows.clear();

	commandManager = nullptr;

	Logger::setCurrentLogger(nullptr, true);
}

//==============================================================================
void SynthAModelerApplication::systemRequestedQuit()
{
    if (Utils::cancelAnyModalComponents())
    {
        new AsyncQuitRetrier();
        return;
    }

    while (mainWindows.size() > 0)
    {
        if (! mainWindows[0]->closeCurrentMDLFile())
            return;

        mainWindows.remove (0);
    }

    quit();

}

void SynthAModelerApplication::closeWindow (MainAppWindow* w)
{
    jassert (mainWindows.contains (w));
    mainWindows.removeObject (w);

//#if ! JUCE_MAC
    if (mainWindows.size() == 0)
        systemRequestedQuit();
//#endif

    updateRecentProjectList();
}

bool SynthAModelerApplication::isCommandActive (const CommandID commandID)
{
    return true;
}


//==============================================================================
const String SynthAModelerApplication::getApplicationName()
{
	return "Synth-A-Modeler";
}

const String SynthAModelerApplication::getApplicationVersion()
{
	return ProjectInfo::versionString;
}

bool SynthAModelerApplication::moreThanOneInstanceAllowed()
{
	return false;
}

void SynthAModelerApplication::anotherInstanceStarted (const String& commandLine)
{

}

SynthAModelerApplication* SynthAModelerApplication::getApp()
{
    return dynamic_cast<SynthAModelerApplication*> (JUCEApplication::getInstance());
}


void SynthAModelerApplication::getAllCommands (Array <CommandID>& commands)
{
    JUCEApplication::getAllCommands (commands);

    const CommandID ids[] = { CommandIDs::newFile,
                              CommandIDs::open,
                              CommandIDs::showPrefs,
                              CommandIDs::showOutputConsole,
                              CommandIDs::clearOutputConsole,
                              CommandIDs::openDataDir,
                              CommandIDs::showHelp,

    };

    commands.addArray (ids, numElementsInArray (ids));
}

void SynthAModelerApplication::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
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
    case CommandIDs::showPrefs:
    	result.setInfo ("Preferences...", "Open preferences window",
    			CommandCategories::general, 0);
    	result.addDefaultKeypress(',', ModifierKeys::commandModifier);
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
    case CommandIDs::showOutputConsole:
    	result.setInfo("Show compiler window", "", CommandCategories::tools,0);
//    	result.setTicked(StoredSettings::getInstance()->getShowCompilerWindow());
    	result.addDefaultKeypress('k', ModifierKeys::commandModifier);
    	break;
    default:
        JUCEApplication::getCommandInfo (commandID, result);
        break;
    }
}

bool SynthAModelerApplication::perform (const InvocationInfo& info)
{
    switch (info.commandID)
    {
    case CommandIDs::newFile:
    	creatNewMDLDocument();
    	break;
    case CommandIDs::open:
    	askUserToOpenFile();
    	break;
    case CommandIDs::showPrefs:
    	PrefsPanel::show();
    	break;

    case CommandIDs::showOutputConsole:
    	debugWindow->toFront(false);
    	getOrCreateFrontmostWindow()->toFront(true);
    	break;
    case CommandIDs::clearOutputConsole:
    	debugWindow->clear();
    	break;
    case CommandIDs::openDataDir:
    	Utils::openDataDir();
    	break;
    case CommandIDs::showHelp:
    	Utils::openHelpUrl();
    	break;
    default:
    	return JUCEApplication::perform (info);
    }

    return true;
}

//==============================================================================

void SynthAModelerApplication::creatNewMDLDocument()
{
	MainAppWindow* mw = getOrCreateEmptyWindow();
    mw->makeVisible();
	avoidSuperimposedWindows (mw);
}

void SynthAModelerApplication::askUserToOpenFile()
{
    FileChooser fc ("Open MDL File");

    if (fc.browseForFileToOpen())
        openFile (fc.getResult());
}

void SynthAModelerApplication::updateRecentProjectList()
{
    Array<File> projects;

    for (int i = 0; i < mainWindows.size(); ++i)
    {
        MainAppWindow* mw = mainWindows[i];

        if (mw != nullptr && mw->getMDLFile() != nullptr)
            projects.add (mw->getMDLFile()->getFile());
    }

    StoredSettings::getInstance()->setLastFiles(projects);
}

MainAppWindow* SynthAModelerApplication::createNewMainWindow()
{
	ScopedPointer <MDLFile> newMDL (new MDLFile());

    MainAppWindow* mw = new MainAppWindow();
    mw->setMDLFile(newMDL.release());
    mainWindows.add (mw);
    mw->restoreWindowPosition();
    avoidSuperimposedWindows (mw);
    return mw;
}

MainAppWindow* SynthAModelerApplication::getOrCreateFrontmostWindow()
{
	if (mainWindows.size() == 0)
		return createNewMainWindow();

	for (int i = Desktop::getInstance().getNumComponents(); --i >= 0;)
	{
		MainAppWindow* mw = dynamic_cast <MainAppWindow*> (Desktop::getInstance().getComponent (i));
		if (mainWindows.contains (mw))
			return mw;
	}

	return mainWindows.getLast();
}

MainAppWindow* SynthAModelerApplication::getOrCreateEmptyWindow()
{
	if (mainWindows.size() == 0)
		return createNewMainWindow();

	for (int i = Desktop::getInstance().getNumComponents(); --i >= 0;)
	{
		MainAppWindow* mw = dynamic_cast <MainAppWindow*> (Desktop::getInstance().getComponent (i));
		if (mainWindows.contains (mw) && mw->getMDLFile()->isEmpty())
			return mw;
	}

	return createNewMainWindow();
}

void SynthAModelerApplication::avoidSuperimposedWindows (MainAppWindow* const mw)
{
	for (int i = mainWindows.size(); --i >= 0;)
	{
		MainAppWindow* const other = mainWindows.getUnchecked(i);

		const Rectangle<int> b1 (mw->getBounds());
		const Rectangle<int> b2 (other->getBounds());

		if (mw != other
			 && std::abs (b1.getX() - b2.getX()) < 3
			 && std::abs (b1.getY() - b2.getY()) < 3
			 && std::abs (b1.getRight() - b2.getRight()) < 3
			 && std::abs (b1.getBottom() - b2.getBottom()) < 3)
		{
			int dx = 40, dy = 30;

			if (b1.getCentreX() >= mw->getScreenBounds().getCentreX())   dx = -dx;
			if (b1.getCentreY() >= mw->getScreenBounds().getCentreY())   dy = -dy;

			mw->setBounds (b1.translated (dx, dy));
		}
	}
}

bool SynthAModelerApplication::openFile(const File& file)
{
    for (int j = mainWindows.size(); --j >= 0;)
    {
        if (mainWindows.getUnchecked(j)->getMDLFile() != nullptr
             && mainWindows.getUnchecked(j)->getMDLFile()->getFile() == file)
        {
            mainWindows.getUnchecked(j)->toFront (true);
            return true;
        }
    }
    if (file.hasFileExtension (MDLFile::mdlFileExtension))
    {
        ScopedPointer <MDLFile> newMDL (new MDLFile());

        if (newMDL->loadFrom (file, true))
        {
            MainAppWindow* w = getOrCreateEmptyWindow();
            w->setMDLFile(newMDL.release());
            w->makeVisible();
            avoidSuperimposedWindows (w);
            return true;
        }
    }
    else if (file.exists())
    {
    	Alerts::wrongFileType();
    }
    return false;
}
//==============================================================================

SynthAModelerApplication::AsyncQuitRetrier::AsyncQuitRetrier()
{
	startTimer (500);
}

void SynthAModelerApplication::AsyncQuitRetrier::timerCallback()
{
	stopTimer();
	delete this;

	if (getApp() != nullptr)
		getApp()->systemRequestedQuit();
}

//==============================================================================

SynthAModelerApplication::MainMenuModel::MainMenuModel()
{
	setApplicationCommandManagerToWatch (commandManager);
}

const StringArray SynthAModelerApplication::MainMenuModel::getMenuBarNames()
{
    const char* const names[] = { "File", "Edit", "Insert",
    		"Generate", "Tools", "Help", nullptr };

    return StringArray (names);
}

const PopupMenu SynthAModelerApplication::MainMenuModel::getMenuForIndex (int topLevelMenuIndex, const String& /*menuName*/)
{
	PopupMenu menu;
    if (topLevelMenuIndex == 0)
    {
        menu.addCommandItem (commandManager, CommandIDs::newFile);
        menu.addCommandItem (commandManager, CommandIDs::open);

        PopupMenu recentFiles;
        StoredSettings::getInstance()->recentFiles
        		.createPopupMenuItems (recentFiles, 100, true, true);

        menu.addSubMenu ("Open recent file", recentFiles);

        menu.addSeparator();
        menu.addCommandItem (commandManager, CommandIDs::closeDocument);
        menu.addCommandItem (commandManager, CommandIDs::saveDocument);
        menu.addCommandItem (commandManager, CommandIDs::saveDocumentAs);
        menu.addSeparator();
        menu.addCommandItem(commandManager, CommandIDs::showPrefs);

#if ! JUCE_MAC
        menu.addSeparator();
        menu.addCommandItem (commandManager, StandardApplicationCommandIDs::quit);
#endif

    }
    else if (topLevelMenuIndex == 1)
    {
        menu.addCommandItem (commandManager, CommandIDs::undo);
        menu.addCommandItem (commandManager, CommandIDs::redo);
        menu.addSeparator();
        menu.addCommandItem (commandManager, StandardApplicationCommandIDs::cut);
        menu.addCommandItem (commandManager, StandardApplicationCommandIDs::copy);
        menu.addCommandItem (commandManager, StandardApplicationCommandIDs::paste);
        menu.addSeparator();
        menu.addCommandItem (commandManager, StandardApplicationCommandIDs::selectAll);
        menu.addCommandItem (commandManager, StandardApplicationCommandIDs::deselectAll);
        menu.addSeparator();
        menu.addCommandItem (commandManager, CommandIDs::defineVariables);
        menu.addSeparator();
        menu.addCommandItem (commandManager, CommandIDs::segmentedConnectors);
        menu.addCommandItem (commandManager, CommandIDs::zoomIn);
        menu.addCommandItem (commandManager, CommandIDs::zoomOut);
        menu.addCommandItem (commandManager, CommandIDs::zoomNormal);
        menu.addCommandItem(commandManager, CommandIDs::reverseDirection);

    }
    else if (topLevelMenuIndex == 2)
    {
    	menu.addCommandItem(commandManager, CommandIDs::insertMass);
    	menu.addCommandItem(commandManager, CommandIDs::insertGround);
    	menu.addCommandItem(commandManager, CommandIDs::insertResonator);
    	menu.addCommandItem(commandManager, CommandIDs::insertPort);
    	menu.addSeparator();
    	menu.addCommandItem(commandManager, CommandIDs::insertLink);
    	menu.addCommandItem(commandManager, CommandIDs::insertTouch);
    	menu.addCommandItem(commandManager, CommandIDs::insertPluck);
    	menu.addSeparator();
    	menu.addCommandItem(commandManager, CommandIDs::insertAudioOutput);
    	menu.addSeparator();
    	menu.addCommandItem(commandManager, CommandIDs::insertWaveguide);
    	menu.addCommandItem(commandManager, CommandIDs::insertTermination);
    }
    else if (topLevelMenuIndex == 3)
    {
    	menu.addCommandItem(commandManager, CommandIDs::generateFaust);
    	menu.addCommandItem(commandManager, CommandIDs::generateExternal);
    }
    else if (topLevelMenuIndex == 4)
    {
    	menu.addCommandItem(commandManager, CommandIDs::showOutputConsole);
    	menu.addCommandItem(commandManager, CommandIDs::clearOutputConsole);
    	menu.addSeparator();
    	menu.addCommandItem(commandManager, CommandIDs::openDataDir);
    }
    else if (topLevelMenuIndex == 5)
    {
    	menu.addCommandItem(commandManager, CommandIDs::showHelp);
    }

	return menu;
}

void SynthAModelerApplication:: MainMenuModel::menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/)
{
    if (menuItemID >= 100 && menuItemID < 200)
    {
		// open a file from the "recent files" menu
		const File file (StoredSettings::getInstance()->recentFiles
				.getFile (menuItemID - 100));

		getApp()->openFile(file);
		StoredSettings::getInstance()->recentFiles.addFile(file);
    }

}

void SynthAModelerApplication::writeToDebugConsole(const String& title, const String& textToWrite)
{
	if(textToWrite.compare("") != 0)
	{
		debugWindow->printHeader();
		debugWindow->addText(title);
		debugWindow->addText(textToWrite);
		debugWindow->addNewLine();
	}
	else
	{
		debugWindow->addText("Nothing...\n\n");
	}

}
