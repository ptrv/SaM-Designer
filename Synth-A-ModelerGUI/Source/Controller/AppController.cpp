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
#include "../Application/DebugWindow.h"
#include "../Application/CommonHeaders.h"

AppController::AppController(MainAppWindow& maw_, DebugWindow& dw_)
: maw(maw_), dw(dw_)
{
	mdlController = new MDLController();
	objController = new ObjController();
}

AppController::~AppController()
{

}

bool AppController::menuItemWasClicked(CommandID menuId)
{
    switch (menuId)
    {
    case CommandIDs::newFile:
    	mdlController->newFile();
        return true;
    case CommandIDs::open:
    	mdlController->open();
        return true;
    case CommandIDs::closeDocument:
    	mdlController->close();
        return true;
    case CommandIDs::saveDocument:
    	mdlController->save();
        return true;
    case CommandIDs::saveDocumentAs:
    	mdlController->saveAs();
        return true;
    case CommandIDs::undo:
		break;
	case CommandIDs::redo:
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

    case CommandIDs::segmentedConnectors:
    	break;
    case CommandIDs::zoomIn:
    	break;
    case CommandIDs::zoomOut:
    	break;
    case CommandIDs::zoomNormal:
    	break;
    case CommandIDs::reverseDirection:
    	break;
    case CommandIDs::showLabels:
    	break;

    case CommandIDs::insertMass:
    	break;
    case CommandIDs::insertGround:
    	break;
    case CommandIDs::insertResonator:
    	break;
    case CommandIDs::insertPort:
    	break;

    case CommandIDs::insertLink:
    	break;
    case CommandIDs::insertTouch:
    	break;
    case CommandIDs::insertPluck:
    	break;

    case CommandIDs::insertAudioOutput:
    	break;
    case CommandIDs::insertWaveguide:
    	break;
    case CommandIDs::insertTermination:
    	break;

    case CommandIDs::generateFaust:
    	mdlController->generateFaust();
    	break;
    case CommandIDs::generateExternal:
    	mdlController->generateExternal();
    	break;

    case CommandIDs::showOutputConsole:
    	debugWindowToggle();
    	break;
    case CommandIDs::clearOutputConsole:
    	debugWindowClear();
    	break;

	default:
        return false;
    };

    return true;

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
