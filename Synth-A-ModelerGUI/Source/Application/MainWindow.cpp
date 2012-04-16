/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic outline for a simple desktop window.

    Author: Peter Vasil

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

#include "MainWindow.h"
#include "CommonHeaders.h"
#include "../View/ContentComp.h"

//==============================================================================
MainAppWindow::MainAppWindow()
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                      Colours::lightgrey,
                      DocumentWindow::allButtons)
{
    setResizable (true, true); // resizability is a property of ResizableWindow

    debugWindow = new DebugWindow();
    appController = new AppController(*this, *debugWindow);
    ContentComp* contentComp = new ContentComp(*this, *appController.get());

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
