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

#include "Application.h"

#include "CommonHeaders.h"

#include "MainWindow.h"
#include "SAMLookAndFeel.h"
#include "CommandLine.h"

#include "Models/MDLFile.h"

#include "View/PrefsPanel.h"
#include "View/PostWindow.h"
#include "View/PropertiesWindow.h"
#include "View/ObjectsHolder.h"


using namespace synthamodeler;

ScopedPointer<PropertiesWindow> synthamodeler::propertiesWindow;
ScopedPointer<PostWindowController> synthamodeler::postWindow;

#if UNIT_TESTS
#include "TestRunner.h"
#endif

static const int snapSizes[] = { 2, 3, 4, 5, 6, 8, 10, 12, 16, 20, 24, 32 };
//==============================================================================

SAMApplication::SAMApplication()
: isRunningCommandLine(false)
{
}

SAMApplication::~SAMApplication()
{
}

//==============================================================================
void SAMApplication::initialise (const String& commandLine)
{
#if UNIT_TESTS
	if(commandLine.contains("--test"))
    {
#if JUCE_MAC
        Process::setDockIconVisible(false);
#endif
        TestRunner::doTests();
		quit();
		return;
	}
#endif

    Utils::setLocale();

	// Do your application's initialisation code here
	samLookAndFeel = new SAMLookAndFeel();
	LookAndFeel::setDefaultLookAndFeel(samLookAndFeel);

    if (commandLine.isNotEmpty())
    {
        const int appReturnCode = performCommandLine(commandLine);

        if (appReturnCode != commandLineNotPerformed)
        {
            isRunningCommandLine = true;
            setApplicationReturnValue(appReturnCode);
            quit();
            return;
        }
    }

    if( StoredSettings::getInstance()->getIsLoggingOn() )
    {
#ifdef DEBUG
        samLogger = Utils::getLogger();
        Logger::setCurrentLogger(samLogger);
#endif
    }

    if (sendCommandLineToPreexistingInstance())
    {
        DBG("Another instance is running - quitting...");
        shutdown();
        quit();
        return;
    }

	commandManager = new ApplicationCommandManager();
	commandManager->registerAllCommandsForTarget (this);

	menuModel = new MainMenuModel();

	postWindow = new PostWindowController();

    propertiesWindow = new PropertiesWindow();

    if (StoredSettings::getInstance()->getReopenLastModelsOnStartup())
    {
        Array<File> mdls (StoredSettings::getInstance()->getLastFiles());

        for (int i = 0; i < mdls.size(); ++ i)
        {
            openFile (mdls.getReference(i));
        }
    }

    // get all exporters
    StoredSettings::getInstance()->initExporters();

    // create default values properties file if not present
    StoredSettings::getInstance()->initDefaultPropertiesFile();

    if (mainWindows.size() == 0)
        createNewMainWindow()->makeVisible();

#if JUCE_MAC
	MenuBarModel::setMacMainMenu (menuModel);
#endif

    postWindow->init();

	getOrCreateFrontmostWindow()->toFront(true);

//    propertiesWindow->makeVisible();
}

void SAMApplication::shutdown()
{
	// Do your application's shutdown code here..

#if JUCE_MAC
	MenuBarModel::setMacMainMenu (nullptr);
#endif
	menuModel = nullptr;

	postWindow = nullptr;
    propertiesWindow = nullptr;

	StoredSettings::deleteInstance();
	mainWindows.clear();

	commandManager = nullptr;

    Logger::setCurrentLogger(nullptr);
#ifdef DEBUG
    samLogger = nullptr;
#endif

	ResourceLoader::deleteInstance();

	samLookAndFeel = nullptr;
}

//==============================================================================
void SAMApplication::systemRequestedQuit()
{
    if (Utils::cancelAnyModalComponents())
    {
        new AsyncQuitRetrier();
        return;
    }

    activeWindowsList.clearQuick();
    while (mainWindows.size() > 0)
    {
        if (! mainWindows[0]->closeCurrentMDLFile())
            return;

        mainWindows.remove (0);
    }

    quit();

}

