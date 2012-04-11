/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic outline for a simple desktop window.

    Author: Peter Vasil

  ==============================================================================
*/

#include "MainWindow.h"
#include "CommonHeaders.h"
#include "ContentComp.h"

//==============================================================================
MainAppWindow::MainAppWindow()
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                      Colours::lightgrey,
                      DocumentWindow::allButtons)
{
    setResizable (true, false); // resizability is a property of ResizableWindow

    debugWindow = new DebugWindow();
    ContentComp* contentComp = new ContentComp(*this, *debugWindow.get());

    commandManager.registerAllCommandsForTarget (contentComp);
    commandManager.registerAllCommandsForTarget (JUCEApplication::getInstance());

    // this lets the command manager use keypresses that arrive in our window to send
    // out commands
    addKeyListener (commandManager.getKeyMappings());

    setContentOwned(contentComp,true);

    // this tells the DocumentWindow to automatically create and manage a MenuBarComponent
    // which uses our contentComp as its MenuBarModel
#ifdef JUCE_MAC
    setMenuBar(nullptr);
    MenuBarModel::setMacMainMenu(contentComp);
#else
    setMenuBar (contentComp);
#endif

    // tells our menu bar model that it should watch this command manager for
    // changes, and send change messages accordingly.
    contentComp->setApplicationCommandManagerToWatch (&commandManager);

    centreWithSize (getWidth(), getHeight());
    setVisible (true);
    setUsingNativeTitleBar(true);

}

MainAppWindow::~MainAppWindow()
{
    setMenuBar (nullptr);

#if JUCE_MAC  // ..and also the main bar if we're using that on a Mac...
    MenuBarModel::setMacMainMenu (nullptr);
#endif
    StoredSettings::deleteInstance();

    clearContentComponent();
}

void MainAppWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}
