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
#include "../Utilities/ObjectsHelper.h"
#include "../Utilities/ObjectsClipboard.h"

#include "ObjectsHolder.h"

using namespace synthamodeler;

int ObjectsHolder::objectsHolderNum = 0;
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
    grabKeyboardFocus();

	commandManager->registerAllCommandsForTarget(this);

	addKeyListener(commandManager->getKeyMappings());

    ++objectsHolderNum;
    setComponentID("ObjectsHolder_" + String(objectsHolderNum));
}

ObjectsHolder::~ObjectsHolder()
{
    grid = nullptr;
    fcPanel = nullptr;
    --objectsHolderNum;
    masterReference.clear();
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
        if(getStartEndObjects(startObj, endObj))
        {
            DBG(String("Link: ") + startObj + String(", ") + endObj);
            showLinkPopupMenu(startObj, endObj);
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

void ObjectsHolder::openFaustcodePanel()
{
    if (fcPanel == nullptr && mdlFile)
    {
        fcPanel = new FaustcodePanel(&objController, mdlFile->getMDLRoot(),
                                     &mdlFile->getUndoMgr());
    }
    fcPanel->show();
}

void ObjectsHolder::setSegmentedLinks()
{
    StoredSettings::getInstance()->setIsSegmentedConnectors(!StoredSettings::getInstance()->getIsSegmentedConnectors());
    objController.setLinksSegmented(StoredSettings::getInstance()->getIsSegmentedConnectors());
    updateComponents();
    repaint();
}

void ObjectsHolder::insertNewObject(const Identifier& objType)
{
    Point<int> mp = getMouseXYRelativeViewport();
    insertNewObject(objType, mp);
}

void ObjectsHolder::insertNewObject(const Identifier& objType,
                                    const Point<int>& point)
{
    String objName = objController.getNewNameForObject(objType);
    ValueTree objTree = ObjectFactory::createNewObjectTree(objType,
                                                           objName,
                                                           point.x,
                                                           point.y);
    if(objType != Ids::comment)
    {
        objController.addNewObject(this, objTree);
    }
    else
    {
        objController.addNewComment(this, objTree);
    }
}

void ObjectsHolder::insertNewLink(const Identifier& linkType)
{
    String startObj, endObj;
    if(! getStartEndObjects(startObj, endObj))
        return;

    // For waveguides also find out which is the right and left object
    if(linkType == Ids::waveguide)
        if(! getStartEndObjectsLeftRight(startObj, endObj))
            return;

    insertNewLink(linkType, startObj, endObj);
}

void ObjectsHolder::insertNewLink(const Identifier& linkType,
                                  const String& startId,
                                  const String& endId)
{
    String linkName = objController.getNewNameForObject(linkType);
    ValueTree linkTree = ObjectFactory::createNewLinkObjectTree(linkType,
                                                                linkName,
                                                                startId,
                                                                endId);
    objController.addNewLinkIfPossible(this, linkTree);
}

Point<int> ObjectsHolder::getMouseXYRelativeViewport()
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

    return mp;
}

bool ObjectsHolder::getStartEndObjects(String& startId, String& endId)
{
    SelectedItemSet<SelectableObject*>& sis = objController.getSelectedObjects();
    if(sis.getNumSelected() == 2)
    {
        ObjectComponent* oc1 = dynamic_cast<ObjectComponent*> (sis.getSelectedItem(0));
        ObjectComponent* oc2 = dynamic_cast<ObjectComponent*> (sis.getSelectedItem(1));
        if (oc1 != nullptr && oc2 != nullptr)
        {
            startId = oc1->getData().getProperty(Ids::identifier).toString();
            endId = oc2->getData().getProperty(Ids::identifier).toString();
            return true;
        }
    }
    return false;
}

bool ObjectsHolder::getStartEndObjectsLeftRight(String& startId, String& endId)
{
    ObjectComponent* oc1 = objController.getObjectForId(startId);
    ObjectComponent* oc2 = objController.getObjectForId(endId);
    if (oc1 != nullptr && oc2 != nullptr)
    {
        if (oc1->getPinPos().x < oc2->getPinPos().x)
        {
            startId = oc1->getData()[Ids::identifier];
            endId = oc2->getData()[Ids::identifier];
        }
        else
        {
            endId = oc1->getData()[Ids::identifier];
            startId = oc2->getData()[Ids::identifier];
        }
        return true;
    }
    return false;
}

void ObjectsHolder::showObjectIds()
{
    if (!isDrawingObjectNames)
    {
        showObjectNames = !showObjectNames;
    }
    repaint();
}

void ObjectsHolder::showAudioConnections()
{
    bool sac = StoredSettings::getInstance()->getShowAudioConnections();
    sac = !sac;
    StoredSettings::getInstance()->setShowAudioConnections(sac);
    objController.setAudioConnectionVisibility(sac);
}

//==========================================================================
ApplicationCommandTarget* ObjectsHolder::getNextCommandTarget()
{
    return findFirstTargetParentComponent();
}
void ObjectsHolder::getAllCommands(Array<CommandID>& commands)
{
    const CommandID ids[] = {
        StandardApplicationCommandIDs::cut,
        StandardApplicationCommandIDs::copy,
        StandardApplicationCommandIDs::paste,
        StandardApplicationCommandIDs::del,
        StandardApplicationCommandIDs::selectAll,
        StandardApplicationCommandIDs::deselectAll,
        CommandIDs::segmentedConnectors,
        CommandIDs::reverseDirection,
        CommandIDs::defineFaustcode,
        CommandIDs::tidyObjects,
        CommandIDs::redrawCircle,
        CommandIDs::redrawForceDirected,
        CommandIDs::showRedrawOptions,
        CommandIDs::insertMass,
        CommandIDs::insertGround,
        CommandIDs::insertResonator,
        CommandIDs::insertPort,
        CommandIDs::insertLink,
        CommandIDs::insertTouch,
        CommandIDs::insertPulsetouch,
        CommandIDs::insertPluck,
        CommandIDs::insertAudioOutput,
        CommandIDs::insertAudioConnection,
        CommandIDs::insertWaveguide,
        CommandIDs::insertTermination,
        CommandIDs::insertJunction,
        CommandIDs::insertComment,
        CommandIDs::moveUp,
        CommandIDs::moveDown,
        CommandIDs::moveLeft,
        CommandIDs::moveRight,
//        CommandIDs::moveUpFine,
//        CommandIDs::moveDownFine,
//        CommandIDs::moveLeftFine,
//        CommandIDs::moveRightFine,
        CommandIDs::showObjectNames,
        CommandIDs::enableSnapToGrid,
        CommandIDs::showGrid,
        CommandIDs::showAudioConnections,
    };

    commands.addArray(ids, numElementsInArray(ids));
}
void ObjectsHolder::getCommandInfo(CommandID commandID,
                                   ApplicationCommandInfo& result)
{
    switch (commandID)
    {
    case StandardApplicationCommandIDs::cut:
        result.setInfo(TRANS("Cut"), "", CommandCategories::editing, 0);
        result.addDefaultKeypress('x', ModifierKeys::commandModifier);
        break;
    case StandardApplicationCommandIDs::copy:
        result.setInfo(TRANS("Copy"), "", CommandCategories::editing, 0);
        result.addDefaultKeypress('c', ModifierKeys::commandModifier);
        break;
    case StandardApplicationCommandIDs::paste:
        result.setInfo(TRANS("Paste"), "", CommandCategories::editing, 0);
        result.addDefaultKeypress('v', ModifierKeys::commandModifier);
        break;
    case StandardApplicationCommandIDs::selectAll:
        result.setInfo(TRANS("Select All"), "", CommandCategories::editing, 0);
        result.addDefaultKeypress('a', ModifierKeys::commandModifier);
        break;
    case StandardApplicationCommandIDs::deselectAll:
        result.setInfo(TRANS("Deselect All"), "", CommandCategories::editing, 0);
        result.addDefaultKeypress('a', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
        break;
    case StandardApplicationCommandIDs::del:
        result.setInfo(TRANS("Delete"), "", CommandCategories::editing, 0);
        result.addDefaultKeypress(KeyPress::deleteKey, 0);
        result.addDefaultKeypress(KeyPress::backspaceKey, 0);
        break;
    case CommandIDs::segmentedConnectors:
        result.setInfo(TRANS("Segmented Connectors"), "",
                       CommandCategories::editing, 0);
        result.setTicked(StoredSettings::getInstance()->getIsSegmentedConnectors());
        result.addDefaultKeypress('t', ModifierKeys::commandModifier);
        break;
    case CommandIDs::reverseDirection:
        result.setInfo(TRANS("Reverse Direction"), "",
                       CommandCategories::editing, 0);
        result.addDefaultKeypress('r', ModifierKeys::commandModifier);
        break;
    case CommandIDs::defineFaustcode:
        result.setInfo(TRANS("Define FAUST Code"), "",
                       CommandCategories::editing, 0);
        result.addDefaultKeypress('d', ModifierKeys::commandModifier);
        break;
    case CommandIDs::tidyObjects:
        result.setInfo(TRANS("Tidy Up"), "", CommandCategories::editing, 0);
        result.addDefaultKeypress('t', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
        break;
    case CommandIDs::redrawCircle:
        result.setInfo(TRANS("Circle"), "", CommandCategories::editing, 0);
        result.setActive(mdlFile != nullptr ? (!mdlFile->isEmpty()) : false);
        break;
    case CommandIDs::redrawForceDirected:
        result.setInfo(TRANS("Force-Directed"), "",
                       CommandCategories::editing, 0);
        result.setActive(mdlFile != nullptr ? (!mdlFile->isEmpty()) : false);
        result.addDefaultKeypress('r', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
        break;
    case CommandIDs::showRedrawOptions:
        result.setInfo(TRANS("Force-Directed options") + "...",
                       TRANS("Open redraw options"), CommandCategories::editing, 0);
        result.addDefaultKeypress('d', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
        break;
    case CommandIDs::insertMass:
        result.setInfo(TRANS("Mass"), "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('1', ModifierKeys::commandModifier);
        break;
    case CommandIDs::insertGround:
        result.setInfo(TRANS("Ground"), "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('2', ModifierKeys::commandModifier);
        break;
    case CommandIDs::insertResonator:
        result.setInfo(TRANS("Resonator"), "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('3', ModifierKeys::commandModifier);
        break;
    case CommandIDs::insertPort:
        result.setInfo(TRANS("Port"), "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('4', ModifierKeys::commandModifier);
        break;

    case CommandIDs::insertLink:
        result.setInfo(TRANS("Linear Link"), "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('5', ModifierKeys::commandModifier);
        break;
    case CommandIDs::insertTouch:
        result.setInfo(TRANS("Touch Link"), "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('6', ModifierKeys::commandModifier);
        break;
    case CommandIDs::insertPulsetouch:
        result.setInfo(TRANS("Pulsetouch Link"), "",
                       CommandCategories::inserting, 0);
//        result.addDefaultKeypress('6', ModifierKeys::commandModifier);
        break;
    case CommandIDs::insertPluck:
        result.setInfo(TRANS("Pluck Link"), "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('7', ModifierKeys::commandModifier);
        break;

    case CommandIDs::insertAudioOutput:
        result.setInfo(TRANS("Audio Output"), "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('8', ModifierKeys::commandModifier);
        break;
    case CommandIDs::insertAudioConnection:
        result.setInfo(TRANS("Audio Connection"), "",
                       CommandCategories::inserting, 0);
        result.addDefaultKeypress('8', ModifierKeys::commandModifier | ModifierKeys::altModifier);
        break;
    case CommandIDs::insertWaveguide:
        result.setInfo(TRANS("Waveguide"), "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('9', ModifierKeys::commandModifier);
        break;
    case CommandIDs::insertTermination:
        result.setInfo(TRANS("Termination"), "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('0', ModifierKeys::commandModifier);
        break;
    case CommandIDs::insertJunction:
        result.setInfo(TRANS("Junction"), "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('0', ModifierKeys::commandModifier | ModifierKeys::altModifier);
        break;
    case CommandIDs::insertComment:
    {
        result.setInfo(TRANS("Comment"), "", CommandCategories::inserting, 0);
//        MDLFile * mdlF = mainWindow.getMDLFile();
//        result.setActive(StoredSettings::getInstance()->getIsUsingMDLX()
//                         || (mdlF != nullptr ? mdlF->getFile().hasFileExtension(".mdlx") : false));
    }
        break;
    case CommandIDs::moveUp:
        result.setInfo(TRANS("Move Object Up"), "",
                       CommandCategories::generation, 0);
        result.addDefaultKeypress(KeyPress::upKey, 0);
        break;
//    case CommandIDs::moveUpFine:
//        result.setInfo("Move Object Up Fine", "", CommandCategories::generation, 0);
//        result.addDefaultKeypress(KeyPress::upKey, ModifierKeys::shiftModifier);
//        break;
    case CommandIDs::moveDown:
        result.setInfo(TRANS("Move Object Down"), "",
                       CommandCategories::generation, 0);
        result.addDefaultKeypress(KeyPress::downKey, 0);
        break;
//    case CommandIDs::moveDownFine:
//        result.setInfo("Move Object Down Fine", "", CommandCategories::generation, 0);
//        result.addDefaultKeypress(KeyPress::downKey, ModifierKeys::shiftModifier);
//        break;
    case CommandIDs::moveLeft:
        result.setInfo(TRANS("Move Object Left"), "",
                       CommandCategories::generation, 0);
        result.addDefaultKeypress(KeyPress::leftKey, 0);
        break;
//    case CommandIDs::moveLeftFine:
//        result.setInfo("Move Object Left Fine", "", CommandCategories::generation, 0);
//        result.addDefaultKeypress(KeyPress::leftKey, ModifierKeys::shiftModifier);
//        break;
    case CommandIDs::moveRight:
        result.setInfo(TRANS("Move Object Right"), "",
                       CommandCategories::generation, 0);
        result.addDefaultKeypress(KeyPress::rightKey, 0);
        break;
//    case CommandIDs::moveRightFine:
//        result.setInfo("Move Object Right Fine", "", CommandCategories::generation, 0);
//        result.addDefaultKeypress(KeyPress::rightKey, ModifierKeys::shiftModifier);
//        break;
    case CommandIDs::showObjectNames:
        result.setInfo(TRANS("Show Identifiers"), "",
                       CommandCategories::editing, 0);
        result.addDefaultKeypress('i', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
        result.setTicked(getShowObjectNames());
        break;

    case CommandIDs::enableSnapToGrid:
        result.setInfo(TRANS("Enable Snap-to-grid"),
                       TRANS("Toggles whether components' positions are aligned to a grid."),
                       CommandCategories::view, 0);
        result.setTicked(isSnapActive(false));
//        result.defaultKeypresses.add(KeyPress('g', cmd, 0));
        break;

    case CommandIDs::showGrid:
        result.setInfo(TRANS("Show Grid"),
                       TRANS("Toggles whether the snapping grid is displayed on-screen."),
                       CommandCategories::view, 0);
        result.setTicked(isSnapShown());
//        result.defaultKeypresses.add(KeyPress('g', cmd | shift, 0));
        break;

    case CommandIDs::showAudioConnections:
        result.setInfo(TRANS("Show Audio Connections"), "",
                       CommandCategories::view, 0);
        result.setTicked(StoredSettings::getInstance()->getShowAudioConnections());
        break;
    default:
        break;
    };

}

int64 lastTime = 0.0f;
bool ObjectsHolder::perform(const InvocationInfo& info)
{
    switch (info.commandID)
    {
    case StandardApplicationCommandIDs::cut:
        ObjectsClipboard::cut(objController, *this);
        break;
    case StandardApplicationCommandIDs::copy:
        ObjectsClipboard::copySelected(objController);
        break;
    case StandardApplicationCommandIDs::paste:
        ObjectsClipboard::paste(objController, *this);
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
        openFaustcodePanel();
        break;
    case CommandIDs::segmentedConnectors:
        setSegmentedLinks();
        break;
    case CommandIDs::reverseDirection:
        objController.reverseLinkDirection();
        break;
    case CommandIDs::tidyObjects:
        ObjectsHelper::tidyUpObjects(objController);
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
        insertNewObject(Ids::mass);
        break;
    case CommandIDs::insertGround:
        insertNewObject(Ids::ground);
        break;
    case CommandIDs::insertResonator:
        insertNewObject(Ids::resonators);
        break;
    case CommandIDs::insertPort:
        insertNewObject(Ids::port);
        break;

    case CommandIDs::insertLink:
        insertNewLink(Ids::link);
        break;
    case CommandIDs::insertTouch:
        insertNewLink(Ids::touch);
    case CommandIDs::insertPulsetouch:
        insertNewLink(Ids::pulsetouch);
        break;
    case CommandIDs::insertPluck:
        insertNewLink(Ids::pluck);
        break;
    case CommandIDs::insertWaveguide:
        insertNewLink(Ids::waveguide);
        break;

    case CommandIDs::insertAudioOutput:
        insertNewObject(Ids::audioout);
        break;
    case CommandIDs::insertAudioConnection:
        objController.addNewAudioConnection(this);
        break;
    case CommandIDs::insertJunction:
        insertNewObject(Ids::junction);
        break;
    case CommandIDs::insertTermination:
        insertNewObject(Ids::termination);
        break;
    case CommandIDs::insertComment:
        insertNewObject(Ids::comment);
        break;
    case CommandIDs::moveUp:
        objController.moveSelectedComps(0, -StoredSettings::getInstance()->getSnapGridPixels());
        break;
//    case CommandIDs::moveUpFine:
//        objController.moveSelectedComps(0, -StoredSettings::getInstance()->getSnapGridPixels()*2);
//        break;
    case CommandIDs::moveDown:
        objController.moveSelectedComps(0, StoredSettings::getInstance()->getSnapGridPixels());
        break;
//    case CommandIDs::moveDownFine:
//        objController.moveSelectedComps(0, StoredSettings::getInstance()->getSnapGridPixels()*2);
//        break;
    case CommandIDs::moveLeft:
        objController.moveSelectedComps(-StoredSettings::getInstance()->getSnapGridPixels(), 0);
        break;
//    case CommandIDs::moveLeftFine:
//        objController.moveSelectedComps(-StoredSettings::getInstance()->getSnapGridPixels()*2, 0);
//        break;
    case CommandIDs::moveRight:
        objController.moveSelectedComps(StoredSettings::getInstance()->getSnapGridPixels(), 0);
        break;
//    case CommandIDs::moveRightFine:
//        objController.moveSelectedComps(StoredSettings::getInstance()->getSnapGridPixels()*2, 0);
//        break;
    case CommandIDs::showObjectNames:
        showObjectIds();
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
        showAudioConnections();
        break;

    default:
        return false;
    }
    objController.getUndoManager().beginNewTransaction();
    return true;
}

void ObjectsHolder::showContextMenu(const Point<int> mPos)
{
    PopupMenu m;
    m.addSectionHeader(TRANS("Insert") + "...");
    m.addItem(1, TRANS("Mass"));
    m.addItem(2, TRANS("Ground"));
    m.addItem(3, TRANS("Resonator"));
    m.addItem(4, TRANS("Port"));
    m.addSeparator();
    m.addItem(5, TRANS("Audio Out"));
    m.addSeparator();
    m.addItem(6, TRANS("Junction"));
    m.addItem(7, TRANS("Termination"));
    m.addSeparator();
//    bool commentEnabled = StoredSettings::getInstance()->getIsUsingMDLX();
//    bool isUsingMDLX = mdlFile != nullptr ? mdlFile->getFile().hasFileExtension(".mdlx") : false;
//    m.addItem(8, "Comment", (commentEnabled || isUsingMDLX));
    m.addItem(8, TRANS("Comment"));

    const int r = m.show();

    if (r == 1)
    {
        insertNewObject(Ids::mass, mPos);
    }
    else if (r == 2)
    {
        insertNewObject(Ids::ground, mPos);
    }
    else if (r == 3)
    {
        insertNewObject(Ids::resonators, mPos);
    }
    else if (r == 4)
    {
        insertNewObject(Ids::port, mPos);
    }
    else if (r == 5)
    {
        insertNewObject(Ids::audioout, mPos);
    }
    else if (r == 6)
    {
        insertNewObject(Ids::junction, mPos);
    }
    else if (r == 7)
    {
        insertNewObject(Ids::termination, mPos);
    }
    else if (r == 8)
    {
        insertNewObject(Ids::comment, mPos);
    }
}

void ObjectsHolder::showLinkPopupMenu(String so, String eo)
{
	PopupMenu m;
    m.addSectionHeader(TRANS("Add") + "...");
    m.addItem(1, TRANS("Linear Link"));
    m.addItem(2, TRANS("Touch Link"));
    m.addItem(3, TRANS("Pulsetouch Link"));
    m.addItem(4, TRANS("Pluck Link"));
    m.addSeparator();
    m.addItem(5, TRANS("Waveguide"));
    m.addSeparator();
    m.addItem(6, TRANS("Audio Connection"));
	const int r = m.show();

	if (r == 1)
	{
		DBG("Add link");
        insertNewLink(Ids::link, so, eo);
	}
	else if (r == 2)
	{
		DBG("Add touch");
        insertNewLink(Ids::touch, so, eo);
	}
	else if (r == 3)
	{
		DBG("Add pulsetouch");
        insertNewLink(Ids::pulsetouch, so, eo);
	}
	else if (r == 4)
	{
		DBG("Add pluck");
        insertNewLink(Ids::pluck, so, eo);
	}
	else if (r == 5)
	{
		DBG("Add waveguide");
        getStartEndObjectsLeftRight(so, eo);
        insertNewLink(Ids::waveguide, so, eo);
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
    m.addSectionHeader(TRANS("Add") + "...");
    m.addItem(1, TRANS("Audio Connection"));
    const int r = m.show();
    if( r == 1)
    {
        objController.addNewAudioConnection(this);
    }
}

void ObjectsHolder::findLassoItemsInArea (Array <SelectableObject*>& results,
                                          const Rectangle<int>& lasso)
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
        pos = ((pos + snapGridPixels * 1024 + snapGridPixels / 2)
            / snapGridPixels - 1024) * snapGridPixels;
    }

    return pos;
}

void ObjectsHolder::setSnappingGrid (const int numPixels,
                                     const bool active,
                                     const bool shown)
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

    timeStep = StoredSettings::getInstance()->getProps()
        .getDoubleValue("redrawparam_timestep", 0.6);

    if(cmdId == CommandIDs::redrawCircle)
    {
        graph = new DirectedGraph();
        ObjectsHelper::makeGraph(objController, *graph.get());
    }
    else if(cmdId == CommandIDs::redrawForceDirected)
    {
        graph = new DirectedGraph();
        ObjectsHelper::makeGraph(objController, *graph.get());
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