void SAMApplication::closeWindow (MainAppWindow* w)
{
    jassert (mainWindows.contains (w));
    int idx = activeWindowsList.indexOf(w);
    activeWindowsList.remove(idx);
    mainWindows.removeObject (w);

//#if ! JUCE_MAC
//    if (mainWindows.size() == 0)
//        systemRequestedQuit();
//#endif

    updateRecentProjectList();
    updateOpenFilesList();
}

bool SAMApplication::isCommandActive (const CommandID /*commandID*/)
{
    return true;
}


//==============================================================================
const String SAMApplication::getApplicationName()
{
	return "Synth-A-Modeler Designer";
}

const String SAMApplication::getApplicationVersion()
{
	return ProjectInfo::versionString;
}

bool SAMApplication::moreThanOneInstanceAllowed()
{
	return true;
}

void SAMApplication::anotherInstanceStarted (const String& commandLine)
{
    File f;
    if(File::isAbsolutePath(commandLine))
        f = commandLine;
    else
        f = File::getCurrentWorkingDirectory().getChildFile(commandLine);

    if(f.existsAsFile() && f.hasFileExtension(MDLFile::mdlFileExtension))
        openFile(f);
}

SAMApplication* SAMApplication::getApp()
{
    return dynamic_cast<SAMApplication*> (JUCEApplication::getInstance());
}


void SAMApplication::getAllCommands (Array <CommandID>& commands)
{
    JUCEApplication::getAllCommands (commands);

    const CommandID ids[] = { CommandIDs::newFile,
                              CommandIDs::open,
                              CommandIDs::showPrefs,
                              CommandIDs::showPostWindow,
                              CommandIDs::clearOutputConsole,
                              CommandIDs::openDataDir,
                              CommandIDs::showHelp,
                              CommandIDs::showPropertiesWindow,
                              CommandIDs::propertiesWindowOnTop,
                              CommandIDs::showPreviousWindow,
                              CommandIDs::showNextWindow,
    };

    commands.addArray (ids, numElementsInArray (ids));
}

