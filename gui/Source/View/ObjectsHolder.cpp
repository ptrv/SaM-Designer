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
#include "SelectableObject.h"
#include "../Graph/FlowAlgorithm.h"
#include "../Graph/ForceDirectedFlowAlgorithm.h"
#include "../Graph/ForceBasedFlowAlgorithm.h"
#include "../Graph/DirectedGraph.h"
#include "../Graph/Node.h"
#include "BaseObjectComponent.h"
#include "ContentComp.h"
#include "ObjectComponent.h"
#include "LinkComponent.h"
#include "../Controller/ObjController.h"
#include "FaustcodePanel.h"
#include "RedrawOptionsPanel.h"
#include "SnapGridPainter.h"
#include "AudioOutConnector.h"
#include "CommentComponent.h"
#include "PropertiesWindow.h"

#include "ObjectsHolder.h"

using namespace synthamodeler;

ObjectsHolder::ObjectsHolder(ObjController& objController_)
: objController(objController_), mdlFile(nullptr),
  dragging(false), isDrawingObjectNames(false), showObjectNames(false),
  maxX(0), maxY(0), fcPanel(nullptr)
{
    snapGridPixels = StoredSettings::getInstance()->getSnapGridPixels();
    snapActive = StoredSettings::getInstance()->getIsSnapGridEnabled();
    snapShown = StoredSettings::getInstance()->getIsSnapGridShow();

    grid = new SnapGridPainter();

    setSize(100, 100);
    setWantsKeyboardFocus(true);

}

ObjectsHolder::~ObjectsHolder()
{
    grid = nullptr;
    fcPanel = nullptr;
}

void ObjectsHolder::paint(Graphics& g)
{
    g.fillAll(Colours::white);

    g.setColour(Colours::black);

    if(isDrawingObjectNames || showObjectNames)
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
    grid->draw(g);
}

void ObjectsHolder::resized()
{
    updateComponents();
    if(grid->updateFromDesign(*this))
        repaint();
}

void ObjectsHolder::changeListenerCallback(ChangeBroadcaster*)
{
    updateComponents();
    if(ContentComp* const cp = findParentComponentOfClass<ContentComp>())
        cp->resized();
}

