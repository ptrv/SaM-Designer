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
#include "../Application/CommonHeaders.h"
//[/Headers]

#include "PrefsMiscPage.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace synthamodeler;
//[/MiscUserDefs]

//==============================================================================
MiscPage::MiscPage ()
    : Component ("MiscPage")
{
    addAndMakeVisible (laDatDir = new Label ("laDatDir",
                                             "Data Directory"));
    laDatDir->setFont (Font (15.00f, Font::plain));
    laDatDir->setJustificationType (Justification::centredLeft);
    laDatDir->setEditable (false, false, false);
    laDatDir->setColour (TextEditor::textColourId, Colours::black);
    laDatDir->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (fcDataDir = new FilenameComponent ("Synth-A-Modeler data directory:",
                                                          File::nonexistent,
                                                          true, true, false,
                                                          String::empty, String::empty, "(select the Synth-A-Modeler data directory)"));
    fcDataDir->setName ("fcDataDir");

    addAndMakeVisible (laFaustDir = new Label ("laFaustDir",
                                               "FAUST Directory"));
    laFaustDir->setFont (Font (15.00f, Font::plain));
    laFaustDir->setJustificationType (Justification::centredLeft);
    laFaustDir->setEditable (false, false, false);
    laFaustDir->setColour (TextEditor::textColourId, Colours::black);
    laFaustDir->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (fcFaustDir = new FilenameComponent ("FAUST directory:",
                                                           File::nonexistent,
                                                           true, true, false,
                                                           String::empty, String::empty,
                                                           "(select the directory where the faust executable resides)"));
    fcFaustDir->setName ("fcFaustDir");

    addAndMakeVisible (laExternalEditor = new Label ("laExternalEditor",
                                                     "External Editor"));
    laExternalEditor->setFont (Font (15.00f, Font::plain));
    laExternalEditor->setJustificationType (Justification::centredLeft);
    laExternalEditor->setEditable (false, false, false);
    laExternalEditor->setColour (TextEditor::textColourId, Colours::black);
    laExternalEditor->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (fcExternalEditor = new FilenameComponent ("External editor:", File::nonexistent,
                                                                 true, false, false,
                                                                 String::empty, String::empty,
                                                                 "(select external editor executable)"));
    fcExternalEditor->setName ("fcExternalEditor");

    addAndMakeVisible (tbAutoCorrect = new ToggleButton ("tbAutoCorrect"));
    tbAutoCorrect->setButtonText ("Autocorrect parameter values");
    tbAutoCorrect->addListener (this);

    addAndMakeVisible (tbRunSAMBeforeExternal = new ToggleButton ("tbRunSAMBeforeExternal"));
    tbRunSAMBeforeExternal->setButtonText ("Compile before generating binary");
    tbRunSAMBeforeExternal->addListener (this);

    addAndMakeVisible (tbConfirmBeforeGeneration = new ToggleButton ("tbConfirmBeforeGeneration"));
    tbConfirmBeforeGeneration->setButtonText ("Confirm before generating code");
    tbConfirmBeforeGeneration->addListener (this);

    addAndMakeVisible (tbOpenDSPFileAfterExp = new ToggleButton ("tbOpenDSPFileAfterExp"));
    tbOpenDSPFileAfterExp->setButtonText ("Open *.dsp file after compilation");
    tbOpenDSPFileAfterExp->addListener (this);

    addAndMakeVisible (tbUseMDLX = new ToggleButton ("tbUseMDLX"));
    tbUseMDLX->setButtonText ("Use MDLX format");
    tbUseMDLX->addListener (this);

    addAndMakeVisible (tbLogging = new ToggleButton ("tbLogging"));
    tbLogging->setButtonText ("Logging (After change, restart required)");
    tbLogging->addListener (this);

    addAndMakeVisible (tbRedrawWhenNoPos = new ToggleButton ("tbRedrawWhenNoPos"));
    tbRedrawWhenNoPos->setButtonText ("Redraw model when no position data present");
    tbRedrawWhenNoPos->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    readValues();

    fcDataDir->addListener(this);
    fcFaustDir->addListener(this);
    fcExternalEditor->addListener(this);
    //[/Constructor]
}

