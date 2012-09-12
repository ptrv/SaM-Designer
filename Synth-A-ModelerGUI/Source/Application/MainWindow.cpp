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

#include "../Application/Application.h"
#include "../View/OutputWindow.h"
#include "../View/ContentComp.h"
#include "../Controller/ObjController.h"
#include "../Controller/MDLController.h"
#include "../Models/MDLFile.h"

#include "MainWindow.h"

ScopedPointer<ApplicationCommandManager> commandManager;

//==============================================================================
MainAppWindow::MainAppWindow()
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
    		Colour::greyLevel (0.6f),
                      DocumentWindow::allButtons,
                      false)
{
	mdlController = new MDLController(*this);
	objController = new ObjController(*mdlController);
	setUsingNativeTitleBar (true);
	createMDLFileContentCompIfNeeded();

#if ! JUCE_MAC
	setMenuBar (SynthAModelerApplication::getApp()->menuModel);
#endif

    setResizable (true, false);
    centreWithSize (800, 600);

	commandManager->registerAllCommandsForTarget(this);
    commandManager->registerAllCommandsForTarget (getMDLFileContentComponent());

    // this lets the command manager use keypresses that arrive in our window to send
    // out commands
    addKeyListener (commandManager->getKeyMappings());

    setWantsKeyboardFocus (false);

    getLookAndFeel().setColour (ColourSelector::backgroundColourId, Colours::transparentBlack);
}

MainAppWindow::~MainAppWindow()
{
#if ! JUCE_MAC
	setMenuBar (nullptr);
#endif
	removeKeyListener (commandManager->getKeyMappings());

    // save the current size and position to our settings file..
    StoredSettings::getInstance()->getProps()
        .setValue ("lastMainWindowPos", getWindowStateAsString());

    clearContentComponent();

    mdlController = nullptr;
    objController = nullptr;

}

void MainAppWindow::createMDLFileContentCompIfNeeded()
{
    if (getMDLFileContentComponent() == nullptr)
    {
        clearContentComponent();
        setContentOwned (new ContentComp(*this, *objController), false);
    }
}

ContentComp* MainAppWindow::getMDLFileContentComponent() const
{
	return dynamic_cast<ContentComp*> (getContentComponent());
}
void MainAppWindow::closeButtonPressed()
{
    if (! closeCurrentMDLFile())
        return;

    SynthAModelerApplication::getApp()->closeWindow (this);
}

void MainAppWindow::changeListenerCallback(ChangeBroadcaster*)
{
	updateTitle();
}


bool MainAppWindow::closeMDLFile (MDLFile* mdlFile)
{
    jassert (mdlFile == mdlController->getMDLFile() && mdlFile != nullptr);

    if (mdlFile->isEmpty() || mdlFile == nullptr)
        return true;

    if(! mdlFile->isUntiled())
    {
        StoredSettings::getInstance()->getProps()
            .setValue (getProjectWindowPosName(), getWindowStateAsString());
        StoredSettings::getInstance()->getProps()
            .setValue(getProjectWindowZoomName(), getMDLFileContentComponent()->getZoom());
    }

    FileBasedDocument::SaveResult r = mdlFile->saveIfNeededAndUserAgrees();

    if (r == FileBasedDocument::savedOk)
    {
        setMDLFile(nullptr);
//        mdlFile->removeChangeListener(this);
//        mdlController->close();
        return true;
    }

    return false;
}

void MainAppWindow::setMDLFile (MDLFile* newMDLFile)
{
    createMDLFileContentCompIfNeeded();
    mdlController->setMDLFile(newMDLFile);
    getMDLFileContentComponent()->setMDLFile(newMDLFile);
    commandManager->commandStatusChanged();

    // (mustn't do this when the project is 0, because that'll happen on shutdown,
    // which will erase the list of recent projects)
    if (newMDLFile != nullptr)
    {
        SynthAModelerApplication::getApp()->updateRecentProjectList();

        newMDLFile->addChangeListener(this);
        updateTitle();

    }
}

bool MainAppWindow::closeCurrentMDLFile()
{
    return mdlController->getMDLFile() == nullptr || closeMDLFile(mdlController->getMDLFile());
}