void ObjectsHolder::updateComponents()
{
    // TODO Find faster solution for updating componenans
    int i;
    Array<LinkComponent*> links;
    Array<AudioOutConnector*> aocs;
    for (i = getNumChildComponents(); --i >= 0;)
    {
        if (ObjectComponent * const bobj = dynamic_cast<ObjectComponent*> (getChildComponent(i)))
        {
            checkExtent(bobj->getBounds());
            bobj->update();
        }
        else if (LinkComponent * const lobj = dynamic_cast<LinkComponent*> (getChildComponent(i)))
        {
            links.add(lobj);
        }
        else if (AudioOutConnector * const aobj = dynamic_cast<AudioOutConnector*> (getChildComponent(i)))
        {
            aocs.add(aobj);
        }
        else if (CommentComponent* const cobj = dynamic_cast<CommentComponent*> (getChildComponent(i)))
        {
            checkExtent(cobj->getBounds());
            cobj->update();
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
//    if(grid->updateFromDesign(*this))
//        repaint();
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

bool ObjectsHolder::keyPressed(const KeyPress& key)
{
    if(key == KeyPress::escapeKey)
    {
        if(isTimerRunning())
        {
            DBG("redraw cancelled!");
            stopTimer();
            return true;
        }
    }

    return false;
}

void ObjectsHolder::setMDLFile(MDLFile* newMDLFile)
{
    if (newMDLFile != mdlFile && newMDLFile != nullptr)
    {
        fcPanel = nullptr;
        mdlFile = newMDLFile;
        mdlFile->addChangeListener(this);
        objController.loadComponents(this);
    }
}

void ObjectsHolder::reloadMDLFile()
{
    fcPanel = nullptr;
    objController.destroy();
    objController.loadComponents(this);
}

namespace synthamodeler
{
static const int dx = 20;
static const int dy = 20;
static const int dxfine = 5;
static const int dyfine = 5;
}

int64 lastTime = 0.0f;
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
    case CommandIDs::defineFaustcode:
    {
        if(fcPanel == nullptr)
        {
            fcPanel = new FaustcodePanel(&objController, mdlFile->mdlRoot, &mdlFile->getUndoMgr());
        }
        fcPanel->show();
    }
        break;
    case CommandIDs::segmentedConnectors:
        StoredSettings::getInstance()->setIsSegmentedConnectors(!StoredSettings::getInstance()->getIsSegmentedConnectors());
        objController.setLinksSegmented(StoredSettings::getInstance()->getIsSegmentedConnectors());
        updateComponents();
        repaint();
        break;
    case CommandIDs::reverseDirection:
        objController.reverseLinkDirection();
        break;
    case CommandIDs::tidyObjects:
        objController.tidyUp();
        break;
    case CommandIDs::redrawCircle:
        redrawObjects(CommandIDs::redrawCircle);
        break;
    case CommandIDs::redrawForceDirected:
        redrawObjects(CommandIDs::redrawForceDirected);
        break;
    case CommandIDs::showRedrawOptions:
        RedrawOptionsPanel::show();
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
                                   ObjectFactory::createNewObjectTree(Ids::resonators,
                                                                      objController.getNewNameForObject(Ids::resonators),
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
    case CommandIDs::insertPulsetouch:
        objController.addNewLinkIfPossible(this,
                                           ObjectFactory::createNewLinkObjectTree(Ids::pulsetouch,
                                                                                  objController.getNewNameForObject(Ids::pulsetouch),
                                                                                  startObj, endObj));
        break;
    case CommandIDs::insertPluck:
        objController.addNewLinkIfPossible(this,
                                           ObjectFactory::createNewLinkObjectTree(Ids::pluck,
                                                                                  objController.getNewNameForObject(Ids::pluck),
                                                                                  startObj, endObj));
       
        break;
    case CommandIDs::insertWaveguide:
    {
        ObjectComponent* oc1 = objController.getObjectForId(startObj);
        ObjectComponent* oc2 = objController.getObjectForId(endObj);
        String tmpSo, tmpEo;
        if(oc1 != nullptr && oc2 != nullptr)
        {
            if (oc1->getPinPos().x < oc2->getPinPos().x)
            {
                tmpSo = oc1->getData()[Ids::identifier];
                tmpEo = oc2->getData()[Ids::identifier];
            }
            else
            {
                tmpSo = oc2->getData()[Ids::identifier];
                tmpEo = oc1->getData()[Ids::identifier];
            }
        }

        objController.addNewLinkIfPossible(this,
                                           ObjectFactory::createNewLinkObjectTree(Ids::waveguide,
                                                                                  objController.getNewNameForObject(Ids::waveguide),
                                                                                  tmpSo, tmpEo));
    }
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
    case CommandIDs::insertJunction:
        objController.addNewObject(this,
                                   ObjectFactory::createNewObjectTree(Ids::junction,
                                                                      objController.getNewNameForObject(Ids::junction),
                                                                      mp.x, mp.y));
        break;
    case CommandIDs::insertTermination:
        objController.addNewObject(this,
                                   ObjectFactory::createNewObjectTree(Ids::termination,
                                                                      objController.getNewNameForObject(Ids::termination),
                                                                      mp.x, mp.y));
        break;
    case CommandIDs::insertComment:
        objController.addNewComment(this,
                                    ObjectFactory::createNewObjectTree(Ids::comment,
                                                                       objController.getNewNameForObject(Ids::comment),
                                                                       mp.x, mp.y));
        break;
    case CommandIDs::moveUp:
        objController.moveSelectedComps(0, -synthamodeler::dy);
        break;
    case CommandIDs::moveUpFine:
        objController.moveSelectedComps(0, -synthamodeler::dyfine);
        break;
    case CommandIDs::moveDown:
        objController.moveSelectedComps(0, synthamodeler::dy);
        break;
    case CommandIDs::moveDownFine:
        objController.moveSelectedComps(0, synthamodeler::dyfine);
        break;
    case CommandIDs::moveLeft:
        objController.moveSelectedComps(-synthamodeler::dx, 0);
        break;
    case CommandIDs::moveLeftFine:
        objController.moveSelectedComps(-synthamodeler::dxfine, 0);
        break;
    case CommandIDs::moveRight:
        objController.moveSelectedComps(synthamodeler::dx, 0);
        break;
    case CommandIDs::moveRightFine:
        objController.moveSelectedComps(synthamodeler::dxfine, 0);
        break;
    case CommandIDs::showObjectNames:
        if(! isDrawingObjectNames)
            showObjectNames = !showObjectNames;
        repaint();
        break;
    case CommandIDs::enableSnapToGrid:
        setSnappingGrid(getSnappingGridSize(),
                        !isSnapActive(false),
                        isSnapShown());
        break;

    case CommandIDs::showGrid:
        setSnappingGrid(getSnappingGridSize(),
                        isSnapActive(false),
                        !isSnapShown());
        break;
    case CommandIDs::showAudioConnections:
    {
        bool sac = StoredSettings::getInstance()->getShowAudioConnections();
        sac = !sac;
        StoredSettings::getInstance()->setShowAudioConnections(sac);
        objController.setAudioConnectionVisibility(sac);
    }
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
    m.addItem(1, "Mass");
    m.addItem(2, "Ground");
    m.addItem(3, "Resonator");
    m.addItem(4, "Port");
    m.addSeparator();
    m.addItem(5, "Audio Out");
    m.addSeparator();
    m.addItem(6, "Junction");
    m.addItem(7, "Termination");
    m.addSeparator();
//    bool commentEnabled = StoredSettings::getInstance()->getIsUsingMDLX();
//    bool isUsingMDLX = mdlFile != nullptr ? mdlFile->getFile().hasFileExtension(".mdlx") : false;
//    m.addItem(8, "Comment", (commentEnabled || isUsingMDLX));
    m.addItem(8, "Comment");

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
                                   ObjectFactory::createNewObjectTree(Ids::resonators,
                                                                      objController.getNewNameForObject(Ids::resonators),
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
//        DBG("Waveguide not implemented!")
        objController.addNewObject(this,
                                   ObjectFactory::createNewObjectTree(Ids::junction,
                                                                      objController.getNewNameForObject(Ids::junction),
                                                                      mPos.x, mPos.y));
    }
    else if (r == 7)
    {
//        DBG("Termination not implemented!")
        objController.addNewObject(this,
                                   ObjectFactory::createNewObjectTree(Ids::termination,
                                                                      objController.getNewNameForObject(Ids::termination),
                                                                      mPos.x, mPos.y));
    }
    else if (r == 8)
    {
        objController.addNewComment(this,
                                    ObjectFactory::createNewObjectTree(Ids::comment,
                                                                       objController.getNewNameForObject(Ids::comment),
                                                                       mPos.x, mPos.y));
    }
}

void ObjectsHolder::showLinkPopupMenu(String so, String eo)
{
	PopupMenu m;
    m.addSectionHeader("Add...");
	m.addItem (1, "Linear Link");
	m.addItem (2, "Touch Link");
	m.addItem (3, "Pulsetouch Link");
	m.addItem (4, "Pluck Link");
    m.addSeparator();
    m.addItem (5, "Waveguide");
    m.addSeparator();
    m.addItem (6, "Audio Connection");
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
		DBG("Add pulsetouch");
        objController.addNewLinkIfPossible(this,
                                           ObjectFactory::createNewLinkObjectTree(Ids::pulsetouch,
                                                                                  objController.getNewNameForObject(Ids::pulsetouch),
                                                                                  so, eo));
	}
	else if (r == 4)
	{
		DBG("Add pluck");
        objController.addNewLinkIfPossible(this,
                                           ObjectFactory::createNewLinkObjectTree(Ids::pluck,
                                                                                  objController.getNewNameForObject(Ids::pluck),
                                                                                  so, eo));
	}
	else if (r == 5)
	{
//        DBG("Waveguide not implemented!")
		DBG("Add waveguide");
        ObjectComponent* oc1 = objController.getObjectForId(so);
        ObjectComponent* oc2 = objController.getObjectForId(eo);
        String tmpSo, tmpEo;
        if(oc1 != nullptr && oc2 != nullptr)
        {
            if(oc1->getPinPos().x < oc2->getPinPos().x)
            {
                tmpSo = oc1->getData()[Ids::identifier];
                tmpEo = oc2->getData()[Ids::identifier];
            }
            else
            {
                tmpSo = oc2->getData()[Ids::identifier];
                tmpEo = oc1->getData()[Ids::identifier];
            }
        }
        objController.addNewLinkIfPossible(this,
                                           ObjectFactory::createNewLinkObjectTree(Ids::waveguide,
                                                                                  objController.getNewNameForObject(Ids::waveguide),
                                                                                  tmpSo, tmpEo));
	}
    else if (r == 6)
    {
        DBG("Add audio connection");
        objController.addNewAudioConnection(this);
    }
}

void ObjectsHolder::showAudioConnectionPopupMenu()
{
    PopupMenu m;
    m.addSectionHeader("Add...");
	m.addItem (1, "Audio Connection");
    const int r = m.show();
    if( r == 1)
    {
        objController.addNewAudioConnection(this);
    }
}

void ObjectsHolder::findLassoItemsInArea (Array <SelectableObject*>& results, const Rectangle<int>& lasso)
{
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        SelectableObject* const e = dynamic_cast <SelectableObject*> (getChildComponent (i));
        bool isIntersecting;
        if(LinkComponent* const lc = dynamic_cast<LinkComponent*>(e))
            isIntersecting = lasso.contains(lc->getIntersectioBounds());
        else if(AudioOutConnector* const aoc = dynamic_cast<AudioOutConnector*>(e))
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

const Rectangle<int> ObjectsHolder::getObjectsExtent() const
{
//    int maxX = 0;
//    int maxY = 0;
//    for (int i = 0; i < getNumChildComponents(); ++i)
//    {
//        Component* const comp = getChildComponent(i);
//        Rectangle<int> rect = comp->getBounds();
//        int x = rect.getRight();
//        int y = rect.getBottom();
//        if(x > maxX)
//            maxX = x;
//        if(y > maxY)
//            maxY = y;
//    }
    return Rectangle<int>(0,0, maxX, maxY);
}

const Rectangle<int> ObjectsHolder::getObjectsBounds() const
{
    int rightX = 0;
    int downY = 0;
    int leftX = getWidth();
    int upY = getHeight();
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        Component* const comp = getChildComponent(i);
        ObjectComponent* const oc = dynamic_cast<ObjectComponent*>(comp);
        CommentComponent* const cc = dynamic_cast<CommentComponent*>(comp);
        if(oc != nullptr || cc != nullptr)
        {
            Rectangle<int> rect = comp->getBounds();
            int x1 = rect.getX();
            int y1 = rect.getY();
            int x2 = rect.getRight();
            int y2 = rect.getBottom();
            if (x1 < leftX)
                leftX = x1;
            if (y1 < upY)
                upY = y1;
            if (x2 > rightX)
                rightX = x2;
            if (y2 > downY)
                downY = y2;
        }
    }
    return Rectangle<int>(leftX, upY, rightX - leftX, downY - upY);
}

void ObjectsHolder::checkExtent(const Rectangle<int>& r)
{
    if(r.getRight() > maxX)
        maxX = r.getRight();
    if(r.getBottom() > maxY)
        maxY = r.getBottom();

}

//==============================================================================
bool ObjectsHolder::isSnapActive (const bool disableIfCtrlKeyDown) const throw()
{
    return snapActive != (disableIfCtrlKeyDown && ModifierKeys::getCurrentModifiers().isCtrlDown());
}

int ObjectsHolder::snapPosition (int pos) const throw()
{
    if (isSnapActive (true))
    {
        jassert (snapGridPixels > 0);
        pos = ((pos + snapGridPixels * 1024 + snapGridPixels / 2) / snapGridPixels - 1024) * snapGridPixels;
    }

    return pos;
}

void ObjectsHolder::setSnappingGrid (const int numPixels, const bool active, const bool shown)
{
    if (numPixels != snapGridPixels
         || active != snapActive
         || shown != snapShown)
    {
        snapGridPixels = numPixels;
        snapActive = active;
        snapShown = shown;
        StoredSettings::getInstance()->setSnapGridPixels(snapGridPixels);
        StoredSettings::getInstance()->setSnapGridEnabled(snapActive);
        StoredSettings::getInstance()->setSnapGridShow(snapShown);
        if(grid->updateFromDesign(*this))
            repaint();

    }
}

double timeStep = 0.6;
void ObjectsHolder::timerCallback()
{
//    DBG("tick");
    if (graph != nullptr)
    {
//        int64 currentTime = Time::getCurrentTime().currentTimeMillis();
//        float dT = (currentTime-lastTime)/1000.0f;


        bool done = graph->reflow(getContentComp()->getViewPosition().x,
                                  getContentComp()->getViewPosition().y,
                                  getContentComp()->getViewWidth(),
                                  getContentComp()->getViewHeight(),
                                  objController, timeStep);
        updateComponents();
        repaint();
        if(done)
        {
            graph = nullptr;
            stopTimer();
            DBG("stop timer");
        }
    }
}

ContentComp* ObjectsHolder::getContentComp()
{
    return findParentComponentOfClass<ContentComp>();
}

void ObjectsHolder::redrawObjects(const int cmdId)
{
    grabKeyboardFocus();
    DBG("redraw objects");
    if (isTimerRunning())
    {
        stopTimer();
        graph = nullptr;
    }

    timeStep = StoredSettings::getInstance()->getProps().getDoubleValue("redrawparam_timestep", 0.6);
    if(cmdId == CommandIDs::redrawCircle)
    {
        graph = new DirectedGraph();
        objController.makeGraph(graph.get());
    }
    else if(cmdId == CommandIDs::redrawForceDirected)
    {
        graph = new DirectedGraph();
        objController.makeGraph(graph.get());
//        DBG(graph->toString());
//        graph->setFlowAlgorithm(new ForceDirectedFlowAlgorithm());
        graph->setFlowAlgorithm(new ForceBasedFlowAlgorithm());
        graph->randomizeNodes(getContentComp()->getViewPosition().x,
                              getContentComp()->getViewPosition().y,
                              getContentComp()->getViewWidth(),
                              getContentComp()->getViewHeight());

    }

    lastTime = Time::getCurrentTime().currentTimeMillis();
    startTimer(10);

}
//==============================================================================