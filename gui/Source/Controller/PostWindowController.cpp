/*
  ==============================================================================

    PostWindowController.cpp
    Created: 2 Aug 2013 11:17:41pm
    Author:  peter

  ==============================================================================
*/

#include "../Application/CommonHeaders.h"
#include "../View/PostWindow.h"

#include "PostWindowController.h"

using namespace synthamodeler;

PostWindowController::PostWindowController()
{
    postWin = new PostWindow();
}

PostWindowController::~PostWindowController()
{
    postWin = nullptr;
}

void PostWindowController::init()
{
	postWin->printWelcomeMessage();
	postWin->makeVisible ();
}

void PostWindowController::post(const String& title, 
                                const String& textToWrite,
                                bool isBold)
{
	if(textToWrite.compare("") != 0)
	{
        Colour color = Colours::black;
        if(title.containsIgnoreCase("Error")
            || textToWrite.containsIgnoreCase("error"))
            color = Colours::red;
		postWin->printHeader();
        postWin->setTextColour(color);
		postWin->addText(title, false);
		postWin->addText(textToWrite, isBold);
		postWin->addNewLine();
        postWin->setTextColour(Colours::black);
	}
	else
	{
		postWin->addText("Nothing...\n\n", false);
	}

}

void PostWindowController::post(const String& textToWrite, bool isBold)
{
	if(textToWrite.compare("") != 0)
	{
		postWin->addText(textToWrite, isBold);
		postWin->addNewLine();
	}
	else
	{
		postWin->addText("Nothing...\n\n", false);
	}
}

void PostWindowController::toFront()
{
    postWin->toFront(true);
}

void PostWindowController::clear()
{
    postWin->clear();
}