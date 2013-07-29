/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "JuceHeader.h"
//[/Headers]

#include "MDLProperties.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace synthamodeler;
//[/MiscUserDefs]

//==============================================================================
MDLProperties::MDLProperties ()
{
    addAndMakeVisible (teMDLProps = new TextEditor ("teMDLProps"));
    teMDLProps->setMultiLine (true);
    teMDLProps->setReturnKeyStartsNewLine (false);
    teMDLProps->setReadOnly (true);
    teMDLProps->setScrollbarsShown (true);
    teMDLProps->setCaretVisible (false);
    teMDLProps->setPopupMenuEnabled (true);
    teMDLProps->setText (String::empty);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (300, 250);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MDLProperties::~MDLProperties()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    teMDLProps = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MDLProperties::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MDLProperties::resized()
{
    teMDLProps->setBounds (10, 10, 280, 230);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void MDLProperties::setContent(const String& s)
{
    teMDLProps->clear();
    teMDLProps->setText(s, false);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MDLProperties" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="300" initialHeight="250">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TEXTEDITOR name="teMDLProps" id="3d321bae99f7e300" memberName="teMDLProps"
              virtualName="" explicitFocusOrder="0" pos="10 10 280 230" initialText=""
              multiline="1" retKeyStartsLine="0" readonly="1" scrollbars="1"
              caret="0" popupmenu="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
