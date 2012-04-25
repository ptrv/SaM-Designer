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

#include "DebugWindow.h"
#include "../View/TextConsole.h"
#include "../Utilities/StoredSettings.h"

DebugWindow::DebugWindow()
:
DocumentWindow("Output", Colours::black, DocumentWindow::closeButton, true)
{
	setResizable (false, false); // resizability is a property of ResizableWindow

	console = new TextConsole();
	console->setSize(400,400);

	setContentOwned(console,true);

	setVisible (StoredSettings::getInstance()->getShowCompilerWindow());
}

DebugWindow::~DebugWindow()
{
	clearContentComponent();
}

void DebugWindow::closeButtonPressed()
{
	setVisible(false);
}

void DebugWindow::toggleDebugWindow()
{
	if(isVisible())
	{
		setVisible(false);
		StoredSettings::getInstance()->setShowCompilerWindow(false);
	}
	else
	{
		setVisible(true);
		StoredSettings::getInstance()->setShowCompilerWindow(true);
	}
}

void DebugWindow::clear()
{
	if(isVisible())
		console->clear();
}

void DebugWindow::addText(const String& compilerText)
{
//	String debugText = "-------------------";
//	debugText << newLine;
//	debugText << "Synth-A-Modeler: ";
//	debugText << Time::getCurrentTime().toString(true,true, true, true);
//	debugText << newLine;
//	debugText << "-------------------";
//	debugText << newLine << newLine;
//	debugText << compilerText;
//	console->addLine(debugText);
	console->addLine(compilerText);
}

void DebugWindow::printHeader()
{
	String debugText = "-------------------";
	debugText << newLine;
	debugText << "Synth-A-Modeler: ";
	debugText << Time::getCurrentTime().toString(true,true, true, true);
	debugText << newLine;
	debugText << "-------------------";
	debugText << newLine << newLine;
	console->addLine(debugText);
}
