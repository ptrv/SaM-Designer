/*
  ==============================================================================

    PropertiesWindow.cpp
    Created: 23 Jul 2013 10:12:43pm
    Author:  peter

  ==============================================================================
*/

#include "../Application/CommonHeaders.h"
#include "../Application/MainWindow.h"

#include "ContentComp.h"
#include "ObjectsHolder.h"
#include "../Controller/ObjController.h"
#include "../Controller/ObjController.h"
#include "../Graph/Node.h"
#include "BaseObjectComponent.h"

#include "PropertiesWindow.h"

using namespace synthamodeler;

PropertiesWindow::PropertiesWindow()
:
DocumentWindow("Properties", Colour::greyLevel (0.92f), DocumentWindow::closeButton, true)
{
    Desktop::getInstance().addFocusChangeListener(this);
}

PropertiesWindow::~PropertiesWindow()
{
    
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
}
void PropertiesWindow::closeButtonPressed()
{
    setVisible(false);
}

static String currentObjectId;
void PropertiesWindow::updateProperties()
{
    Component* comp;
    ;
    if (BaseObjectComponent * caller = dynamic_cast<BaseObjectComponent*> (currentSelection->getSelectedItem(0)))
    {
        ObjController& objCtrl = currentContentComp->getHolderComponent()->getObjController();
        UndoManager* undoManager_ = objCtrl.getUndoManager();

        currentObjectId = caller->getData()[Ids::identifier].toString();

        if (caller->getData().getType() == Ids::mass)
        {
//            comp = new MassPropertiesComponent(this, objCtrl,
//                                               caller->getData(), undoManager_);
        }
        else if (caller->getData().getType() == Ids::port)
        {
//            comp = new PortPropertiesComponent(this, objCtrl,
//                                               caller->getData(), undoManager_);
        }
        else if (caller->getData().getType() == Ids::resonators)
        {
//            comp = new ResonatorPropertiesComponent(this, objCtrl,
//                                                    caller->getData(), undoManager_);
        }
        else if (caller->getData().getType() == Ids::ground)
        {
//            comp = new GroundPropertiesComponent(this, objCtrl,
//                                                 caller->getData(), undoManager_);
        }
        else if (caller->getData().getType() == Ids::link
            || caller->getData().getType() == Ids::touch
            || caller->getData().getType() == Ids::pluck
            || caller->getData().getType() == Ids::pulsetouch)
        {
//            comp = new LinkPropertiesComponent(this, objCtrl,
//                                               caller->getData(), undoManager_);
        }
        else if (caller->getData().getType() == Ids::waveguide)
        {
//            comp = new WaveguidePropertiesComponent(this, objCtrl,
//                                                    caller->getData(), undoManager_);
        }
        else if (caller->getData().getType() == Ids::junction)
        {
//            comp = new JunctionPropertiesComponent(this, objCtrl,
//                                                   caller->getData(), undoManager_);
        }
        else if (caller->getData().getType() == Ids::termination)
        {
//            comp = new TerminationPropertiesComponent(this, objCtrl,
//                                                      caller->getData(), undoManager_);
        }
        else if (caller->getData().getType() == Ids::audioout)
        {
//            comp = new AudiooutPropertiesComponent(this, objCtrl,
//                                                   caller->getData(), undoManager_);
        }
        else
        {
            comp = new Component();
            currentObjectId = "";
        }
//        setContentOwned(comp, true);
    }
}