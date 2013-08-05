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
#include "../View/PostWindow.h"
#include "../View/PropertiesWindow.h"
#include "MainWindow.h"
#include "../Models/MDLFile.h"
#include "SAMLookAndFeel.h"
#include "../View/ObjectsHolder.h"
#include "CommandLine.h"

#include "Application.h"

using namespace synthamodeler;

ScopedPointer<PropertiesWindow> synthamodeler::propertiesWindow;
ScopedPointer<PostWindowController> synthamodeler::postWindow;

#if UNIT_TESTS
#include "../../Testsuite/TestRunner.h"
#endif

static const int snapSizes[] = { 2, 3, 4, 5, 6, 8, 10, 12, 16, 20, 24, 32 };
//==============================================================================

SynthAModelerApplication::SynthAModelerApplication()
: isRunningCommandLine(false)
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
        quit();
        return;
    }

	commandManager = new ApplicationCommandManager();
	commandManager->registerAllCommandsForTarget (this);

	menuModel = new MainMenuModel();

	postWindow = new PostWindowController();

    propertiesWindow = new PropertiesWindow();

    Array<File> mdls (StoredSettings::getInstance()->getLastFiles());

    for (int i = 0; i < mdls.size(); ++ i)
        openFile (mdls.getReference(i));

	if (commandLine.trim().isNotEmpty() && ! commandLine.trim().startsWithChar ('-'))
	{
		 DBG(commandLine);
		 anotherInstanceStarted (commandLine);
	}
	else
	{
		Array<File> mdlsArr (StoredSettings::getInstance()->getLastFiles());
		for (int i = 0; i < mdlsArr.size(); ++ i)
			openFile (mdlsArr.getReference(i));
	}
    // get all exporters
    if(StoredSettings::getInstance()->getExporters().getAllProperties().size() == 0)
    {
        XmlDocument xml(String::createStringFromData(BinaryData::default_exporters_xml,
                                                     BinaryData::default_exporters_xmlSize));
        ScopedPointer<XmlElement> elem(xml.getDocumentElement());
        for (int i = 0; i < elem->getNumChildElements(); ++i)
        {
            XmlElement* c = elem->getChildElement(i);
            StoredSettings::getInstance()->getExporters().setValue(c->getStringAttribute("name"),
                                                                   c->getStringAttribute("val"));

        }
        elem = nullptr;
        String currentExporter = StoredSettings::getInstance()->getExporters().getAllProperties().getAllKeys()[0];
        StoredSettings::getInstance()->setCurrentExporter(currentExporter);
    }

    // create default values properties file if not present
    XmlDocument defaultValuesXml(String::createStringFromData(BinaryData::default_values_xml,
                                                              BinaryData::default_values_xmlSize));
    ScopedPointer<XmlElement> elem(defaultValuesXml.getDocumentElement());

    if(StoredSettings::getInstance()->getDefaultValues().getAllProperties().size()
        != elem->getNumChildElements())
    {
        for (int i = 0; i < elem->getNumChildElements(); ++i)
        {
            XmlElement* c = elem->getChildElement(i);
            StoredSettings::getInstance()->getDefaultValues().setValue(c->getStringAttribute("name"),
                                                                       c->getStringAttribute("val"));
        }
    }
    elem = nullptr;

    if (mainWindows.size() == 0)
        createNewMainWindow()->makeVisible();

#if JUCE_MAC
	MenuBarModel::setMacMainMenu (menuModel);
#endif

    postWindow->init();

	getOrCreateFrontmostWindow()->toFront(true);

//    propertiesWindow->makeVisible();
}

void SynthAModelerApplication::shutdown()
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
//    if (mainWindows.size() == 0)
//        systemRequestedQuit();
//#endif

    updateRecentProjectList();
}

bool SynthAModelerApplication::isCommandActive (const CommandID /*commandID*/)
{
    return true;
}


//==============================================================================
const String SynthAModelerApplication::getApplicationName()
{
	return "Synth-A-Modeler Designer";
}

const String SynthAModelerApplication::getApplicationVersion()
{
	return ProjectInfo::versionString;
}

bool SynthAModelerApplication::moreThanOneInstanceAllowed()
{
	return true;
}

