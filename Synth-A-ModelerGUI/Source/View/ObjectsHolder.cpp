/*
  ==============================================================================

    ObjectsHolder.cpp
    Created: 11 Apr 2012 5:10:20pm
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
#include "../Models/MDLFile.h"
#include "../Models/ObjectFactory.h"
#include "ContentComp.h"
#include "ObjectComponent.h"
#include "LinkComponent.h"
#include "SelectableObject.h"
#include "../Controller/ObjController.h"
#include "VariablesPanel.h"

#include "ObjectsHolder.h"
#include "AudioOutConnector.h"

ObjectsHolder::ObjectsHolder(ObjController& objController_) 
: objController(objController_), mdlFile(nullptr), 
    dragging(false), isDrawingObjectNames(false)
{
    setSize(100, 100);
    setWantsKeyboardFocus(true);
}

ObjectsHolder::~ObjectsHolder()
{
    if (mdlFile != nullptr)
    {
        mdlFile->removeChangeListener(this);
    }
}

void ObjectsHolder::paint(Graphics& g)
{
    g.fillAll(Colours::white);

    g.setColour(Colours::black);
    
    if(isDrawingObjectNames)
    {
        for(int i = 0; i < objController.getNumObjects(); ++i)
        {
            ObjectComponent* oc = objController.getObjectUnchecked(i);
            Point<int> tl = oc->getBounds().getTopLeft();
            g.drawSingleLineText(oc->getData()[Ids::identifier].toString(), tl.x, tl.y, Justification::left);
        }
        for(int i = 0; i < objController.getNumLinks(); ++i)
        {
            LinkComponent* lc = objController.getLinkUnchecked(i);
            Point<int> tl = lc->getBounds().getCentre();
            g.drawSingleLineText(lc->getData()[Ids::identifier].toString(), tl.x, tl.y-5, Justification::left);
        }

    }
}

void ObjectsHolder::resized()
{
    updateComponents();
}

void ObjectsHolder::changeListenerCallback(ChangeBroadcaster*)
{
    updateComponents();
}

void ObjectsHolder::updateComponents()
{
    // TODO Find faster solution for updating componenans
    int i;
    Array<LinkComponent*> links;
    Array<AudioOutConnector*> aocs;
    for (i = getNumChildComponents(); --i >= 0;)
    {
        ObjectComponent * const bobj = dynamic_cast<ObjectComponent*> (getChildComponent(i));

        if (bobj != nullptr)
            bobj->update();
        
        LinkComponent * const lobj = dynamic_cast<LinkComponent*> (getChildComponent(i));
        
        if (lobj != nullptr)
        {
            links.add(lobj);
        }
        AudioOutConnector * const aobj = dynamic_cast<AudioOutConnector*> (getChildComponent(i));
        
        if (aobj != nullptr)
        {
            aocs.add(aobj);
        }
    }
    for (i = 0; i < links.size(); ++i)
    {
        links[i]->update();
        links[i]->toBack();
    }
    for (i = 0; i < aocs.size(); ++i)
    {
        aocs[i]->update();
        aocs[i]->toBack();
    }

}

void ObjectsHolder::mouseDrag(const MouseEvent& e)
{
    lassoComp.toFront (false);
    lassoComp.dragLasso (e);
}

void ObjectsHolder::mouseUp(const MouseEvent& e)
{
    if (e.mouseWasClicked())
    {
    }
    
    if (e.mouseWasClicked() && ! e.mods.isAnyModifierKeyDown())
    {
        // object changed
        objController.getSelectedObjects().deselectAll();
    }
    lassoComp.endLasso();
}

void ObjectsHolder::mouseDown(const MouseEvent& e)
{
    if (e.mods.isPopupMenu() && objController.getSelectedObjects().getNumSelected() == 2)
    {
        String startObj;
        String endObj;
        if (objController.getSelectedObjects().getNumSelected() == 2)
        {
            ObjectComponent* oc1 = dynamic_cast<ObjectComponent*>(objController.getSelectedObjects().getSelectedItem(0));
            ObjectComponent* oc2 = dynamic_cast<ObjectComponent*>(objController.getSelectedObjects().getSelectedItem(1));
            if(oc1 != nullptr && oc2 != nullptr)
            {
                startObj = oc1->getData().getProperty(Ids::identifier).toString();
                endObj = oc2->getData().getProperty(Ids::identifier).toString();
                DBG(String("Link: ") + startObj + String(", ") + endObj);
                showLinkPopupMenu(startObj, endObj);
            }
        }

    }
    else if (e.mods.isPopupMenu())
    {
        showContextMenu(e.getMouseDownPosition());
    }
    else
    {
        addAndMakeVisible(&lassoComp);
        lassoComp.beginLasso(e, this);
    }
}

void ObjectsHolder::setMDLFile(MDLFile* newMDLFile)
{
    if (newMDLFile != mdlFile && newMDLFile != nullptr)
    {
        mdlFile = newMDLFile;
        mdlFile->addChangeListener(this);
        objController.loadComponents(this);
    }
}

static const int dx = 20;
static const int dy = 20;
static const int dxfine = 5;
static const int dyfine = 5;
bool ObjectsHolder::dispatchMenuItemClick(const ApplicationCommandTarget::InvocationInfo& info)
{
    Point<int> mp = getMouseXYRelative();

    if (mp.x < 0)
        mp.x = 0;
    else if (mp.x > getWidth())
        mp.x = getWidth();
    if (mp.y < 0)
        mp.y = 0;
    else if (mp.y > getHeight())
        mp.y = getHeight();

    String startObj;
    String endObj;
    if(objController.getSelectedObjects().getNumSelected() == 2)
    {
        ObjectComponent* oc1 = dynamic_cast<ObjectComponent*> (objController.getSelectedObjects().getSelectedItem(0));
        ObjectComponent* oc2 = dynamic_cast<ObjectComponent*> (objController.getSelectedObjects().getSelectedItem(1));
        if (oc1 != nullptr && oc2 != nullptr)
        {
            startObj = oc1->getData().getProperty(Ids::identifier).toString();
            endObj = oc2->getData().getProperty(Ids::identifier).toString();
        }
    }
    switch (info.commandID)
    {
    case StandardApplicationCommandIDs::cut:
        objController.cut(this);
        break;
    case StandardApplicationCommandIDs::copy:
        objController.copySelectedToClipboard();
        break;
    case StandardApplicationCommandIDs::paste:
        objController.paste(this);
        break;
    case StandardApplicationCommandIDs::selectAll:
        objController.selectAll(true);
        break;
    case StandardApplicationCommandIDs::deselectAll:
        objController.selectAll(false);
        break;
    case StandardApplicationCommandIDs::del:
        deleteSelectedObjects();
        break;
    case CommandIDs::defineVariables:
        VariablesPanel::show(mdlFile->mdlRoot, &mdlFile->getUndoMgr());
        break;
    case CommandIDs::segmentedConnectors:
        // TODO: implement segmented connectors
        StoredSettings::getInstance()->setIsSegmentedConnectors(!StoredSettings::getInstance()->getIsSegmentedConnectors());
        break;
    case CommandIDs::reverseDirection:
        objController.reverseLinkDirection();
        break;

    case CommandIDs::insertMass:
        objController.addNewObject(this,
                                   ObjectFactory::createNewObjectTree(Ids::mass,
                                                                      objController.getNewNameForObject(Ids::mass),
                                                                      mp.x, mp.y));
        break;
    case CommandIDs::insertGround:
        objController.addNewObject(this,
                                   ObjectFactory::createNewObjectTree(Ids::ground,
                                                                      objController.getNewNameForObject(Ids::ground),
                                                                      mp.x, mp.y));
        break;
    case CommandIDs::insertResonator:
        objController.addNewObject(this,
                                   ObjectFactory::createNewObjectTree(Ids::resonator,
                                                                      objController.getNewNameForObject(Ids::resonator),
                                                                      mp.x, mp.y));
        break;
    case CommandIDs::insertPort:
        objController.addNewObject(this,
                                   ObjectFactory::createNewObjectTree(Ids::port,
                                                                      objController.getNewNameForObject(Ids::port),
                                                                      mp.x, mp.y));
        break;

    case CommandIDs::insertLink:
        
        objController.addNewLinkIfPossible(this,
                                           ObjectFactory::createNewLinkObjectTree(Ids::link,
                                                                                  objController.getNewNameForObject(Ids::link),
                                                                                  startObj, endObj));
        break;
    case CommandIDs::insertTouch:
        objController.addNewLinkIfPossible(this,
                                           ObjectFactory::createNewLinkObjectTree(Ids::touch,
                                                                                  objController.getNewNameForObject(Ids::touch),
                                                                                  startObj, endObj));
        break;
    case CommandIDs::insertPluck:
        objController.addNewLinkIfPossible(this,
                                           ObjectFactory::createNewLinkObjectTree(Ids::pluck,
                                                                                  objController.getNewNameForObject(Ids::pluck),
                                                                                  startObj, endObj));
        break;

    case CommandIDs::insertAudioOutput:
        objController.addNewObject(this,
                                   ObjectFactory::createNewObjectTree(Ids::audioout,
                                                                      objController.getNewNameForObject(Ids::audioout),
                                                                      mp.x, mp.y));
        break;
    case CommandIDs::insertAudioConnection:
        objController.addNewAudioConnection(this);
        break;
    case CommandIDs::insertWaveguide:
//        objController.addNewObject(this,
//                                   ObjectFactory::createNewObjectTree(Ids::waveguide,
//                                                                      objController.getNewNameForObject(Ids::waveguide),
//                                                                      mp.x, mp.y));
        DBG("Waveguide not implemented!")
        break;
    case CommandIDs::insertTermination:
//        objController.addNewObject(this,
//                                   ObjectFactory::createNewObjectTree(Ids::termination,
//                                                                      objController.getNewNameForObject(Ids::termination),
//                                                                      mp.x, mp.y));
        DBG("Termination not implemented!")
        break;
    case CommandIDs::moveUp:
        objController.moveSelectedComps(0, -dy);
        break;
    case CommandIDs::moveUpFine:
        objController.moveSelectedComps(0, -dyfine);
        break;
    case CommandIDs::moveDown:
        objController.moveSelectedComps(0, dy);
        break;
    case CommandIDs::moveDownFine:
        objController.moveSelectedComps(0, dyfine);
        break;
    case CommandIDs::moveLeft:
        objController.moveSelectedComps(-dx, 0);
        break;
    case CommandIDs::moveLeftFine:
        objController.moveSelectedComps(-dxfine, 0);
        break;
    case CommandIDs::moveRight:
        objController.moveSelectedComps(dx, 0);
        break;
    case CommandIDs::moveRightFine:
        objController.moveSelectedComps(dxfine, 0);
        break;
    default:
        return false;
    }
    objController.getUndoManager()->beginNewTransaction();
    return true;
}

void ObjectsHolder::showContextMenu(const Point<int> mPos)
{
    PopupMenu m;
    m.addSectionHeader("Insert...");
    m.addItem(1, "mass");
    m.addItem(2, "ground");
    m.addItem(3, "resonator");
    m.addItem(4, "port");
    m.addSeparator();
    m.addItem(5, "audioout");
    m.addSeparator();
    m.addItem(6, "waveguide");
    m.addItem(7, "termination");

    const int r = m.show();

    if (r == 1)
    {
        objController.addNewObject(this,
                                   ObjectFactory::createNewObjectTree(Ids::mass,
                                                                      objController.getNewNameForObject(Ids::mass),
                                                                      mPos.x, mPos.y));
    }
    else if (r == 2)
    {
        objController.addNewObject(this,
                                   ObjectFactory::createNewObjectTree(Ids::ground,
                                                                      objController.getNewNameForObject(Ids::ground),
                                                                      mPos.x, mPos.y));
    }
    else if (r == 3)
    {
        objController.addNewObject(this,
                                   ObjectFactory::createNewObjectTree(Ids::resonator,
                                                                      objController.getNewNameForObject(Ids::resonator),
                                                                      mPos.x, mPos.y));
    }
    else if (r == 4)
    {
        objController.addNewObject(this,
                                   ObjectFactory::createNewObjectTree(Ids::port,
                                                                      objController.getNewNameForObject(Ids::port),
                                                                      mPos.x, mPos.y));
    }
    else if (r == 5)
    {
        objController.addNewObject(this,
                                   ObjectFactory::createNewObjectTree(Ids::audioout,
                                                                      objController.getNewNameForObject(Ids::audioout),
                                                                      mPos.x, mPos.y));
    }
    else if (r == 6)
    {
        DBG("Waveguide not implemented!")
//        objController.addNewObject(this,
//                                   ObjectFactory::createNewObjectTree(Ids::waveguide,
//                                                                      objController.getNewNameForObject(Ids::waveguide),
//                                                                      mPos.x, mPos.y));
    }
    else if (r == 7)
    {
        DBG("Termination not implemented!")
//        objController.addNewObject(this,
//                                   ObjectFactory::createNewObjectTree(Ids::termination,
//                                                                      objController.getNewNameForObject(Ids::termination),
//                                                                      mPos.x, mPos.y));
    }
}

void ObjectsHolder::showLinkPopupMenu(String so, String eo)
{
	PopupMenu m;
	m.addItem (1, "Add link");
	m.addItem (2, "Add touch");
	m.addItem (3, "Add pluck");
    m.addSeparator();
    m.addItem (4, "Add audio connection");
	const int r = m.show();

	if (r == 1)
	{
		DBG("Add link");
        objController.addNewLinkIfPossible(this,
                                           ObjectFactory::createNewLinkObjectTree(Ids::link,
                                                                                  objController.getNewNameForObject(Ids::link),
                                                                                  so, eo));
		return;
	}
	else if (r == 2)
	{
		DBG("Add touch");
        objController.addNewLinkIfPossible(this,
                                           ObjectFactory::createNewLinkObjectTree(Ids::touch,
                                                                                  objController.getNewNameForObject(Ids::touch),
                                                                                  so, eo));
	}
	else if (r == 3)
	{
		DBG("Add pluck");
        objController.addNewLinkIfPossible(this,
                                           ObjectFactory::createNewLinkObjectTree(Ids::pluck,
                                                                                        objController.getNewNameForObject(Ids::pluck),
                                                                                        so, eo));
	}
    else if (r == 4)
    {
        DBG("Add audio connection");
        objController.addNewAudioConnection(this);
    }
}

void ObjectsHolder::showAudioConnectionPopupMenu()
{
    PopupMenu m;
	m.addItem (1, "Add audio connection");
    const int r = m.show();
    if( r == 1)
    {
        objController.addNewAudioConnection(this);
    }
}

void ObjectsHolder::editObjectProperties(BaseObjectComponent* oc)
{
    isDrawingObjectNames = true;
    toFront(false);
    repaint();
    objController.editObjectProperties(oc, &mdlFile->getUndoMgr());
    isDrawingObjectNames = false;
    toBack();
    repaint();
}

void ObjectsHolder::findLassoItemsInArea (Array <SelectableObject*>& results, const Rectangle<int>& lasso)
{
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        SelectableObject* const e = dynamic_cast <SelectableObject*> (getChildComponent (i));
        LinkComponent* const lc = dynamic_cast<LinkComponent*>(e);
        AudioOutConnector* const aoc = dynamic_cast<AudioOutConnector*>(e);
        bool isIntersecting;
        if(lc != nullptr)
            isIntersecting = lasso.contains(lc->getIntersectioBounds());
        else if(aoc != nullptr)
            isIntersecting = lasso.contains(aoc->getIntersectioBounds());
        else
            isIntersecting = getChildComponent(i)->getBounds().intersects (lasso);
        if (e != 0 && isIntersecting)
        {
            results.add (e);
        }
    }
}

SelectedItemSet <SelectableObject*>& ObjectsHolder::getLassoSelection()
{
    return objController.getSelectedObjects();
}


//const Rectangle<int> ObjectsHolder::getComponentArea() const
//{
////    if (document.isFixedSize())
////    {
////        return Rectangle<int> ((getWidth() - document.getInitialWidth()) / 2,
////                               (getHeight() - document.getInitialHeight()) / 2,
////                               document.getInitialWidth(),
////                               document.getInitialHeight());
////    }
////    else
////    {
////        return Rectangle<int> (editorEdgeGap, editorEdgeGap,
////                               getWidth() - editorEdgeGap * 2,
////                               getHeight() - editorEdgeGap * 2);
////    }
//}

void ObjectsHolder::deleteSelectedObjects()
{
    objController.removeSelectedObjects(this);
}