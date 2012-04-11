/*
  ==============================================================================

    DebugWindow.cpp
    Created: 11 Apr 2012 3:18:15pm
    Author:  peter

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

	setVisible (false);
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