MiscPage::~MiscPage()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    laDatDir = nullptr;
    fcDataDir = nullptr;
    laFaustDir = nullptr;
    fcFaustDir = nullptr;
    laExternalEditor = nullptr;
    fcExternalEditor = nullptr;
    tbAutoCorrect = nullptr;
    tbRunSAMBeforeExternal = nullptr;
    tbConfirmBeforeGeneration = nullptr;
    tbOpenDSPFileAfterExp = nullptr;
    tbUseMDLX = nullptr;
    tbLogging = nullptr;
    tbRedrawWhenNoPos = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MiscPage::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MiscPage::resized()
{
    laDatDir->setBounds (8, 8, 150, 24);
    fcDataDir->setBounds (8, 32, getWidth() - 16, 24);
    laFaustDir->setBounds (8, 64, 150, 24);
    fcFaustDir->setBounds (8, 88, getWidth() - 16, 24);
    laExternalEditor->setBounds (8, 120, 150, 24);
    fcExternalEditor->setBounds (8, 144, getWidth() - 16, 24);
    tbAutoCorrect->setBounds (8, 192, getWidth() - 16, 24);
    tbRunSAMBeforeExternal->setBounds (8, 224, getWidth() - 16, 24);
    tbConfirmBeforeGeneration->setBounds (8, 256, getWidth() - 16, 24);
    tbOpenDSPFileAfterExp->setBounds (8, 288, getWidth() - 16, 24);
    tbUseMDLX->setBounds (8, 320, getWidth() - 16, 24);
    tbLogging->setBounds (8, 352, getWidth() - 16, 24);
    tbRedrawWhenNoPos->setBounds (8, 384, getWidth() - 16, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MiscPage::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == tbAutoCorrect)
    {
        //[UserButtonCode_tbAutoCorrect] -- add your button handler code here..
        StoredSettings::getInstance()->setAutoCorrectValues(tbAutoCorrect->getToggleState());
        //[/UserButtonCode_tbAutoCorrect]
    }
    else if (buttonThatWasClicked == tbRunSAMBeforeExternal)
    {
        //[UserButtonCode_tbRunSAMBeforeExternal] -- add your button handler code here..
        StoredSettings::getInstance()->setRunSAMBeforeExternal(tbRunSAMBeforeExternal->getToggleState());
        //[/UserButtonCode_tbRunSAMBeforeExternal]
    }
    else if (buttonThatWasClicked == tbConfirmBeforeGeneration)
    {
        //[UserButtonCode_tbConfirmBeforeGeneration] -- add your button handler code here..
        StoredSettings::getInstance()->setIsExportConfirm(tbConfirmBeforeGeneration->getToggleState());
        //[/UserButtonCode_tbConfirmBeforeGeneration]
    }
    else if (buttonThatWasClicked == tbOpenDSPFileAfterExp)
    {
        //[UserButtonCode_tbOpenDSPFileAfterExp] -- add your button handler code here..
        StoredSettings::getInstance()->setOpenFaustExport(tbOpenDSPFileAfterExp->getToggleState());
        //[/UserButtonCode_tbOpenDSPFileAfterExp]
    }
    else if (buttonThatWasClicked == tbUseMDLX)
    {
        //[UserButtonCode_tbUseMDLX] -- add your button handler code here..
        StoredSettings::getInstance()->setIsUsingMDLX(tbUseMDLX->getToggleState());
        //[/UserButtonCode_tbUseMDLX]
    }
    else if (buttonThatWasClicked == tbLogging)
    {
        //[UserButtonCode_tbLogging] -- add your button handler code here..
        StoredSettings::getInstance()->setIsLoggingOn(tbLogging->getToggleState());
        //[/UserButtonCode_tbLogging]
    }
    else if (buttonThatWasClicked == tbRedrawWhenNoPos)
    {
        //[UserButtonCode_tbRedrawWhenNoPos] -- add your button handler code here..
        StoredSettings::getInstance()->setShouldRedrawOnLoad(tbRedrawWhenNoPos->getToggleState());
        //[/UserButtonCode_tbRedrawWhenNoPos]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MiscPage::filenameComponentChanged(FilenameComponent* fileComponentThatHasChanged)
{
    if (fileComponentThatHasChanged == fcDataDir)
    {
        StoredSettings::getInstance()->setDataDir(fcDataDir->getCurrentFile().getFullPathName());
    }
    else if (fileComponentThatHasChanged == fcFaustDir)
    {
        StoredSettings::getInstance()->setFaustDir(fcFaustDir->getCurrentFile().getFullPathName());
    }
    else if (fileComponentThatHasChanged == fcExternalEditor)
    {
        StoredSettings::getInstance()->setExternalEditor(fcExternalEditor->getCurrentFile().getFullPathName());
    }
}

void MiscPage::readValues()
{
    fcDataDir->setCurrentFile(StoredSettings::getInstance()->getDataDir(),
                              true, dontSendNotification);
    fcFaustDir->setCurrentFile(StoredSettings::getInstance()->getFaustDir(),
                               true, dontSendNotification);
    fcExternalEditor->setCurrentFile(StoredSettings::getInstance()->getExternalEditor(),
                                     true, dontSendNotification);
    tbAutoCorrect->setToggleState(StoredSettings::getInstance()->getAutoCorrectValues(),
                                  dontSendNotification);
    tbRunSAMBeforeExternal->setToggleState(StoredSettings::getInstance()->getRunSAMBeforeExternal(),
                                           dontSendNotification);
    tbConfirmBeforeGeneration->setToggleState(StoredSettings::getInstance()->getIsExportConfirm(),
                                              dontSendNotification);
    tbOpenDSPFileAfterExp->setToggleState(StoredSettings::getInstance()->getOpenFaustExport(),
                                          dontSendNotification);
    tbUseMDLX->setToggleState(StoredSettings::getInstance()->getIsUsingMDLX(),
                              dontSendNotification);
    tbLogging->setToggleState(StoredSettings::getInstance()->getIsLoggingOn(),
                              dontSendNotification);
    tbRedrawWhenNoPos->setToggleState(StoredSettings::getInstance()->getShouldRedrawOnLoad(),
                                      dontSendNotification);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MiscPage" componentName="MiscPage"
                 parentClasses="public Component, public FilenameComponentListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330000013" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
  <LABEL name="laDatDir" id="484351012dae21b6" memberName="laDatDir" virtualName=""
         explicitFocusOrder="0" pos="8 8 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Data Directory" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="fcDataDir" id="f24633023217d0b6" memberName="fcDataDir"
                    virtualName="" explicitFocusOrder="0" pos="8 32 16M 24" class="FilenameComponent"
                    params="&quot;Synth-A-Modeler data directory:&quot;,&#10;File::nonexistent,&#10;true, true, false,&#10;String::empty, String::empty, &quot;(select the Synth-A-Modeler data directory)&quot;"/>
  <LABEL name="laFaustDir" id="79c7c7ec20ae7bde" memberName="laFaustDir"
         virtualName="" explicitFocusOrder="0" pos="8 64 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="FAUST Directory" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="fcFaustDir" id="c593cebcb2a05f1d" memberName="fcFaustDir"
                    virtualName="" explicitFocusOrder="0" pos="8 88 16M 24" class="FilenameComponent"
                    params="&quot;FAUST directory:&quot;,&#10;File::nonexistent,&#10;true, true, false,&#10;String::empty, String::empty,&#10;&quot;(select the directory where the faust executable resides)&quot;"/>
  <LABEL name="laExternalEditor" id="cebcc458dcd6db2b" memberName="laExternalEditor"
         virtualName="" explicitFocusOrder="0" pos="8 120 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="External Editor" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="fcExternalEditor" id="34adc001253d375f" memberName="fcExternalEditor"
                    virtualName="" explicitFocusOrder="0" pos="8 144 16M 24" class="FilenameComponent"
                    params="&quot;External editor:&quot;, File::nonexistent,&#10;true, false, false,&#10;String::empty, String::empty,&#10;&quot;(select external editor executable)&quot;"/>
  <TOGGLEBUTTON name="tbAutoCorrect" id="bf80045c13fd1366" memberName="tbAutoCorrect"
                virtualName="" explicitFocusOrder="0" pos="8 192 16M 24" buttonText="Autocorrect parameter values"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="tbRunSAMBeforeExternal" id="e19e844bbcc45450" memberName="tbRunSAMBeforeExternal"
                virtualName="" explicitFocusOrder="0" pos="8 224 16M 24" buttonText="Compile before generating binary"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="tbConfirmBeforeGeneration" id="38d630ddadac6157" memberName="tbConfirmBeforeGeneration"
                virtualName="" explicitFocusOrder="0" pos="8 256 16M 24" buttonText="Confirm before generating code"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="tbOpenDSPFileAfterExp" id="e2fc5a93d51d6fc8" memberName="tbOpenDSPFileAfterExp"
                virtualName="" explicitFocusOrder="0" pos="8 288 16M 24" buttonText="Open *.dsp file after compilation"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="tbUseMDLX" id="c0223498bd8f5b96" memberName="tbUseMDLX"
                virtualName="" explicitFocusOrder="0" pos="8 320 16M 24" buttonText="Use MDLX format"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="tbLogging" id="69bceff3e30923de" memberName="tbLogging"
                virtualName="" explicitFocusOrder="0" pos="8 352 16M 24" buttonText="Logging (After change, restart required)"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="tbRedrawWhenNoPos" id="580755024d1dbf83" memberName="tbRedrawWhenNoPos"
                virtualName="" explicitFocusOrder="0" pos="8 384 16M 24" buttonText="Redraw model when no position data present"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