void SAMApplication::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    switch (commandID)
    {
    case CommandIDs::newFile:
        result.setInfo(TRANS("New"), TRANS("Create new *.mdl file."),
                       CommandCategories::general, 0);
        result.addDefaultKeypress('n', ModifierKeys::commandModifier);
        break;
    case CommandIDs::open:
        result.setInfo(TRANS("Open"), TRANS("Open *.mdl file."),
                       CommandCategories::general, 0);
        result.addDefaultKeypress ('o', ModifierKeys::commandModifier);
        break;
    case CommandIDs::showPrefs:
        result.setInfo(TRANS("Preferences..."),
                       TRANS("Open preferences window"),
                       CommandCategories::general, 0);
    	result.addDefaultKeypress(',', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::clearOutputConsole:
        result.setInfo(TRANS("Clear Output Window"),
                       TRANS("Delete text in the output window"),
                       CommandCategories::tools, 0);
        result.addDefaultKeypress('k', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
    	break;
    case CommandIDs::openDataDir:
        result.setInfo(TRANS("Open Data Directory"),
                       TRANS("Open data directory in file manager"),
                       CommandCategories::tools, 0);
        result.addDefaultKeypress('l', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::showHelp:
        result.setInfo(TRANS("Online Help"),
                       TRANS("Open online help in web browser."),
                       CommandCategories::help, 0);
        break;
    case CommandIDs::showPostWindow:
        result.setInfo(TRANS("Toggle Post Window"),
                       TRANS("Show/hide post window"),
                       CommandCategories::tools, 0);
    	result.addDefaultKeypress('k', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::showPropertiesWindow:
        result.setInfo(TRANS("Show Properties Window"),
                       TRANS("Show properties window"),
                       CommandCategories::windows, 0);
    	result.addDefaultKeypress('i', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::propertiesWindowOnTop:
        result.setInfo(TRANS("Properties Window Always On Top"),
                       TRANS("Toggle always on top"),
                       CommandCategories::windows, 0);
        result.setTicked(StoredSettings::getInstance()->getIsPropertiesWindowAlwaysOnTop());
        break;
    case CommandIDs::showPreviousWindow:
        result.setInfo(TRANS("Previous Window"),
                       TRANS("Display previous window"),
                       CommandCategories::windows, 0);
#ifdef JUCE_MAC
        result.addDefaultKeypress(KeyPress::leftKey, ModifierKeys::commandModifier | ModifierKeys::altModifier);
#else
        result.addDefaultKeypress(KeyPress::pageUpKey, ModifierKeys::commandModifier);
#endif
        break;
    case CommandIDs::showNextWindow:
        result.setInfo(TRANS("Next Window"),
                       TRANS("Display next window"),
                       CommandCategories::windows, 0);
#ifdef JUCE_MAC
        result.addDefaultKeypress(KeyPress::rightKey, ModifierKeys::commandModifier | ModifierKeys::altModifier);
#else
        result.addDefaultKeypress(KeyPress::pageDownKey, ModifierKeys::commandModifier);
#endif
        break;
    default:
        JUCEApplication::getCommandInfo (commandID, result);
        break;
    }
}

bool SAMApplication::perform (const InvocationInfo& info)
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

    case CommandIDs::showPostWindow:
        togglePostWindowFocus();
    	break;
    case CommandIDs::clearOutputConsole:
        postWindow->clear();
    	break;
    case CommandIDs::openDataDir:
    	Utils::openDataDir();
    	break;
    case CommandIDs::showHelp:
    	Utils::openHelpUrl();
    	break;
    case CommandIDs::showPropertiesWindow:
        togglePropertiesWindow();
    	break;
    case CommandIDs::propertiesWindowOnTop:
        togglePropertiesWindowAlwaysOnTop();
        break;
    case CommandIDs::showPreviousWindow:
        switchToPreviousWindow();
        break;
    case CommandIDs::showNextWindow:
        switchToNextWindow();
        break;
    default:
    	return JUCEApplication::perform (info);
    }

    return true;
}

//==============================================================================

void SAMApplication::creatNewMDLDocument()
{
	MainAppWindow* mw = getOrCreateEmptyWindow();
    mw->makeVisible();
	avoidSuperimposedWindows (mw);
}

void SAMApplication::askUserToOpenFile()
{
    File lastOpenDir = StoredSettings::getInstance()->getLastOpenDirectory();

    FileChooser fc(TRANS("Open MDL File"), lastOpenDir, "*.mdl;*.mdlx");

    if (fc.browseForFileToOpen())
    {
        File fileToOpen = fc.getResult();
        openFile(fileToOpen);
        StoredSettings::getInstance()->setLastOpenDirectory(
            fileToOpen.getParentDirectory());
    }
}

void SAMApplication::updateRecentProjectList()
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

void SAMApplication::togglePostWindowFocus()
{
    if(! postWindow->isActiveWindow())
        postWindow->toFront();
    else if(MainAppWindow* mw = getFrontmostWindow())
        mw->toFront(true);
}

MainAppWindow* SAMApplication::getFrontmostWindow()
{
	if (mainWindows.size() == 0)
		return nullptr;

	for (int i = Desktop::getInstance().getNumComponents(); --i >= 0;)
	{
		MainAppWindow* mw = dynamic_cast <MainAppWindow*> (Desktop::getInstance().getComponent (i));
		if (mainWindows.contains (mw))
			return mw;
	}

	return mainWindows.getLast();
}

MainAppWindow* SAMApplication::createNewMainWindow()
{
	ScopedPointer <MDLFile> newMDL (new MDLFile());

    MainAppWindow* mw = new MainAppWindow();
    mw->setMDLFile(newMDL.release());
    mainWindows.add (mw);
    activeWindowsList.insert(0, mw);
    mw->restoreWindowPosition();
    avoidSuperimposedWindows (mw);
    addToFileList(mw->getMDLFile()->getFile());
    mw->updateTitle();
    return mw;
}

MainAppWindow* SAMApplication::getOrCreateFrontmostWindow()
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

MainAppWindow* SAMApplication::getOrCreateEmptyWindow()
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

void SAMApplication::avoidSuperimposedWindows (MainAppWindow* const mw)
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

bool SAMApplication::openFile(const File& file)
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
            addToFileList(w->getMDLFile()->getFile());
            w->updateTitle();
            return true;
        }
    }
    else if (file.exists())
    {
    	Alerts::wrongFileType();
    }
    return false;
}

