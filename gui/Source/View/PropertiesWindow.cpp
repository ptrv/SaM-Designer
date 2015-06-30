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

#include "PropertiesWindow.h"

#include "Application/CommonHeaders.h"
#include "Application/MainWindow.h"

#include "Controller/ObjController.h"

#include "Graph/Node.h"

#include "ContentComp.h"
#include "ObjectsHolder.h"
#include "BaseObjectComponent.h"
#include "ObjectPropertiesComponent.h"
#include "SelectableObject.h"
#include "ObjectComponent.h"
#include "LinkComponent.h"
#include "AudioOutConnector.h"

#include "Utilities/ObjectsHelper.h"


using namespace synthamodeler;

typedef ObjectPropertiesComponent OPC;

class EmptyComponent : public Component
{
public:
    EmptyComponent() : Component(), textToShow(TRANS("No object selected!"))
    {
        setComponentID("EmptyComponent");
    }
    EmptyComponent(const String& compName) : Component(compName) {}
    void paint(Graphics& g)
    {
        // The following code is basically the same as
        // Graphics::drawMultiLineText but with centered text
        int startX = 20;
        int baselineY = getHeight() / 2;
        int maximumLineWidth = getWidth() - 40;

        LowLevelGraphicsContext& context = g.getInternalContext();

        if (textToShow.isNotEmpty() && startX < context.getClipBounds().getRight())
        {
            GlyphArrangement arr;
            arr.addJustifiedText(context.getFont(), textToShow,
                                 (float) startX, (float) baselineY,
                                 (float) maximumLineWidth,
                                 Justification::centred);
            arr.draw(g);
        }
    }
    void setText(const String& newText) { textToShow = newText; /*repaint();*/ }
private:
    String textToShow;
};

PropertiesWindow::PropertiesWindow()
:
DocumentWindow(TRANS("Properties"),
               Colour::greyLevel (0.92f),
               DocumentWindow::closeButton | DocumentWindow::minimiseButton,
               false),
    currentContentComp(nullptr),
    currentSelection(nullptr)
{
    setSize(400, 500);

    setUsingNativeTitleBar(true);

    const StoredSettings& settings = *StoredSettings::getInstance();
    // restore the last size and position from our settings file..
	restoreWindowStateFromString (
        settings.getProps().getValue("lastPropertiesWindowPos"));

    setContentOwned(new EmptyComponent(), false);
    setResizable(true, true);

    setAlwaysOnTop(settings.getIsPropertiesWindowAlwaysOnTop());

	commandManager->registerAllCommandsForTarget(this);

	addKeyListener(commandManager->getKeyMappings());

    setComponentID("PropertiesWindow");
}

PropertiesWindow::~PropertiesWindow()
{
    StoredSettings::getInstance()->getProps()
        .setValue ("lastPropertiesWindowPos", getWindowStateAsString());

    clearContentComponent();
}

void PropertiesWindow::makeVisible(bool shouldBeVisible)
{
    if(shouldBeVisible)
    {
        if (!isOnDesktop())
        {
            addToDesktop();
        }

        setVisible(true);

        updateProperties();

        Component* cc = getContentComponent();
        if (cc != nullptr && cc->getComponentID().compare("EmptyComponent") != 0)
        {
            setWantsKeyboardFocus(false);
        }

        if (OPC * const opc = dynamic_cast<OPC*> (cc))
        {
            if (opc->canceledEditing)
            {
                opc->canceledEditing = false;
            }
        }

        if (isMinimised())
        {
            setMinimised(false);
        }

        if (!isAlwaysOnTop())
        {
            toFront(true);
        }
    }
    else
    {
        setVisible(false);
        removeFromDesktop();
    }
}

void PropertiesWindow::setCurrentActiveWindow(MainAppWindow& maw)
{
    if(maw.isActiveWindow())
    {
        if (ContentComp * cc = static_cast<ContentComp*> (maw.getContentComponent()))
        {
            if (currentContentComp != cc)
            {
//                DBG("current active window changed");
                currentContentComp = cc;
                currentSelection = &cc->getHolderComponent()->getObjController()
                    .getSelectedObjects();
                currentSelection->sendChangeMessage();
            }
        }
    }
}

void PropertiesWindow::changeListenerCallback(ChangeBroadcaster* /*source*/)
{
    if (isVisible())
    {
        updateProperties();
    }
}
void PropertiesWindow::closeButtonPressed()
{
    makeVisible(false);
}

bool PropertiesWindow::keyPressed(const KeyPress& kp)
{
    if (kp == KeyPress::escapeKey)
    {
        closeButtonPressed();
        return true;
    }
    return false;
}

