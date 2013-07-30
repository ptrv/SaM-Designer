/*
  ==============================================================================

    DebugWindow.cpp
    Created: 11 Apr 2012 3:18:15pm
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
#include "../Application/CommonHeaders.h"
#include "../Application/Application.h"
#include "../View/TextConsole.h"
#include "../Utilities/StoredSettings.h"

#include "OutputWindow.h"

using namespace synthamodeler;

OutputWindow::OutputWindow()
:
DocumentWindow(SynthAModelerApplication::getApp()->getApplicationName() + " - Post window",
               Colours::darkseagreen, DocumentWindow::closeButton, false)
{
    setUsingNativeTitleBar(true);
	setResizable (true, true); // resizability is a property of ResizableWindow

	setSize(400,400);
	// restore the last size and position from our settings file..
	restoreWindowStateFromString (StoredSettings::getInstance()->getProps()
	                                    .getValue ("lastPostWindowPos"));

	console = new TextConsole();
	console->setSize(getWidth(),getHeight());

	setContentOwned(console,true);

	commandManager->registerAllCommandsForTarget(this);

	addKeyListener(commandManager->getKeyMappings());

}

OutputWindow::~OutputWindow()
{
    StoredSettings::getInstance()->getProps()
        .setValue ("lastPostWindowPos", getWindowStateAsString());

    clearContentComponent();
}

void OutputWindow::closeButtonPressed()
{
    SynthAModelerApplication::getApp()->systemRequestedQuit();
}

void OutputWindow::toggleDebugWindow()
{
	if(isVisible())
	{
		makeHide();
	}
	else
	{
		makeVisible();
	}
}

void OutputWindow::clear()
{
	if(isVisible())
		console->clear();
}

void OutputWindow::addText(const String& compilerText, bool isBold)
{
	SAM_LOG(compilerText);
    console->setBold(isBold);
	console->addLine(compilerText);
    console->setBold(!isBold);
}

void OutputWindow::printWelcomeMessage()
{
	String welcomeMsg;
	welcomeMsg << "Hello " << SystemStats::getLogonName() << "!" << newLine;
	welcomeMsg << "Welcome to Synth-A-Modeler. Happy physical modelling!" << newLine;
//	welcomeMsg << "----------------------------------------------";
	welcomeMsg << newLine;
	console->addLine(welcomeMsg);
}
void OutputWindow::printHeader()
{
	String debugText;
	debugText << newLine;
	debugText << "[";
	debugText << Time::getCurrentTime().toString(true,true, true, true);
	debugText << "]: " << newLine;
	debugText << newLine;
	addText(debugText, false);
}

void OutputWindow::addNewLine()
{
	console->addLine(newLine);
}
void OutputWindow::makeVisible()
{
	setVisible(true);
	addToDesktop();
	StoredSettings::getInstance()->setShowCompilerWindow(true);
}

void OutputWindow::makeHide()
{
	setVisible(false);
	removeFromDesktop();
	StoredSettings::getInstance()->setShowCompilerWindow(false);
}

ApplicationCommandTarget* OutputWindow::getNextCommandTarget()
{
    return findFirstTargetParentComponent();
}

void OutputWindow::getAllCommands (Array <CommandID>& /*commands*/)
{
}

void OutputWindow::getCommandInfo (CommandID /*commandID*/,
                                   ApplicationCommandInfo& /*result*/)
{
}

bool OutputWindow::perform (const InvocationInfo& /*info*/)
{
	return false;
}

void OutputWindow::setTextColour(const Colour& textColour)
{
    console->setColour(TextEditor::textColourId, textColour);
}
