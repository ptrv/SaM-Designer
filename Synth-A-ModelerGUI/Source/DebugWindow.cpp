/*
  ==============================================================================

    DebugWindow.cpp
    Created: 11 Apr 2012 3:18:15pm
    Author:  Peter Vasil

  ==============================================================================
*/

#include "DebugWindow.h"
#include "TextConsole.h"

DebugWindow::DebugWindow()
:
DocumentWindow("Output", Colours::black, DocumentWindow::closeButton, true)
{
	setResizable (false, false); // resizability is a property of ResizableWindow

	console = new TextConsole();
	console->setSize(400,400);

	setContentOwned(console,true);

	setVisible (true);
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
		setVisible(false);
	else
		setVisible(true);
}

void DebugWindow::clear()
{
	if(isVisible())
		console->clear();
}

void DebugWindow::addText(const String& compilerText)
{
	String debugText = "-------------------";
	debugText << newLine;
	debugText << "Synth-A-Modeler: ";
	debugText << Time::getCurrentTime().toString(true,true, true, true);
	debugText << newLine;
	debugText << "-------------------";
	debugText << newLine << newLine;
	debugText << compilerText;
	console->addLine(debugText);
}