void SAMApplication::addToFileList(const File& newFile)
{
    openFilesList.set(newFile.getFullPathName(), newFile.getFileName());
    updateOpenFilesList();
    for (int i = 0; i < mainWindows.size(); ++i)
        mainWindows[i]->updateTitle();
}

void SAMApplication::updateOpenFilesList()
{
    openFilesList.clear();
    for (const auto win : mainWindows)
    {
        File tmpMdl = win->getMDLFile()->getFile();
        openFilesList.set(tmpMdl.getFullPathName(), tmpMdl.getFileName());
    }

    Utils::uniquifyPaths(openFilesList);
}

String SAMApplication::getUniqueMDLPath(const String& mdlPath)
{
    return openFilesList[mdlPath];
}

void SAMApplication::removeFromFileList(const File& f)
{
    openFilesList.remove(f.getFullPathName());
    updateOpenFilesList();
    for (int i = 0; i < mainWindows.size(); ++i)
        mainWindows[i]->updateTitle();
}

//==============================================================================

SAMApplication::AsyncQuitRetrier::AsyncQuitRetrier()
{
	startTimer (500);
}

void SAMApplication::AsyncQuitRetrier::timerCallback()
{
	stopTimer();
	delete this;

	if (getApp() != nullptr)
		getApp()->systemRequestedQuit();
}

//==============================================================================

SAMApplication::MainMenuModel::MainMenuModel()
{
	setApplicationCommandManagerToWatch (commandManager);
}

StringArray SAMApplication::MainMenuModel::getMenuBarNames()
{
    const char* const names[] = {"File", "Edit", "Insert", "Generate",
        "Tools", "View", "Window", "Help", nullptr};

    return StringArray (names);
}

