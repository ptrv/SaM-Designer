/*
  ==============================================================================

    PostWindow.cpp
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

#include "PostWindow.h"

using namespace synthamodeler;

PostWindow::PostWindow()
:
DocumentWindow(SynthAModelerApplication::getApp()->getApplicationName() + " - Post Window",
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

    setComponentID("PostWindow");
}

PostWindow::~PostWindow()
{
    StoredSettings::getInstance()->getProps()
        .setValue ("lastPostWindowPos", getWindowStateAsString());

    clearContentComponent();
}

void PostWindow::closeButtonPressed()
{
    SynthAModelerApplication::getApp()->systemRequestedQuit();
}

void PostWindow::toggleDebugWindow()
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

void PostWindow::clear()
{
	if(isVisible())
		console->clear();
}

void PostWindow::addText(const String& compilerText, bool isBold)
{
	SAM_LOG(compilerText);
    console->setBold(isBold);
	console->addLine(compilerText);
}

void PostWindow::printWelcomeMessage()
{
	String welcomeMsg;
	welcomeMsg << "Hello " << SystemStats::getLogonName() << "!" << newLine;
	welcomeMsg << "Welcome to Synth-A-Modeler Designer. Happy physical modelling!";
    welcomeMsg << newLine;
//	welcomeMsg << "----------------------------------------------";
	welcomeMsg << newLine;
	console->addLine(welcomeMsg);
}
void PostWindow::printTimeStamp()
{
    String currTime = Time::getCurrentTime().formatted("%F %T");
    currTime << "." << String::formatted("%03d", Utils::getCurrentTimeMillis());

	String debugText;
	debugText << newLine;
	debugText << "[";
	debugText << currTime;
	debugText << "]: ";
//	debugText << "]: " << newLine;
//	debugText << newLine;
	addText(debugText, false);
}

void PostWindow::addNewLine()
{
	console->addLine(newLine);
}
void PostWindow::makeVisible()
{
	setVisible(true);
	addToDesktop();
	StoredSettings::getInstance()->setShowCompilerWindow(true);
}

void PostWindow::makeHide()
{
	setVisible(false);
	removeFromDesktop();
	StoredSettings::getInstance()->setShowCompilerWindow(false);
}

ApplicationCommandTarget* PostWindow::getNextCommandTarget()
{
    return findFirstTargetParentComponent();
}

void PostWindow::getAllCommands (Array <CommandID>& /*commands*/)
{
}

void PostWindow::getCommandInfo (CommandID /*commandID*/,
                                   ApplicationCommandInfo& /*result*/)
{
}

bool PostWindow::perform (const InvocationInfo& /*info*/)
{
	return false;
}

void PostWindow::setTextColour(const Colour& textColour)
{
    console->setColour(TextEditor::textColourId, textColour);
}
