/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  12 May 2012 12:35:30am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
 */

//[Headers] You can add your own extra header files here...
#include "../Application/CommonHeaders.h"
#include "../Application/MainWindow.h"
#include "../Controller/ObjController.h"
#include "ObjectsHolder.h"
#include "../Models/MDLFile.h"
//[/Headers]

#include "ContentComp.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

class ContentComp::MagnifierComponent : public Component
{
public:

    MagnifierComponent(Component* content_)
    : scaleFactor(1.0), content(content_)
    {
        addAndMakeVisible(content);
        childBoundsChanged(content);
    }

    void childBoundsChanged(Component* child)
    {
        const Rectangle<int> childArea(getLocalArea(child, child->getLocalBounds()));
        setSize(childArea.getWidth(), childArea.getHeight());
    }

    double getScaleFactor() const
    {
        return scaleFactor;
    }

    void setScaleFactor(double newScale)
    {
        scaleFactor = newScale;
        content->setTransform(AffineTransform::scale((float) scaleFactor,
                                                     (float) scaleFactor));
    }

private:
    double scaleFactor;
    ScopedPointer<Component> content;
};

//==============================================================================

class ZoomingViewport : public Viewport
{
public:

    ZoomingViewport(ContentComp * const panel_)
    : panel(panel_),
    isSpaceDown(false)
    {
    }

    ~ZoomingViewport()
    {
    }

    void mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& wheel)
    {
        if (e.mods.isCtrlDown() || e.mods.isAltDown())
        {
            const double factor = (wheel.deltaY > 0) ? 2.0 : 0.5;

            panel->setZoom(panel->getZoom() * factor, wheel.deltaX, wheel.deltaY);
        }
        else
        {
            Viewport::mouseWheelMove(e, wheel);
        }
    }

    void dragKeyHeldDown(const bool isKeyDown)
    {
        if (isSpaceDown != isKeyDown)
        {
            isSpaceDown = isKeyDown;

            if (isSpaceDown)
            {
                DraggerOverlayComp * const dc = new DraggerOverlayComp();
                addAndMakeVisible(dc);
                dc->setBounds(0, 0, getWidth(), getHeight());
            }
            else
            {
                for (int i = getNumChildComponents(); --i >= 0;)
                {
                    if (dynamic_cast<DraggerOverlayComp*> (getChildComponent(i)) != 0)
                    {
                        delete getChildComponent(i);
                    }
                }
            }
        }
    }

private:
    ContentComp * const panel;
    bool isSpaceDown;

    //==============================================================================

    class DraggerOverlayComp : public Component
    {
    public:

        DraggerOverlayComp() : startX(0), startY(0)
        {
            setMouseCursor(MouseCursor::DraggingHandCursor);
            setAlwaysOnTop(true);
        }

        ~DraggerOverlayComp()
        {
        }

        void mouseDown(const MouseEvent& e)
        {
            Viewport* viewport = findParentComponentOfClass<Viewport > ();

            if (viewport != 0)
            {
                startX = viewport->getViewPositionX();
                startY = viewport->getViewPositionY();
            }
        }

        void mouseDrag(const MouseEvent& e)
        {
            Viewport* viewport = findParentComponentOfClass<Viewport > ();

            if (viewport != 0)
            {
                viewport->setViewPosition(jlimit(0,
                                                 jmax(0, viewport->getViewedComponent()->getWidth()
                                                      - viewport->getViewWidth()),
                                                 startX - e.getDistanceFromDragStartX()),
                                          jlimit(0,
                                                 jmax(0, viewport->getViewedComponent()->getHeight()
                                                      - viewport->getViewHeight()),
                                                 startY - e.getDistanceFromDragStartY()));
                //            	viewport->setViewPosition(e.getMouseDownPosition());
            }
        }

    private:
        int startX, startY;
    };
};

//[/MiscUserDefs]

//==============================================================================

