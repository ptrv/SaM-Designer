/*
  ==============================================================================

    AppController.cpp
    Created: 12 Apr 2012 1:30:26am
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

#include "AppController.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../Application/MainWindow.h"
#include "../View/DebugWindow.h"
#include "../Application/CommonHeaders.h"
#include "../View/PrefsPanel.h"
#include "../Models/ObjectIDs.h"

AppController::AppController(MainAppWindow& maw_, DebugWindow& dw_)
: maw(maw_), dw(dw_)
{
	mdlController = new MDLController(*this);
	objController = new ObjController(*this);
	setMainWindowTitle();
}

AppController::~AppController()
{

}

bool AppController::perform (UndoableAction* const action, const String& actionName)
{
	bool performOk = false;
	if(mdlController->getUndoManager() != nullptr)
	{
		mdlController->getUndoManager()->beginNewTransaction(actionName);
		performOk = mdlController->getUndoManager()->perform(action, actionName);
	}
	return performOk;
}
bool AppController::menuItemWasClicked(CommandID menuId)
{
    switch (menuId)
    {
    case CommandIDs::newFile:
    	mdlController->newFile();
    	setMainWindowTitle();
        return true;
    case CommandIDs::open:
    	mdlController->open();
    	setMainWindowTitle();
    	return true;
    case CommandIDs::closeDocument:
    	mdlController->close();
    	setMainWindowTitle();
    	return true;
    case CommandIDs::saveDocument:
    	mdlController->save();
    	setMainWindowTitle();
        return true;
    case CommandIDs::saveDocumentAs:
    	mdlController->saveAs();
    	setMainWindowTitle();
        return true;
    case CommandIDs::showPrefs:
    	PrefsPanel::show();
    	break;
    case CommandIDs::undo:
    	mdlController->getUndoManager()->undo();
		break;
	case CommandIDs::redo:
		mdlController->getUndoManager()->redo();
		break;

	case StandardApplicationCommandIDs::cut:
		break;
	case StandardApplicationCommandIDs::copy:
		break;
	case StandardApplicationCommandIDs::paste:
		break;
	case StandardApplicationCommandIDs::selectAll:
		break;
	case StandardApplicationCommandIDs::deselectAll:
		break;
	case StandardApplicationCommandIDs::del:
		DBG("delete");
		break;
    case CommandIDs::defineVariables:
    	break;
    case CommandIDs::segmentedConnectors:
    	StoredSettings::getInstance()->setIsSegmentedConnectors(!StoredSettings::getInstance()->getIsSegmentedConnectors());
    	break;
    case CommandIDs::zoomIn:
    	break;
    case CommandIDs::zoomOut:
    	break;
    case CommandIDs::zoomNormal:
    	break;
    case CommandIDs::reverseDirection:
    	break;

    case CommandIDs::insertMass:
    	objController->addObject(Ids::mass);
    	break;
    case CommandIDs::insertGround:
    	objController->addObject(Ids::ground);
    	break;
    case CommandIDs::insertResonator:
    	objController->addObject(Ids::resonator);
    	break;
    case CommandIDs::insertPort:
    	objController->addObject(Ids::port);
    	break;

    case CommandIDs::insertLink:
    	objController->addObject(Ids::link);
    	break;
    case CommandIDs::insertTouch:
    	objController->addObject(Ids::touch);
    	break;
    case CommandIDs::insertPluck:
    	objController->addObject(Ids::pluck);
    	break;

    case CommandIDs::insertAudioOutput:
    	objController->addObject(Ids::audioout);
    	break;
    case CommandIDs::insertWaveguide:
    	objController->addObject(Ids::waveguide);
    	break;
    case CommandIDs::insertTermination:
    	objController->addObject(Ids::termination);
    	break;

    case CommandIDs::generateFaust:
    {
    	String consoleText = mdlController->generateFaust();
    	if(consoleText.compare("") != 0)
    	{
    		dw.printHeader();
    		dw.addText("Generating FAUST code...\n");
    		dw.addText(consoleText);
    		dw.addText(newLine);
    	}
    	else
    	{
    		dw.addText("Nothing...\n\n");
    	}
    }
    	break;
    case CommandIDs::generateExternal:
    {
    	String consoleText = mdlController->generateExternal();
    	if(consoleText.compare("") != 0)
    	{
    		dw.printHeader();
    		dw.addText("Generating "+StoredSettings::getInstance()->getCmdExporter()+" external...\n\n");
    		dw.addText(consoleText);
    		dw.addText(newLine);
    	}
    	else
    	{
    		dw.addText("Nothing...\n\n");
    	}
    }
    	break;

    case CommandIDs::showOutputConsole:
    	debugWindowToggle();
    	break;
    case CommandIDs::clearOutputConsole:
    	debugWindowClear();
    	break;
    case CommandIDs::openDataDir:
    {
#if JUCE_MAC
        Process::openDocument("/usr/bin/open", StoredSettings::getInstance()->getDataDir());
#else
    	Process::openDocument("file:"+StoredSettings::getInstance()->getDataDir(), "");
#endif
        
    }
    break;
    case CommandIDs::showHelp:
    {
    	URL helpUrl("https://github.com/ptrv/Synth-A-Modeler/wiki");
    	helpUrl.launchInDefaultBrowser();
    }
    break;
	default:
        return false;
    };
    setMainWindowTitle();
    return true;

}

void AppController::openMDL(const File& mdlFile)
{
	mdlController->openFromFile(mdlFile);
}
void AppController::debugWindowToggle()
{
	dw.toggleDebugWindow();
	maw.toFront(true);

}

void AppController::debugWindowClear()
{
	dw.clear();
}

void AppController::setMainWindowTitle()
{
	String title = JUCEApplication::getInstance()->getApplicationName();
	title << " - " << mdlController->getMDLName();
	maw.setName(title);
}

bool AppController::mdlCheckAndSave()
{
	return mdlController->mdlCheckAndSaveIfNeeded();
}

UndoManager* AppController::getUndoManager()
{
	return mdlController->getUndoManager();
}