void PropertiesWindow::updateProperties()
{
    if (currentSelection == nullptr)
    {
        return;
    }

    if(currentSelection->getNumSelected() > 0)
    {
        Array<ValueTree> datas;
        StringArray audioSourceIds;
        const Array<SelectableObject*> selectedItems = currentSelection->getItemArray();
        Identifier selectedId;
        for (SelectableObject* const selectedItem : selectedItems)
        {
            if(BaseObjectComponent* boc = dynamic_cast<BaseObjectComponent*>(selectedItem))
            {
                // get first id type. This is the type for a multiple selection
                if (datas.size() == 0)
                {
                    selectedId = boc->getData().getType();
                }

                if (selectedId == boc->getData().getType())
                {
                    datas.add(boc->getData());
                }
                else
                {
                    showEmptyComponent(TRANS("Please select objects of the same type "
                                             "in order to edit their properties!"));
                    return;
                }
            }
            else if(AudioOutConnector* aoc = dynamic_cast<AudioOutConnector*>(selectedItem))
            {
                if (datas.size() == 0)
                {
                    selectedId = Ids::audioconnector;
                }
                else if(selectedId != Ids::audioconnector)
                {
                    showEmptyComponent(TRANS("Please select objects of the same type "
                                             "in order to edit their properties!"));
                    return;
                }

                String sourceId;
                if (ObjectComponent * const oc = dynamic_cast<ObjectComponent*>(aoc->getSourceObject()))
                {
                    sourceId = oc->getData()[Ids::identifier].toString();
                }
                else if (LinkComponent * const lc = dynamic_cast<LinkComponent*>(aoc->getSourceObject()))
                {
                    sourceId = lc->getData()[Ids::identifier].toString();
                }

                audioSourceIds.add(sourceId);
                datas.add(aoc->getAudioObject()->getData());
            }
        }

        if (datas.size() == 0)
        {
            return;
        }
        
        Component* comp;
        ObjController& objCtrl = currentContentComp->getHolderComponent()->getObjController();
        UndoManager* undoManager_ = &objCtrl.getUndoManager();

        if (selectedId == Ids::mass)
        {
            comp = new MassPropertiesComponent(objCtrl, datas, undoManager_);
        }
        else if (selectedId == Ids::port)
        {
            comp = new PortPropertiesComponent(objCtrl, datas, undoManager_);
        }
        else if (selectedId == Ids::resonators)
        {
            comp = new ResonatorPropertiesComponent(objCtrl, datas, undoManager_);
        }
        else if (selectedId == Ids::ground)
        {
            comp = new GroundPropertiesComponent(objCtrl, datas, undoManager_);
        }
        else if (selectedId == Ids::link
            || selectedId == Ids::touch
            || selectedId == Ids::pluck
            || selectedId == Ids::pulsetouch)
        {
            comp = new LinkPropertiesComponent(objCtrl, datas, undoManager_);
        }
        else if (selectedId == Ids::waveguide)
        {
            comp = new WaveguidePropertiesComponent(objCtrl, datas, undoManager_);
        }
        else if (selectedId == Ids::junction)
        {
            comp = new JunctionPropertiesComponent(objCtrl, datas, undoManager_);
        }
        else if (selectedId == Ids::termination)
        {
            comp = new TerminationPropertiesComponent(objCtrl, datas, undoManager_);
        }
        else if (selectedId == Ids::audioout)
        {
            comp = new AudiooutPropertiesComponent(objCtrl, datas, undoManager_);
        }
        else if (selectedId == Ids::audioconnector)
        {
            comp = new GainComponent(audioSourceIds, datas, undoManager_);
        }
        else
        {
            comp = new EmptyComponent();
        }
        setWantsKeyboardFocus(false);
        setContentOwned(comp, false);
    }
    else
    {
        setContentOwned(new EmptyComponent(), false);
    }
}

void PropertiesWindow::mdlChanged()
{
    if (isVisible())
    {
        if (OPC * opc = dynamic_cast<OPC*> (getContentComponent()))
        {
            opc->readValues();
        }
    }
}

void PropertiesWindow::showEmptyComponent(const String& textToShow)
{
    EmptyComponent* ec = new EmptyComponent();
    if (textToShow.isNotEmpty())
    {
        ec->setText(textToShow);
    }
    setWantsKeyboardFocus(true);
    setContentOwned(ec, false);
    setName(TRANS("Properties"));
}
//==============================================================================
void PropertiesWindow::valueTreePropertyChanged (ValueTree& /*tree*/,
                                                 const Identifier& /*property*/)
{
    mdlChanged();
}

void PropertiesWindow::valueTreeChildAdded (ValueTree& /*parentTree*/,
                                            ValueTree& /*childWhichHasBeenAdded*/)
{
	mdlChanged();
}

void PropertiesWindow::valueTreeChildRemoved (ValueTree& /*parentTree*/,
                                              ValueTree& /*childWhichHasBeenRemoved*/,
                                              int /*indexFromWhichChildWasRemoved*/)
{
	mdlChanged();
}

void PropertiesWindow::valueTreeChildOrderChanged (ValueTree& /*parentTree*/,
                                                   int /*oldIndex*/, int /*newIndex*/)
{
	mdlChanged();
}

void PropertiesWindow::valueTreeParentChanged (ValueTree& /*tree*/)
{
    mdlChanged();
}

//==============================================================================
ApplicationCommandTarget* PropertiesWindow::getNextCommandTarget()
{
    return findFirstTargetParentComponent();
}

void PropertiesWindow::getAllCommands (Array <CommandID>& /*commands*/)
{
}

void PropertiesWindow::getCommandInfo (CommandID /*commandID*/,
                                       ApplicationCommandInfo& /*result*/)
{
}

bool PropertiesWindow::perform (const InvocationInfo& /*info*/)
{
	return false;
}