ContentComp::ContentComp(MainAppWindow& mainWindow_, ObjController& objController_)
: mainWindow(mainWindow_),
objController(objController_),
objectsHolder(0)
{

    //[UserPreSize]
    //[/UserPreSize]

    //    setSize (600, 600);
    addAndMakeVisible(viewport = new ZoomingViewport(this));

    //[Constructor] You can add your own custom stuff here..
    objectsHolder = new ObjectsHolder(objController);
    //    addAndMakeVisible (objectsHolder);
    viewport->setViewedComponent(magnifier = new MagnifierComponent(objectsHolder));

    //    objectsHolder->setVisible(true);
    setWantsKeyboardFocus(true);
    //[/Constructor]
}

ContentComp::~ContentComp()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
//    objectsHolder = nullptr;
    deleteAllChildren();
    //[/Destructor]
}

//==============================================================================

void ContentComp::paint(Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll(Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ContentComp::resized()
{
    //[UserResized] Add your own custom resize handling here..
    //    propsPanel->setBounds (contentW + 4, 4, jmax (100, getWidth() - contentW - 8), getHeight() - 8);

    viewport->setBounds(4, 4, getWidth(), getHeight() - 8);

    if (false)
    objectsHolder->setSize(jmax(mainWindow.getWidth() - 8,
                                roundToInt((viewport->getWidth() - viewport->getScrollBarThickness()) / getZoom())),
                           jmax(mainWindow.getHeight() - LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight() - 8,
                                roundToInt((viewport->getHeight() - viewport->getScrollBarThickness()) / getZoom())));
    else
    {
        Rectangle<int> rect = objectsHolder->getObjectsExtent();
        objectsHolder->setSize(jmax(roundToInt((viewport->getWidth() - viewport->getScrollBarThickness()) / getZoom()), rect.getRight()),
                               jmax(roundToInt((viewport->getHeight() - viewport->getScrollBarThickness()) / getZoom()), rect.getBottom()));
    }
    //        DBG(viewport->getBounds().toString());
//    DBG(String(viewport->getViewWidth()) + ", " + String(viewport->getViewHeight()));
    //    objectsHolder->setSize (mainWindow.getWidth(), mainWindow.getHeight());

    //	if(objectsHolder != nullptr)
    //		objectsHolder->setBounds(0, 0, getWidth(), getHeight());
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void ContentComp::setMDLFile(MDLFile* newMDLFile)
{
    objectsHolder->setMDLFile(newMDLFile);
}

void ContentComp::updateMainAppWindowTitle(const String& newTitle)
{
    MainAppWindow* mw = findParentComponentOfClass<MainAppWindow > ();

    if (mw != nullptr)
        mw->updateTitle();

}

ApplicationCommandTarget* ContentComp::getNextCommandTarget()
{
    // this will return the next parent			 component that is an ApplicationCommandTarget (in this
    // case, there probably isn't one, but it's best to use this method in your own apps).
    return findFirstTargetParentComponent();
}

void ContentComp::getAllCommands(Array <CommandID>& commands)
{
    // this returns the set of all commands that this target can perform..
    const CommandID ids[] = {CommandIDs::undo,
        CommandIDs::redo,
        StandardApplicationCommandIDs::cut,
        StandardApplicationCommandIDs::copy,
        StandardApplicationCommandIDs::paste,
        StandardApplicationCommandIDs::del,
        StandardApplicationCommandIDs::selectAll,
        StandardApplicationCommandIDs::deselectAll,
        CommandIDs::segmentedConnectors,
        CommandIDs::zoomIn,
        CommandIDs::zoomOut,
        CommandIDs::zoomNormal,
        CommandIDs::reverseDirection,
        CommandIDs::defineVariables,
        CommandIDs::tidyObjects,
        CommandIDs::insertMass,
        CommandIDs::insertGround,
        CommandIDs::insertResonator,
        CommandIDs::insertPort,
        CommandIDs::insertLink,
        CommandIDs::insertTouch,
        CommandIDs::insertPluck,
        CommandIDs::insertAudioOutput,
        CommandIDs::insertAudioConnection,
        CommandIDs::insertWaveguide,
        CommandIDs::insertTermination,
        CommandIDs::insertJunction,
        CommandIDs::spaceBarDrag,
        CommandIDs::moveUp,
        CommandIDs::moveDown,
        CommandIDs::moveLeft,
        CommandIDs::moveRight,
        CommandIDs::moveUpFine,
        CommandIDs::moveDownFine,
        CommandIDs::moveLeftFine,
        CommandIDs::moveRightFine,
        CommandIDs::showObjectNames,
        CommandIDs::enableSnapToGrid,
        CommandIDs::showGrid,
    };

    commands.addArray(ids, numElementsInArray(ids));
}

// This method is used when something needs to find out the details about one of the commands
// that this object can perform..

void ContentComp::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
    switch (commandID)
    {
    case CommandIDs::undo:
        result.setInfo("Undo", "Undo last edit", CommandCategories::editing, 0);
        result.addDefaultKeypress('z', ModifierKeys::commandModifier);
        break;
    case CommandIDs::redo:
        result.setInfo("Redo", "Undo last undo", CommandCategories::editing, 0);
        result.addDefaultKeypress('z', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
        break;

    case StandardApplicationCommandIDs::cut:
        result.setInfo("Cut", "", CommandCategories::editing, 0);
        result.addDefaultKeypress('x', ModifierKeys::commandModifier);
        break;
    case StandardApplicationCommandIDs::copy:
        result.setInfo("Copy", "", CommandCategories::editing, 0);
        result.addDefaultKeypress('c', ModifierKeys::commandModifier);
        break;
    case StandardApplicationCommandIDs::paste:
        result.setInfo("Paste", "", CommandCategories::editing, 0);
        result.addDefaultKeypress('v', ModifierKeys::commandModifier);
        break;
    case StandardApplicationCommandIDs::selectAll:
        result.setInfo("SelectAll", "", CommandCategories::editing, 0);
        result.addDefaultKeypress('a', ModifierKeys::commandModifier);
        break;
    case StandardApplicationCommandIDs::deselectAll:
        result.setInfo("DeselectAll", "", CommandCategories::editing, 0);
        result.addDefaultKeypress('a', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
        break;
    case StandardApplicationCommandIDs::del:
        result.setInfo("Delete", "", CommandCategories::editing, 0);
        result.addDefaultKeypress(KeyPress::deleteKey, 0);
        result.addDefaultKeypress(KeyPress::backspaceKey, 0);
        break;
    case CommandIDs::segmentedConnectors:
        result.setInfo("Segmeted connectors", "", CommandCategories::editing, 0);
        result.setTicked(StoredSettings::getInstance()->getIsSegmentedConnectors());
        result.addDefaultKeypress('t', ModifierKeys::commandModifier);
        break;
    case CommandIDs::zoomIn:
        result.setInfo("Zoom In", "", CommandCategories::editing, 0);
        result.addDefaultKeypress('9', ModifierKeys::altModifier);
        break;
    case CommandIDs::zoomOut:
        result.setInfo("Zoom Out", "", CommandCategories::editing, 0);
        result.addDefaultKeypress('8', ModifierKeys::altModifier);
        break;
    case CommandIDs::zoomNormal:
        result.setInfo("Zoom Normal", "", CommandCategories::editing, 0);
        result.addDefaultKeypress('0', ModifierKeys::altModifier);
        break;
    case CommandIDs::reverseDirection:
        result.setInfo("Reverse direction", "", CommandCategories::editing, 0);
        result.addDefaultKeypress('r', ModifierKeys::commandModifier);
        break;
    case CommandIDs::defineVariables:
        result.setInfo("Define variable", "", CommandCategories::editing, 0);
        result.addDefaultKeypress('d', ModifierKeys::commandModifier);
        break;
    case CommandIDs::tidyObjects:
        result.setInfo("Tidy up", "", CommandCategories::editing, 0);
        result.addDefaultKeypress('t', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
        break;

    case CommandIDs::insertMass:
        result.setInfo("Mass", "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('1', ModifierKeys::commandModifier);
        break;
    case CommandIDs::insertGround:
        result.setInfo("Ground", "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('2', ModifierKeys::commandModifier);
        break;
    case CommandIDs::insertResonator:
        result.setInfo("Resonator", "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('3', ModifierKeys::commandModifier);
        break;
    case CommandIDs::insertPort:
        result.setInfo("Port", "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('4', ModifierKeys::commandModifier);
        break;

    case CommandIDs::insertLink:
        result.setInfo("Linear Link", "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('5', ModifierKeys::commandModifier);
        break;
    case CommandIDs::insertTouch:
        result.setInfo("Touch Link", "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('6', ModifierKeys::commandModifier);
        break;
    case CommandIDs::insertPluck:
        result.setInfo("Pluck Link", "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('7', ModifierKeys::commandModifier);
        break;

    case CommandIDs::insertAudioOutput:
        result.setInfo("Audio Output", "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('8', ModifierKeys::commandModifier);
        break;
    case CommandIDs::insertAudioConnection:
        result.setInfo("Audio connection", "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('8', ModifierKeys::commandModifier | ModifierKeys::altModifier);
        break;
    case CommandIDs::insertWaveguide:
        result.setInfo("Waveguide", "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('9', ModifierKeys::commandModifier);
        break;
    case CommandIDs::insertTermination:
        result.setInfo("Termination", "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('0', ModifierKeys::commandModifier);
        break;
    case CommandIDs::insertJunction:
        result.setInfo("Junction", "", CommandCategories::inserting, 0);
        result.addDefaultKeypress('0', ModifierKeys::commandModifier | ModifierKeys::altModifier);
        break;
    case CommandIDs::spaceBarDrag:
        result.setInfo("Scroll while dragging mouse",
                       "When held down, this key lets you scroll around by dragging with the mouse.",
                       CommandCategories::view, ApplicationCommandInfo::wantsKeyUpDownCallbacks);
        //        result.setActive(currentPaintRoutine != 0 || currentLayout != 0);
        result.defaultKeypresses.add(KeyPress(KeyPress::spaceKey, 0, 0));
        break;
    case CommandIDs::moveUp:
        result.setInfo("Move object up", "", CommandCategories::generation, 0);
        result.addDefaultKeypress(KeyPress::upKey, 0);
        break;
    case CommandIDs::moveUpFine:
        result.setInfo("Move object up fine", "", CommandCategories::generation, 0);
        result.addDefaultKeypress(KeyPress::upKey, ModifierKeys::shiftModifier);
        break;
    case CommandIDs::moveDown:
        result.setInfo("Move object down", "", CommandCategories::generation, 0);
        result.addDefaultKeypress(KeyPress::downKey, 0);
        break;
    case CommandIDs::moveDownFine:
        result.setInfo("Move object down fine", "", CommandCategories::generation, 0);
        result.addDefaultKeypress(KeyPress::downKey, ModifierKeys::shiftModifier);
        break;
    case CommandIDs::moveLeft:
        result.setInfo("Move object left", "", CommandCategories::generation, 0);
        result.addDefaultKeypress(KeyPress::leftKey, 0);
        break;
    case CommandIDs::moveLeftFine:
        result.setInfo("Move object left fine", "", CommandCategories::generation, 0);
        result.addDefaultKeypress(KeyPress::leftKey, ModifierKeys::shiftModifier);
        break;
    case CommandIDs::moveRight:
        result.setInfo("Move object right", "", CommandCategories::generation, 0);
        result.addDefaultKeypress(KeyPress::rightKey, 0);
        break;
    case CommandIDs::moveRightFine:
        result.setInfo("Move object right fine", "", CommandCategories::generation, 0);
        result.addDefaultKeypress(KeyPress::rightKey, ModifierKeys::shiftModifier);
        break;
    case CommandIDs::showObjectNames:
        result.setInfo("Show variable names", "", CommandCategories::editing, 0);
        result.addDefaultKeypress('i', ModifierKeys::commandModifier);
        result.setTicked(objectsHolder->getShowObjectNames());
        break;

    case CommandIDs::enableSnapToGrid:
        result.setInfo("Enable snap-to-grid",
                       "Toggles whether components' positions are aligned to a grid.",
                       CommandCategories::view, 0);
        result.setTicked(objectsHolder->isSnapActive(false));
//        result.defaultKeypresses.add(KeyPress('g', cmd, 0));
        break;

    case CommandIDs::showGrid:
        result.setInfo("Show snap-to-grid",
                       "Toggles whether the snapping grid is displayed on-screen.",
                       CommandCategories::view, 0);
        result.setTicked(objectsHolder->isSnapShown());
//        result.defaultKeypresses.add(KeyPress('g', cmd | shift, 0));
        break;

    default:
        break;
    };
}

// this is the ApplicationCommandTarget method that is used to actually perform one of our commands..


bool ContentComp::perform(const InvocationInfo& info)
{
    mainWindow.getUndoManager()->beginNewTransaction();
    switch (info.commandID)
    {
    case CommandIDs::undo:
        mainWindow.getUndoManager()->undo();
        break;
    case CommandIDs::redo:
        mainWindow.getUndoManager()->redo();
        break;

    case CommandIDs::zoomIn:
        setZoom(getZoom() * 2.0);
        break;
    case CommandIDs::zoomOut:
        setZoom(getZoom() / 2.0);
        break;
    case CommandIDs::zoomNormal:
        setZoom(1.0);
        break;
    case CommandIDs::spaceBarDrag:
        dragKeyHeldDown(info.isKeyDown);
        break;

    default:
        return objectsHolder->dispatchMenuItemClick(info);
    }
    mainWindow.getUndoManager()->beginNewTransaction();
    return true;
}


//void ContentComp::visibilityChanged()
//{
//    if (isVisible())
//    {
////        updatePropertiesList();
//
//        if (getParentComponent() != 0)
//        {
//            resized();
//            JucerDocumentHolder* const cdh = dynamic_cast <JucerDocumentHolder*> (getParentComponent()->getParentComponent());
//
//            if (cdh != 0)
//                cdh->setViewportToLastPos (viewport, *this);
//
//            resized();
//        }
//    }
//    else
//    {
//        if (getParentComponent() != 0)
//        {
//            JucerDocumentHolder* const cdh = dynamic_cast <JucerDocumentHolder*> (getParentComponent()->getParentComponent());
//
//            if (cdh != 0)
//                cdh->storeLastViewportPos (viewport, *this);
//        }
//    }
//
//    editor->setVisible (isVisible());
//}

double ContentComp::getZoom() const
{
    return magnifier->getScaleFactor();
}

void ContentComp::setZoom(double newScale)
{
    setZoom(jlimit(1.0 / 16.0, 16.0, newScale),
            viewport->getWidth() / 2,
            viewport->getHeight() / 2);
}

void ContentComp::setZoom(double newScale, int anchorX, int anchorY)
{
    Point<int> anchor(objectsHolder->getLocalPoint(viewport, Point<int> (anchorX, anchorY)));

    magnifier->setScaleFactor(newScale);

    resized();
    anchor = viewport->getLocalPoint(objectsHolder, anchor);

    viewport->setViewPosition(jlimit(0, jmax(0, viewport->getViewedComponent()->getWidth() - viewport->getViewWidth()),
                                     viewport->getViewPositionX() + anchor.getX() - anchorX),
                              jlimit(0, jmax(0, viewport->getViewedComponent()->getHeight() - viewport->getViewHeight()),
                                     viewport->getViewPositionY() + anchor.getY() - anchorY));
}

void ContentComp::xyToTargetXY(int& x, int& y) const
{
    Point<int> pos(objectsHolder->getLocalPoint(this, Point<int> (x, y)));
    x = pos.getX();
    y = pos.getY();
}

void ContentComp::dragKeyHeldDown(bool isKeyDown)
{
    ((ZoomingViewport*) viewport)->dragKeyHeldDown(isKeyDown);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ContentComp" componentName=""
                 parentClasses="public Component, public ApplicationCommandTarget"
                 constructorParams="MainAppWindow&amp; mainWindow_, ObjController&amp; objController_"
                 variableInitialisers="mainWindow(mainWindow_),&#10;objController(objController_),&#10;objectsHolder(0)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff000000"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
 */
#endif
