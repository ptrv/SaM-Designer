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
#include "ContentComp.h"
#include "ObjectComponent.h"
#include "../Controller/ObjController.h"
#include "VariablesPanel.h"

#include "ObjectsHolder.h"

ObjectsHolder::ObjectsHolder(ObjController& objController_)
: objController(objController_),
mdlFile(nullptr),
dragging(false),
isMultipleSelection(false)
{
    setSize(100, 100);
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
    if (dragging)
    {
        //		setAlwaysOnTop(true);
        float x = draggingStart.x;
        float y = draggingStart.y;
        float w = draggingActual.x; // - x;
        float h = draggingActual.y; // - y;
        g.drawRect(x, y, w, h);
    }
}

void ObjectsHolder::resized()
{
    //	DBG(getBounds().toString());
    //	setBounds(0,0, getParentComponent()->getWidth(), getParentComponent()->getHeight());
    updateComponents();
}

void ObjectsHolder::changeListenerCallback(ChangeBroadcaster*)
{
    updateComponents();
}

void ObjectsHolder::updateComponents()
{
    int i;
    for (i = getNumChildComponents(); --i >= 0;)
    {
        ObjectComponent * const bobj = dynamic_cast<ObjectComponent*> (getChildComponent(i));

        if (bobj != nullptr)
            bobj->update();
    }

}

void ObjectsHolder::mouseDrag(const MouseEvent& e)
{
    lassoComp.toFront (false);
    lassoComp.dragLasso (e);
//    
//    draggingActual.x = e.getOffsetFromDragStart().x;
//    draggingActual.y = e.getOffsetFromDragStart().y;
//
//    dragging = true;
//    repaint();
}

void ObjectsHolder::mouseUp(const MouseEvent& e)
{
    if (e.mouseWasClicked())
    {

    }
    lassoComp.endLasso();
    
    if (e.mouseWasClicked() && ! e.mods.isAnyModifierKeyDown())
    {
        // object changed
//        int x = draggingStart.x;
//        int y = draggingStart.y;
//        int w = draggingActual.x; // - x;
//        int h = draggingActual.y; // - y;
//
//        Rectangle<int> tmpRect(x, y, w, h);
//        selectedObjects = objController.selectObjectsWithinRectagle(tmpRect);
//        if (selectedObjects.size() > 0)
//            isMultipleSelection = true;
//        dragging = false;
//        repaint();
        
        deselectAllSelectedObjects();
        selectedObjects2.deselectAll();
    }
}