PopupMenu SAMApplication::MainMenuModel::getMenuForIndex (int topLevelMenuIndex, const String& /*menuName*/)
{
	PopupMenu menu;
    if (topLevelMenuIndex == 0)
    {
        menu.addCommandItem (commandManager, CommandIDs::newFile);
        menu.addCommandItem (commandManager, CommandIDs::open);

        PopupMenu recentFiles;
        StoredSettings::getInstance()->recentFiles
            .createPopupMenuItems(recentFiles, recentProjectsBaseID, true, true);

        menu.addSubMenu(TRANS("Open Recent File"), recentFiles);

        menu.addSeparator();
        menu.addCommandItem (commandManager, CommandIDs::closeDocument);
        menu.addCommandItem (commandManager, CommandIDs::saveDocument);
        menu.addCommandItem (commandManager, CommandIDs::saveDocumentAs);
        menu.addSeparator();
        menu.addCommandItem (commandManager, CommandIDs::showMDLProperties);
        menu.addSeparator();
        menu.addCommandItem (commandManager, CommandIDs::saveDocumentAsImage);
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
        menu.addCommandItem (commandManager, CommandIDs::defineFaustcode);
        menu.addSeparator();
        menu.addCommandItem(commandManager, CommandIDs::reverseDirection);
        menu.addSeparator();
        menu.addCommandItem(commandManager, CommandIDs::tidyObjects);
        menu.addSeparator();

        PopupMenu redrawModelMenu;
        redrawModelMenu.addCommandItem(commandManager, CommandIDs::toggleRedrawIncludeMisc);
        redrawModelMenu.addSeparator();
        redrawModelMenu.addCommandItem(commandManager, CommandIDs::redrawCircle);
        redrawModelMenu.addSeparator();
        redrawModelMenu.addCommandItem(commandManager, CommandIDs::redrawForceDirected);
        redrawModelMenu.addCommandItem(commandManager, CommandIDs::showRedrawOptions);
        menu.addSubMenu(TRANS("Redraw Model"), redrawModelMenu);
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
    	menu.addCommandItem(commandManager, CommandIDs::insertPulsetouch);
    	menu.addCommandItem(commandManager, CommandIDs::insertDetent);
    	menu.addCommandItem(commandManager, CommandIDs::insertSofteningLink);
    	menu.addCommandItem(commandManager, CommandIDs::insertStiffeningLink);
    	menu.addSeparator();
    	menu.addCommandItem(commandManager, CommandIDs::insertAudioOutput);
        menu.addCommandItem(commandManager, CommandIDs::insertAudioConnection);
        menu.addCommandItem(commandManager, CommandIDs::insertDisplay);
        menu.addCommandItem(commandManager, CommandIDs::insertDisplayConnection);
    	menu.addSeparator();
    	menu.addCommandItem(commandManager, CommandIDs::insertWaveguide);
    	menu.addCommandItem(commandManager, CommandIDs::insertTermination);
        menu.addCommandItem(commandManager, CommandIDs::insertJunction);
        menu.addSeparator();
        menu.addCommandItem(commandManager, CommandIDs::insertComment);
    }
    else if (topLevelMenuIndex == 3)
    {
    	menu.addCommandItem(commandManager, CommandIDs::generateFaust);
        menu.addSeparator();
    	menu.addCommandItem(commandManager, CommandIDs::generateExternal);
        menu.addSeparator();
        StringPairArray spa = StoredSettings::getInstance()->getExporters().getAllProperties();
        String currExporter = StoredSettings::getInstance()->getCurrentExporter();
        for (int i = 0; i < spa.size(); ++i)
        {
            bool isTicked = false;
            if(spa.getAllKeys()[i].compare(currExporter) == 0)
                isTicked = true;
            menu.addItem(exportersBaseID + i, spa.getAllKeys()[i],true,isTicked);
        }
    }
    else if (topLevelMenuIndex == 4)
    {
        menu.addCommandItem(commandManager, CommandIDs::cleanDataDir);
        menu.addCommandItem(commandManager, CommandIDs::cleanDataDirAll);
      	menu.addSeparator();
    	menu.addCommandItem(commandManager, CommandIDs::openDataDir);
        menu.addCommandItem(commandManager, CommandIDs::openMdlFileExtern);
        menu.addSeparator();
        menu.addCommandItem(commandManager, CommandIDs::printSAMLog);
        menu.addSeparator();
        menu.addCommandItem(commandManager, CommandIDs::printMDL);
#ifdef _DEBUG
        menu.addSeparator();
        menu.addCommandItem(commandManager, CommandIDs::writeMDLFileAsXml);
#endif
    }
    else if (topLevelMenuIndex == 5)
    {
        menu.addCommandItem (commandManager, CommandIDs::zoomIn);
        menu.addCommandItem (commandManager, CommandIDs::zoomOut);
        menu.addCommandItem (commandManager, CommandIDs::zoomNormal);
        menu.addSeparator();
        menu.addCommandItem (commandManager, CommandIDs::segmentedConnectors);
        menu.addSeparator();
        menu.addCommandItem(commandManager, CommandIDs::showAudioConnections);
        menu.addCommandItem(commandManager, CommandIDs::showObjectNames);
        menu.addSeparator();
        menu.addCommandItem (commandManager, CommandIDs::showGrid);
        menu.addCommandItem (commandManager, CommandIDs::enableSnapToGrid);

        const int currentSnapSize = getApp()->getActiveHolderComponent() != 0
            ? getApp()->getActiveHolderComponent()->getSnappingGridSize() : 0;

        PopupMenu m;
        for (int i = 0; i < numElementsInArray (snapSizes); ++i)
            m.addItem (snappingSizesBaseID + i,
                       String(snapSizes[i]) + " " + TRANS("pixels"),
                       true, snapSizes[i] == currentSnapSize);

        menu.addSubMenu(TRANS("Grid Size"), m, getApp()->getActiveHolderComponent() != 0);
    }
    else if (topLevelMenuIndex == 6)
    {
        menu.addCommandItem(commandManager, CommandIDs::showPostWindow);
        menu.addCommandItem(commandManager, CommandIDs::clearOutputConsole);
        menu.addSeparator();
        menu.addCommandItem(commandManager, CommandIDs::showPropertiesWindow);
        menu.addCommandItem(commandManager, CommandIDs::propertiesWindowOnTop);
        menu.addSeparator();
        menu.addCommandItem(commandManager, CommandIDs::showPreviousWindow);
        menu.addCommandItem(commandManager, CommandIDs::showNextWindow);
        menu.addSeparator();

        const int numDocs = jmin (50, SAMApplication::getApp()->mainWindows.size());

        for (int i = 0; i < numDocs; ++i)
        {
            MDLFile* mdl = SAMApplication::getApp()->mainWindows[i]->getMDLFile();
            menu.addItem (activeMDLsBaseID + i, mdl->getName());
        }
    }
    else if (topLevelMenuIndex == 7)
    {
    	menu.addCommandItem(commandManager, CommandIDs::showHelp);
    }

	return menu;
}