void SynthAModelerApplication::anotherInstanceStarted (const String& commandLine)
{
    File f;
    if(File::isAbsolutePath(commandLine))
        f = commandLine;
    else
        f = File::getCurrentWorkingDirectory().getChildFile(commandLine);

    if(f.existsAsFile() && f.hasFileExtension(MDLFile::mdlFileExtension))
        openFile(f);
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
                              CommandIDs::showPostWindow,
                              CommandIDs::clearOutputConsole,
                              CommandIDs::openDataDir,
                              CommandIDs::showHelp,
                              CommandIDs::showPropertiesWindow,
                              CommandIDs::propertiesWindowOnTop
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
        result.setInfo("Clear Output Window", "", CommandCategories::tools,0);
        result.addDefaultKeypress('k', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
    	break;
    case CommandIDs::openDataDir:
        result.setInfo("Open Data Directory", "", CommandCategories::tools, 0);
        result.addDefaultKeypress('l', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::showHelp:
    	result.setInfo("Online Help", "Open online help in web browser.", CommandCategories::help, 0);
    	break;
    case CommandIDs::showPostWindow:
        result.setInfo("Show Post Window", "", CommandCategories::tools,0);
    	result.addDefaultKeypress('k', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::showPropertiesWindow:
        result.setInfo("Show Properties Window", "", CommandCategories::windows,0);
    	result.addDefaultKeypress('i', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::propertiesWindowOnTop:
        result.setInfo("Properties Window Always On Top", "", CommandCategories::windows,0);
        result.setTicked(StoredSettings::getInstance()->getIsPropertiesWindowAlwaysOnTop());
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

    case CommandIDs::showPostWindow:
        postWindow->toFront();
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
    FileChooser fc ("Open MDL File", File::nonexistent, "*.mdl;*.mdlx");

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

StringArray SynthAModelerApplication::MainMenuModel::getMenuBarNames()
{
    const char* const names[] = { "File", "Edit", "Insert",
    		"Generate", "Tools", "View", "Help", nullptr };

    return StringArray (names);
}

PopupMenu SynthAModelerApplication::MainMenuModel::getMenuForIndex (int topLevelMenuIndex, const String& /*menuName*/)
{
	PopupMenu menu;
    if (topLevelMenuIndex == 0)
    {
        menu.addCommandItem (commandManager, CommandIDs::newFile);
        menu.addCommandItem (commandManager, CommandIDs::open);

        PopupMenu recentFiles;
        StoredSettings::getInstance()->recentFiles
        		.createPopupMenuItems (recentFiles, 100, true, true);

        menu.addSubMenu ("Open Recent File", recentFiles);

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
        redrawModelMenu.addCommandItem(commandManager, CommandIDs::redrawCircle);
        redrawModelMenu.addSeparator();
        redrawModelMenu.addCommandItem(commandManager, CommandIDs::redrawForceDirected);
        redrawModelMenu.addCommandItem(commandManager, CommandIDs::showRedrawOptions);
        menu.addSubMenu("Redraw Model", redrawModelMenu);
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
    	menu.addSeparator();
    	menu.addCommandItem(commandManager, CommandIDs::insertAudioOutput);
        menu.addCommandItem(commandManager, CommandIDs::insertAudioConnection);
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
            menu.addItem(200 + i, spa.getAllKeys()[i],true,isTicked);
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
        menu.addCommandItem(commandManager, CommandIDs::dumpMDL);
#ifdef _DEBUG
        menu.addSeparator();
        menu.addCommandItem(commandManager, CommandIDs::writeMDLFileAsXml);
#endif
    }
    else if (topLevelMenuIndex == 5)
    {
        menu.addCommandItem(commandManager, CommandIDs::showPostWindow);
        menu.addCommandItem(commandManager, CommandIDs::clearOutputConsole);
        menu.addSeparator();
        menu.addCommandItem(commandManager, CommandIDs::showPropertiesWindow);
        menu.addCommandItem(commandManager, CommandIDs::propertiesWindowOnTop);
        menu.addSeparator();
        menu.addCommandItem (commandManager, CommandIDs::segmentedConnectors);
        menu.addSeparator();
        menu.addCommandItem (commandManager, CommandIDs::zoomIn);
        menu.addCommandItem (commandManager, CommandIDs::zoomOut);
        menu.addCommandItem (commandManager, CommandIDs::zoomNormal);
        menu.addSeparator();
        menu.addCommandItem(commandManager, CommandIDs::showObjectNames);
        menu.addSeparator();
        menu.addCommandItem (commandManager, CommandIDs::showGrid);
        menu.addCommandItem (commandManager, CommandIDs::enableSnapToGrid);

        const int currentSnapSize = getApp()->getActiveHolderComponent() != 0
            ? getApp()->getActiveHolderComponent()->getSnappingGridSize() : 0;

        PopupMenu m;
        for (int i = 0; i < numElementsInArray (snapSizes); ++i)
            m.addItem (300 + i, String (snapSizes[i]) + " pixels", true, snapSizes[i] == currentSnapSize);

        menu.addSubMenu ("Grid Size", m, getApp()->getActiveHolderComponent() != 0);

        menu.addSeparator();
        menu.addCommandItem(commandManager, CommandIDs::showAudioConnections);
    }
    else if (topLevelMenuIndex == 6)
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
    if(menuItemID >= 200 && menuItemID < 300)
    {
        StringPairArray spa = StoredSettings::getInstance()->getExporters().getAllProperties();
        StoredSettings::getInstance()->setCurrentExporter(spa.getAllKeys()[menuItemID - 200]);
    }
    else if (menuItemID >= 300 && menuItemID < 400)
    {
        if (getApp()->getActiveHolderComponent() != 0)
        {
            getApp()->getActiveHolderComponent()->setSnappingGrid (snapSizes [menuItemID - 300],
                                                                   getApp()->getActiveHolderComponent()->isSnapActive(false),
                                                                   getApp()->getActiveHolderComponent()->isSnapShown());
        }
    }
}

ObjectsHolder* SynthAModelerApplication::getActiveHolderComponent()
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

void SynthAModelerApplication::togglePropertiesWindow()
{
    if ((propertiesWindow->isVisible() && propertiesWindow->isAlwaysOnTop())
        || propertiesWindow->isActiveWindow())
        propertiesWindow->makeVisible(false);
    else
        propertiesWindow->makeVisible(true);

}

void SynthAModelerApplication::togglePropertiesWindowAlwaysOnTop()
{
    StoredSettings* settings = StoredSettings::getInstance();

    bool shouldStayOnTop = ! settings->getIsPropertiesWindowAlwaysOnTop();

    propertiesWindow->setAlwaysOnTop(shouldStayOnTop);

    settings->setIsPropertiesWindowAlwaysOnTop(shouldStayOnTop);
}