void ObjectsHolder::mouseDown(const MouseEvent& e)
{
    if (e.mods.isPopupMenu())
    {
        showContextMenu(e.getMouseDownPosition());
    }
    else
    {
        addAndMakeVisible(&lassoComp);
        lassoComp.beginLasso(e, this);
    }
//    draggingStart.x = e.getMouseDownX();
//    draggingStart.y = e.getMouseDownY();
    //	dragging = true;

//    for (int i = getNumChildComponents(); --i >= 0;)
//    {
//        ObjectComponent* oc = dynamic_cast<ObjectComponent*> (getChildComponent(i));
//        oc->setSelected(false);
//    }
//    isMultipleSelection = false;
//    selectedObjects.clear();
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

void ObjectsHolder::moveObjectsData(Point<int> offset)
{
    objController.moveObjects(this, offset);
}

void ObjectsHolder::moveObjectComponents(Point<int> offset)
{
    //	for (int i = 0; i < getNumChildComponents(); ++i) {
    //		ObjectComponent* oc = dynamic_cast<ObjectComponent*>(getChildComponent(i));
    //		if(oc->selected() && oc != ObjectComponent::isLastClicked)
    //		{
    //			oc->mouseDragPassive(offset);
    //		}
    //	}
    for (int i = 0; i < selectedObjects.size(); ++i)
    {
        if (selectedObjects[i] != ObjectComponent::isLastClicked)
        {
            selectedObjects[i]->mouseDragPassive(offset);
        }
    }

}

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

    switch (info.commandID)
    {
    case StandardApplicationCommandIDs::cut:
        // TODO: implement cut
        break;
    case StandardApplicationCommandIDs::copy:
        // TODO: implement copy
        break;
    case StandardApplicationCommandIDs::paste:
        // TODO: implement paste
        break;
    case StandardApplicationCommandIDs::selectAll:
        selectedObjects.clear();
        objController.selectAll(true);
        isMultipleSelection = true;
        break;
    case StandardApplicationCommandIDs::deselectAll:
        objController.selectAll(false);
        isMultipleSelection = false;
        selectedObjects.clear();
        break;
    case StandardApplicationCommandIDs::del:
        objController.removeObject(this);
        break;
    case CommandIDs::defineVariables:
        VariablesPanel::show(mdlFile->mdlRoot, &mdlFile->getUndoMgr());
        break;
    case CommandIDs::segmentedConnectors:
        // TODO: implement segmented connectors
        StoredSettings::getInstance()->setIsSegmentedConnectors(!StoredSettings::getInstance()->getIsSegmentedConnectors());
        break;
        //    case CommandIDs::zoomIn:
        //    	// TODO: implement zoom in
        //    	break;
        //    case CommandIDs::zoomOut:
        //    	// TODO: implement zoom out
        //    	break;
        //    case CommandIDs::zoomNormal:
        //    	// TODO: implement zoom normal
        //    	break;
    case CommandIDs::reverseDirection:
        // TODO: implement reverse direction
        break;

    case CommandIDs::insertMass:
        objController.addObject(this, Ids::mass, mp.x, mp.y);
        break;
    case CommandIDs::insertGround:
        objController.addObject(this, Ids::ground, mp.x, mp.y);
        break;
    case CommandIDs::insertResonator:
        objController.addObject(this, Ids::resonator, mp.x, mp.y);
        break;
    case CommandIDs::insertPort:
        objController.addObject(this, Ids::port, mp.x, mp.y);
        break;

    case CommandIDs::insertLink:
        //    	objController.addObject(this, Ids::link, mp.x, mp.y);
        break;
    case CommandIDs::insertTouch:
        //    	objController.addObject(this, Ids::touch, mp.x, mp.y);
        break;
    case CommandIDs::insertPluck:
        //    	objController.addObject(this, Ids::pluck, mp.x, mp.y);
        break;

    case CommandIDs::insertAudioOutput:
        objController.addObject(this, Ids::audioout, mp.x, mp.y);
        break;
    case CommandIDs::insertWaveguide:
        objController.addObject(this, Ids::waveguide, mp.x, mp.y);
        break;
    case CommandIDs::insertTermination:
        objController.addObject(this, Ids::termination, mp.x, mp.y);
        break;
    default:
        return false;
    }
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
        objController.addObject(this, Ids::mass, mPos.x, mPos.y);
    }
    else if (r == 2)
    {
        objController.addObject(this, Ids::ground, mPos.x, mPos.y);
    }
    else if (r == 3)
    {
        objController.addObject(this, Ids::resonator, mPos.x, mPos.y);
    }
    else if (r == 4)
    {
        objController.addObject(this, Ids::port, mPos.x, mPos.y);
    }
    else if (r == 5)
    {
        objController.addObject(this, Ids::audioout, mPos.x, mPos.y);
    }
    else if (r == 6)
    {
        objController.addObject(this, Ids::waveguide, mPos.x, mPos.y);
    }
    else if (r == 7)
    {
        objController.addObject(this, Ids::termination, mPos.x, mPos.y);
    }
}

void ObjectsHolder::editObjectProperties(ObjectComponent* oc)
{
    objController.editObjectProperties(oc, &mdlFile->getUndoMgr());
}

//void ObjectsHolder::addSelectedObject(ObjectComponent* comp)
//{
//    selectedObjects.add(comp);
//    if (selectedObjects.size() > 1)
//        isMultipleSelection = true;
//    else
//        isMultipleSelection = false;
//}
//
//void ObjectsHolder::removeSelectedObject(ObjectComponent* comp)
//{
//    selectedObjects.removeFirstMatchingValue(comp);
//    if (selectedObjects.size() > 1)
//        isMultipleSelection = true;
//    else
//        isMultipleSelection = false;
//}
//
//void ObjectsHolder::deselectAllSelectedObjects()
//{
//    selectedObjects.clear();
//    isMultipleSelection = false;
//}
//
//ObjectComponent* ObjectsHolder::getSelectedObject(int index)
//{
//    return selectedObjects[index];
//}
//
//void ObjectsHolder::updateSelectedObjects()
//{
//    selectedObjects.clear();
//    for (int i = 0; i < getNumChildComponents(); ++i)
//    {
//        ObjectComponent * const oc = dynamic_cast<ObjectComponent*> (getChildComponent(i));
//        if (oc->selected())
//            selectedObjects.add(oc);
//    }
//}

void ObjectsHolder::findLassoItemsInArea (Array <ObjectComponent*>& results, const Rectangle<int>& lasso)
{
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        ObjectComponent* const e = dynamic_cast <ObjectComponent*> (getChildComponent (i));

        if (e != 0 && e->getBounds().intersects (lasso))
            results.add (e);
    }
}

SelectedItemSet <ObjectComponent*>& ObjectsHolder::getLassoSelection()
{
    return objController.;
}