void SAMApplication:: MainMenuModel::menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/)
{
    if (menuItemID >= recentProjectsBaseID && menuItemID < recentProjectsBaseID + 100)
    {
        // open a file from the "recent files" menu
        const File file(StoredSettings::getInstance()->recentFiles
                        .getFile(menuItemID - recentProjectsBaseID));

        getApp()->openFile(file);
        StoredSettings::getInstance()->recentFiles.addFile(file);
    }
    if(menuItemID >= exportersBaseID && menuItemID < exportersBaseID + 100)
    {
        StringPairArray spa = StoredSettings::getInstance()
            ->getExporters().getAllProperties();

        StoredSettings::getInstance()
            ->setCurrentExporter(spa.getAllKeys()[menuItemID - exportersBaseID]);
#ifdef JUCE_MAC
        menuItemsChanged();
#endif
    }
    else if (menuItemID >= snappingSizesBaseID && menuItemID < snappingSizesBaseID + 100)
    {
        if (getApp()->getActiveHolderComponent() != 0)
        {
            getApp()->getActiveHolderComponent()
                ->setSnappingGrid(snapSizes [menuItemID - snappingSizesBaseID],
                                  getApp()->getActiveHolderComponent()->isSnapActive(false),
                                  getApp()->getActiveHolderComponent()->isSnapShown());
        }
    }
    else if(menuItemID >= activeMDLsBaseID && menuItemID < activeMDLsBaseID + 100)
    {
        int mawId = menuItemID - activeMDLsBaseID;
        if(mawId < SAMApplication::getApp()->mainWindows.size())
            if(MainAppWindow* maw = SAMApplication::getApp()->mainWindows[mawId])
                maw->toFront(true);
    }
}

ObjectsHolder* SAMApplication::getActiveHolderComponent()
{
    if (mainWindows.size() == 0)
        return nullptr;

    for (int i = Desktop::getInstance().getNumComponents(); --i >= 0;)
    {
        MainAppWindow* mw = dynamic_cast<MainAppWindow*> (Desktop::getInstance().getComponent(i));
        if (mainWindows.contains(mw))
            return mw->getHolderComponent();
    }

    return mainWindows.getLast()->getHolderComponent();
}

void SAMApplication::togglePropertiesWindow()
{
    if ((propertiesWindow->isVisible() && propertiesWindow->isAlwaysOnTop())
        || propertiesWindow->isActiveWindow())
        propertiesWindow->makeVisible(false);
    else
        propertiesWindow->makeVisible(true);

}

void SAMApplication::togglePropertiesWindowAlwaysOnTop()
{
    StoredSettings* settings = StoredSettings::getInstance();

    const bool shouldStayOnTop = !settings->getIsPropertiesWindowAlwaysOnTop();

    propertiesWindow->setAlwaysOnTop(shouldStayOnTop);

    settings->setIsPropertiesWindowAlwaysOnTop(shouldStayOnTop);
}

void SAMApplication::switchToNextWindow()
{
    if(activeWindowsList.size() > 0)
    {
        MainAppWindow* maw = activeWindowsList[0];
        activeWindowsList.remove(0);
        activeWindowsList.add(maw);
        activeWindowsList[0]->toFront(true);
    }
}

void SAMApplication::switchToPreviousWindow()
{
    if(activeWindowsList.size() > 0)
    {
        MainAppWindow* maw = activeWindowsList.getLast();
        activeWindowsList.remove(activeWindowsList.size()-1);
        activeWindowsList.insert(0, maw);
        activeWindowsList[0]->toFront(true);
    }
}

void SAMApplication::mainWindowActivityChanged (MainAppWindow* mainWin)
{
    const int idx = activeWindowsList.indexOf(mainWin);
    activeWindowsList.remove(idx);
    activeWindowsList.insert(0, mainWin);
}
