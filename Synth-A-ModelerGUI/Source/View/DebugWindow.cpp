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
#include "DebugWindow.h"
#include "../Application/Application.h"
#include "../View/TextConsole.h"
#include "../Utilities/StoredSettings.h"

DebugWindow::DebugWindow()
:
DocumentWindow("Output", Colours::darkseagreen, 0)
{
	setResizable (true, true); // resizability is a property of ResizableWindow

	setSize(400,400);
	// restore the last size and position from our settings file..
	restoreWindowStateFromString (StoredSettings::getInstance()->getProps()
	                                    .getValue ("lastDebugWindowPos"));

	console = new TextConsole();
	console->setSize(getWidth(),getHeight());

	setContentOwned(console,true);

	commandManager->registerAllCommandsForTarget(this);

	addKeyListener(commandManager->getKeyMappings());
}

DebugWindow::~DebugWindow()
{
	// Workaround to prevent wrong bound values to be stored in user settings on linux
#if JUCE_LINUX
	Rectangle<int> br = getBounds();
	br.setHeight(getHeight()-28);
	br.setWidth(getWidth()-2);
	setBounds(br);
#endif

    StoredSettings::getInstance()->getProps()
        .setValue ("lastDebugWindowPos", getWindowStateAsString());

    clearContentComponent();

}

void DebugWindow::closeButtonPressed()
{
	makeHide();
}

void DebugWindow::toggleDebugWindow()
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

void DebugWindow::clear()
{
	if(isVisible())
		console->clear();
}

void DebugWindow::addText(const String& compilerText)
{
	SAM_LOG(compilerText);
	console->addLine(compilerText);
}

void DebugWindow::printWelcomeMessage()
{
	String welcomeMsg;
	welcomeMsg << "Hi " << SystemStats::getLogonName() << "!" << newLine;
	welcomeMsg << "Welcome to Synth-A-Modeler. Have fun!" << newLine;
	welcomeMsg << "----------------------------------------------";
	welcomeMsg << newLine;
	console->addLine(welcomeMsg);
}
void DebugWindow::printHeader()
{
	String debugText;
	debugText << newLine;
	debugText << "[";
	debugText << Time::getCurrentTime().toString(true,true, true, true);
	debugText << "]: " << newLine;
	debugText << newLine;
	console->addLine(debugText);
}

void DebugWindow::addNewLine()
{
	console->addLine(newLine);
}
void DebugWindow::makeVisible()
{
	setVisible(true);
	addToDesktop();
	StoredSettings::getInstance()->setShowCompilerWindow(true);
}

void DebugWindow::makeHide()
{
	setVisible(false);
	removeFromDesktop();
	StoredSettings::getInstance()->setShowCompilerWindow(false);
}

ApplicationCommandTarget* DebugWindow::getNextCommandTarget()
{
    return findFirstTargetParentComponent();
}

void DebugWindow::getAllCommands (Array <CommandID>& commands)
{
}

void DebugWindow::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
}

bool DebugWindow::perform (const InvocationInfo& info)
{
	return false;
}
