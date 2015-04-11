/*
  ==============================================================================

    PostWindowController.cpp
    Created: 2 Aug 2013 11:17:41pm
    Author:  peter

  ==============================================================================
*/

#include "PostWindowController.h"

#include "Application/CommonHeaders.h"

#include "Models/SAMCmd.h"

#include "View/PostWindow.h"


using namespace synthamodeler;

PostWindowController::PostWindowController()
{
    postWin = new PostWindow();

//    currentPosLevel = StoredSettings::getInstance()->getCurrentPostLevel();
    currentPosLevel = ALL;
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

void PostWindowController::post(const String& textToWrite,
                                const PostLevel pl,
                                bool isBold)
{
    if(currentPosLevel <= pl)
    {
        if (textToWrite.compare("") != 0)
        {
            Colour color = Colours::black;
            String lvl = String::empty;
            if (pl == VERBOSE)
                lvl = "DEBUG: ";
            else if (pl == NORMAL)
                lvl = "LOG: ";
            else if (pl == ERROR)
                lvl = "ERROR: ";
            else if (pl == FATAL)
                lvl = "FATAL: ";

            if (textToWrite.containsIgnoreCase("error")
                || pl == ERROR || pl == FATAL)
                color = Colours::red;
            postWin->printTimeStamp();
            postWin->setTextColour(color);
            if(lvl.isNotEmpty())
                postWin->addText(lvl, false);
            postWin->addText(textToWrite, isBold);
            postWin->addNewLine();
            postWin->setTextColour(Colours::black);
        }
    }
}

//void PostWindowController::post(const String& textToWrite, bool isBold)
//{
//	if(textToWrite.compare("") != 0)
//	{
//		postWin->addText(textToWrite, isBold);
//		postWin->addNewLine();
//	}
//	else
//	{
//		postWin->addText("Nothing...\n\n", false);
//	}
//}

void PostWindowController::postLocked(const String& textToWrite,
                                      const PostLevel pl,
                                      bool isBold)
{
    const MessageManagerLock mmLock;
    if(mmLock.lockWasGained())
        post(textToWrite, pl, isBold);
}

bool PostWindowController::isActiveWindow()
{
    return postWin->isActiveWindow();
}
void PostWindowController::toFront()
{
    postWin->toFront(true);
}

void PostWindowController::clear()
{
    postWin->clear();
}