void MainAppWindow::restoreWindowPosition()
{
    String windowState;

    if (mdlController->getMDLFile() != nullptr)
    {
        windowState = StoredSettings::getInstance()->getProps().getValue (getProjectWindowPosName());
        double newZoomFactor = StoredSettings::getInstance()->getProps().getDoubleValue (getProjectWindowZoomName(), 1.0);
        getMDLFileContentComponent()->setZoom(newZoomFactor);
    }

    if (windowState.isEmpty())
        windowState = StoredSettings::getInstance()->getProps().getValue ("lastMainWindowPos");

    restoreWindowStateFromString (windowState);
}
//==============================================================================
void MainAppWindow::makeVisible()
{
    setVisible (true);
    addToDesktop();  // (must add before restoring size so that fullscreen will work)
    restoreWindowPosition();

    getContentComponent()->grabKeyboardFocus();
}


//==============================================================================
ApplicationCommandTarget* MainAppWindow::getNextCommandTarget()
{
    return findFirstTargetParentComponent();

}

void MainAppWindow::getAllCommands (Array <CommandID>& commands)
{
    const CommandID ids[] = { 	CommandIDs::closeDocument,
            					CommandIDs::saveDocument,
            					CommandIDs::saveDocumentAs,
                                CommandIDs::generateFaust,
                                CommandIDs::generateExternal,
#ifdef _DEBUG
                                CommandIDs::writeMDLFileAsXml,
#endif
                                CommandIDs::dumpMDL,
    };

    commands.addArray (ids, numElementsInArray (ids));
}

void MainAppWindow::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
    switch (commandID)
    {
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
    case CommandIDs::generateFaust:
    	result.setInfo("Generic Faust Code", "", CommandCategories::generation,0);
    	result.addDefaultKeypress('g', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::generateExternal:
    	result.setInfo("External Object", "", CommandCategories::generation,0);
    	result.addDefaultKeypress('e', ModifierKeys::commandModifier);
    	break;
#if defined _DEBUG
    case CommandIDs::writeMDLFileAsXml:
        result.setInfo("MDL -> XML", "Write MDL file as XML", CommandCategories::tools, 0);
        break;
#endif
    case CommandIDs::dumpMDL:
        result.setInfo("Dump MDL", "", CommandCategories::tools, 0);
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
    switch (info.commandID)
    {
    case CommandIDs::closeDocument:
    	closeButtonPressed();
    	break;
    case CommandIDs::saveDocument:
    	mdlController->save();
        updateTitle();
    	break;
    case CommandIDs::saveDocumentAs:
    	mdlController->saveAs();
        updateTitle();
    	break;
    case CommandIDs::generateFaust:
    {
    	String consoleText = "Start Synth-A-Modeler...";
        consoleText << mdlController->generateFaust();
        consoleText << "\nSynth-A-Modeler finished.";
    	String titleText = "Generate FAUST code...\n\n";
    	SynthAModelerApplication::getApp()->writeToDebugConsole(titleText, consoleText);
    }
    	break;
    case CommandIDs::generateExternal:
    {
    	String consoleText = mdlController->generateExternal();
    	String titleText = "Generating "+StoredSettings::getInstance()->getCmdExporter()+" external...\n\n";
        consoleText << "\nFinished!";
    	SynthAModelerApplication::getApp()->writeToDebugConsole(titleText, consoleText);
    }
    	break;
#ifdef _DEBUG
    case CommandIDs::writeMDLFileAsXml:
        return mdlController->saveAsXml();
#endif
    case CommandIDs::dumpMDL:
        SAM_CONSOLE(mdlController->getMDLName()+"\n\n",
                    mdlController->getMDLFile()->toString());
        break;

	default:
        return false;
    };
    return true;

}

void MainAppWindow::updateTitle ()
{
	String title = JUCEApplication::getInstance()->getApplicationName();
	title << " - " << mdlController->getMDLFile()->getNameWithStatus();
	this->setName(title);

}

MDLFile* MainAppWindow::getMDLFile()
{
	return mdlController->getMDLFile();
}

UndoManager* MainAppWindow::getUndoManager()
{
	return mdlController->getUndoManager();
}

String MainAppWindow::getProjectWindowPosName() const
{
    jassert (mdlController->getMDLFile() != nullptr);
    if (mdlController->getMDLFile() == nullptr)
        return String::empty;

    return "projectWindowPos_" + mdlController->getMDLFile()->getName();
}
String MainAppWindow::getProjectWindowZoomName() const
{
    jassert (mdlController->getMDLFile() != nullptr);
    if (mdlController->getMDLFile() == nullptr)
        return String::empty;
    
    return "projectWindowZoom_" + mdlController->getMDLFile()->getName();
}
