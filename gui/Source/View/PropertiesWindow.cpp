/*
  ==============================================================================

    PropertiesWindow.cpp
    Created: 23 Jul 2013 10:12:43pm
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
#include "../Application/MainWindow.h"

#include "ContentComp.h"
#include "ObjectsHolder.h"
#include "../Controller/ObjController.h"
#include "../Controller/ObjController.h"
#include "../Graph/Node.h"
#include "BaseObjectComponent.h"
#include "ObjectPropertiesComponent.h"

#include "PropertiesWindow.h"

using namespace synthamodeler;

class EmptyComponent : public Component
{
public:
    EmptyComponent() : Component() {}
    EmptyComponent(const String& compName) : Component(compName) {}
    void paint(Graphics& g)
    {
        g.drawText("No object selected", getWidth()/2 - 75, getHeight()/2 - 25,
                   150, 50, Justification::centred,false);
    }
};

PropertiesWindow::PropertiesWindow()
:
DocumentWindow("Properties", Colour::greyLevel (0.92f), DocumentWindow::closeButton, true)
{
    Desktop::getInstance().addFocusChangeListener(this);

    setSize(200, 300);

    // restore the last size and position from our settings file..
	restoreWindowStateFromString (StoredSettings::getInstance()->getProps()
	                                    .getValue ("lastPropertiesWindowPos"));

    setContentOwned(new EmptyComponent(), false);
    setResizable(true, true);
}

PropertiesWindow::~PropertiesWindow()
{
    StoredSettings::getInstance()->getProps()
        .setValue ("lastPropertiesWindowPos", getWindowStateAsString());

    clearContentComponent();
}

void PropertiesWindow::makeVisible(bool shouldBeVisible)
{
	setVisible(shouldBeVisible);
//	StoredSettings::getInstance()->setShowCompilerWindow(true);
}

void PropertiesWindow::globalFocusChanged (Component* focusedComponent)
{
    if(ContentComp* cc = dynamic_cast<ContentComp*>(focusedComponent))
    {
        currentContentComp = cc;
        currentSelection = &cc->getHolderComponent()->getObjController().getSelectedObjects();
        DBG("focus changed");
    }

}

void PropertiesWindow::changeListenerCallback(ChangeBroadcaster* /*source*/)
{
    if(currentSelection)
    {
        DBG("Selection changed");
        updateProperties();
    }
    else
    {
        setContentOwned(new EmptyComponent("no selection"), true);
    }
}
void PropertiesWindow::closeButtonPressed()
{
    setVisible(false);
}

bool PropertiesWindow::keyPressed(const KeyPress& kp)
{
    if(kp == KeyPress::escapeKey)
    {
        closeButtonPressed();
        return true;
    }
    return false;
}

static String currentObjectId;
void PropertiesWindow::updateProperties()
{
    if (BaseObjectComponent * caller = dynamic_cast<BaseObjectComponent*> (currentSelection->getSelectedItem(0)))
    {
        Component* comp;
        ObjController* objCtrl = &currentContentComp->getHolderComponent()->getObjController();
        UndoManager* undoManager_ = objCtrl->getUndoManager();

        currentObjectId = caller->getData()[Ids::identifier].toString();

        if (caller->getData().getType() == Ids::mass)
        {
            comp = new MassPropertiesComponent(objCtrl,
                                               caller->getData(), undoManager_);
        }
        else if (caller->getData().getType() == Ids::port)
        {
            comp = new PortPropertiesComponent(objCtrl,
                                               caller->getData(), undoManager_);
        }
        else if (caller->getData().getType() == Ids::resonators)
        {
            comp = new ResonatorPropertiesComponent(objCtrl,
                                                    caller->getData(), undoManager_);
        }
        else if (caller->getData().getType() == Ids::ground)
        {
            comp = new GroundPropertiesComponent(objCtrl,
                                                 caller->getData(), undoManager_);
        }
        else if (caller->getData().getType() == Ids::link
            || caller->getData().getType() == Ids::touch
            || caller->getData().getType() == Ids::pluck
            || caller->getData().getType() == Ids::pulsetouch)
        {
            comp = new LinkPropertiesComponent(objCtrl,
                                               caller->getData(), undoManager_);
        }
        else if (caller->getData().getType() == Ids::waveguide)
        {
            comp = new WaveguidePropertiesComponent(objCtrl,
                                                    caller->getData(), undoManager_);
        }
        else if (caller->getData().getType() == Ids::junction)
        {
            comp = new JunctionPropertiesComponent(objCtrl,
                                                   caller->getData(), undoManager_);
        }
        else if (caller->getData().getType() == Ids::termination)
        {
            comp = new TerminationPropertiesComponent(objCtrl,
                                                      caller->getData(), undoManager_);
        }
        else if (caller->getData().getType() == Ids::audioout)
        {
            comp = new AudiooutPropertiesComponent(objCtrl,
                                                   caller->getData(), undoManager_);
        }
        else
        {
            comp = new Component();
            currentObjectId = "";
        }
        setContentOwned (comp, false);
    }
    else
    {
        setContentOwned(new EmptyComponent(), false);
    }
}