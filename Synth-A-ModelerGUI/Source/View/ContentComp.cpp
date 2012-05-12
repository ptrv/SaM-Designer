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
//[/MiscUserDefs]

//==============================================================================
ContentComp::ContentComp (MainAppWindow& mainWindow_, ObjController& objController_)
    : mainWindow(mainWindow_),
      objController(objController_),
      objectsHolder(0)
{

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
//	debugWindow = new DebugWindow();
//	debugWindow->setVisible(true);
    addAndMakeVisible (objectsHolder = new ObjectsHolder(objController));
    setWantsKeyboardFocus(true);
    //[/Constructor]
}

ContentComp::~ContentComp()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
	objectsHolder = nullptr;
    //[/Destructor]
}

//==============================================================================
void ContentComp::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ContentComp::resized()
{
    //[UserResized] Add your own custom resize handling here..
//	objComp->setBounds(1, 1, getWidth(), getHeight());
	if(objectsHolder != nullptr)
		objectsHolder->setBounds(0, 0, getWidth(), getHeight());
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void ContentComp::setMDLFile(MDLFile* newMDLFile)
{
	objectsHolder->setMDLFile(newMDLFile);
}

void ContentComp::updateMainAppWindowTitle(const String& newTitle)
{
    MainAppWindow* mw = findParentComponentOfClass<MainAppWindow>();

    if (mw != nullptr)
        mw->updateTitle ();

}

ApplicationCommandTarget* ContentComp::getNextCommandTarget()
{
    // this will return the next parent			 component that is an ApplicationCommandTarget (in this
    // case, there probably isn't one, but it's best to use this method in your own apps).
    return findFirstTargetParentComponent();
}

void ContentComp::getAllCommands (Array <CommandID>& commands)
{
    // this returns the set of all commands that this target can perform..
    const CommandID ids[] = { CommandIDs::undo,
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
                              CommandIDs::insertMass,
                              CommandIDs::insertGround,
                              CommandIDs::insertResonator,
                              CommandIDs::insertPort,
                              CommandIDs::insertLink,
                              CommandIDs::insertTouch,
                              CommandIDs::insertPluck,
                              CommandIDs::insertAudioOutput,
                              CommandIDs::insertWaveguide,
                              CommandIDs::insertTermination,
    };

    commands.addArray (ids, numElementsInArray (ids));
}

// This method is used when something needs to find out the details about one of the commands
// that this object can perform..
void ContentComp::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    switch (commandID)
    {
    case CommandIDs::undo:
    	result.setInfo("Undo", "Undo last edit", CommandCategories::editing,0);
    	result.addDefaultKeypress('z', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::redo:
    	result.setInfo("Redo", "Undo last undo", CommandCategories::editing,0);
    	result.addDefaultKeypress('z', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
    	break;

    case StandardApplicationCommandIDs::cut:
    	result.setInfo("Cut", "", CommandCategories::editing,0);
    	result.addDefaultKeypress('x', ModifierKeys::commandModifier);
    	break;
    case StandardApplicationCommandIDs::copy:
    	result.setInfo("Copy", "", CommandCategories::editing,0);
    	result.addDefaultKeypress('c', ModifierKeys::commandModifier);
    	break;
    case StandardApplicationCommandIDs::paste:
    	result.setInfo("Paste", "", CommandCategories::editing,0);
    	result.addDefaultKeypress('v', ModifierKeys::commandModifier);
    	break;
    case StandardApplicationCommandIDs::selectAll:
    	result.setInfo("SelectAll", "", CommandCategories::editing,0);
    	result.addDefaultKeypress('a', ModifierKeys::commandModifier);
    	break;
    case StandardApplicationCommandIDs::deselectAll:
    	result.setInfo("DeselectAll", "", CommandCategories::editing,0);
    	result.addDefaultKeypress('a', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
    	break;
    case StandardApplicationCommandIDs::del:
    	result.setInfo("Delete", "", CommandCategories::editing,0);
    	result.addDefaultKeypress(KeyPress::deleteKey, 0);
    	break;
    case CommandIDs::segmentedConnectors:
    	result.setInfo("Segmeted connectors", "", CommandCategories::editing,0);
    	result.setTicked(StoredSettings::getInstance()->getIsSegmentedConnectors());
    	result.addDefaultKeypress('t', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::zoomIn:
    	result.setInfo("Zoom In", "", CommandCategories::editing,0);
    	result.addDefaultKeypress('+', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::zoomOut:
    	result.setInfo("Zoom Out", "", CommandCategories::editing,0);
    	result.addDefaultKeypress('-', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::zoomNormal:
    	result.setInfo("Zoom Normal", "", CommandCategories::editing,0);
    	result.addDefaultKeypress('=', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::reverseDirection:
    	result.setInfo("Reverse direction", "", CommandCategories::editing,0);
    	result.addDefaultKeypress('r', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::defineVariables:
    	result.setInfo("Define variable", "", CommandCategories::editing,0);
    	result.addDefaultKeypress('d', ModifierKeys::commandModifier);
    	break;


    case CommandIDs::insertMass:
    	result.setInfo("Mass", "", CommandCategories::inserting,0);
    	result.addDefaultKeypress('1', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::insertGround:
    	result.setInfo("Ground", "", CommandCategories::inserting,0);
    	result.addDefaultKeypress('2', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::insertResonator:
    	result.setInfo("Resonator", "", CommandCategories::inserting,0);
    	result.addDefaultKeypress('3', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::insertPort:
    	result.setInfo("Port", "", CommandCategories::inserting,0);
    	result.addDefaultKeypress('4', ModifierKeys::commandModifier);
    	break;

    case CommandIDs::insertLink:
    	result.setInfo("Linear Link", "", CommandCategories::inserting,0);
    	result.addDefaultKeypress('5', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::insertTouch:
    	result.setInfo("Touch Link", "", CommandCategories::inserting,0);
    	result.addDefaultKeypress('6', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::insertPluck:
    	result.setInfo("Pluck Link", "", CommandCategories::inserting,0);
    	result.addDefaultKeypress('7', ModifierKeys::commandModifier);
    	break;

    case CommandIDs::insertAudioOutput:
    	result.setInfo("Audio Output", "", CommandCategories::inserting,0);
    	result.addDefaultKeypress('8', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::insertWaveguide:
    	result.setInfo("Waveguide", "", CommandCategories::inserting,0);
    	result.addDefaultKeypress('9', ModifierKeys::commandModifier);
    	break;
    case CommandIDs::insertTermination:
    	result.setInfo("Termination", "", CommandCategories::inserting,0);
    	result.addDefaultKeypress('0', ModifierKeys::commandModifier);
    	break;

    default:
        break;
    };
}

// this is the ApplicationCommandTarget method that is used to actually perform one of our commands..
bool ContentComp::perform (const InvocationInfo& info)
{
	switch(info.commandID)
	{
    case CommandIDs::undo:
    	mainWindow.getUndoManager()->undo();
		break;
	case CommandIDs::redo:
		mainWindow.getUndoManager()->redo();
		break;

	case StandardApplicationCommandIDs::cut:
		break;
	case StandardApplicationCommandIDs::copy:
		break;
	case StandardApplicationCommandIDs::paste:
		break;
	case StandardApplicationCommandIDs::selectAll:
		break;
	case StandardApplicationCommandIDs::deselectAll:
		break;
	case StandardApplicationCommandIDs::del:
		DBG("delete");
		objController.removeObject("dev1");
		break;
    case CommandIDs::defineVariables:
    	break;
    case CommandIDs::segmentedConnectors:
    	StoredSettings::getInstance()->setIsSegmentedConnectors(!StoredSettings::getInstance()->getIsSegmentedConnectors());
    	break;
    case CommandIDs::zoomIn:
    	break;
    case CommandIDs::zoomOut:
    	break;
    case CommandIDs::zoomNormal:
    	break;
    case CommandIDs::reverseDirection:
    	break;

    case CommandIDs::insertMass:
    	objController.addObject(Ids::mass);
    	break;
    case CommandIDs::insertGround:
    	objController.addObject(Ids::ground);
    	break;
    case CommandIDs::insertResonator:
    	objController.addObject(Ids::resonator);
    	break;
    case CommandIDs::insertPort:
    	objController.addObject(Ids::port);
    	break;

    case CommandIDs::insertLink:
    	objController.addObject(Ids::link);
    	break;
    case CommandIDs::insertTouch:
    	objController.addObject(Ids::touch);
    	break;
    case CommandIDs::insertPluck:
    	objController.addObject(Ids::pluck);
    	break;

    case CommandIDs::insertAudioOutput:
    	objController.addObject(Ids::audioout);
    	break;
    case CommandIDs::insertWaveguide:
    	objController.addObject(Ids::waveguide);
    	break;
    case CommandIDs::insertTermination:
    	objController.addObject(Ids::termination);
    	break;
    default:
    	return false;
	}
	return true;
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
