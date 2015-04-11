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
//[/Headers]

#include "RedrawOptionsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#include "Application/CommonHeaders.h"

#include "RedrawOptionsPanel.h"


using namespace synthamodeler;
//[/MiscUserDefs]

//==============================================================================
RedrawOptionsComponent::RedrawOptionsComponent (RedrawOptionsPanel* rop_)
    : rop(rop_), pf(StoredSettings::getInstance()->getProps())
{
    addAndMakeVisible (laAlpha = new Label ("laAlpha",
                                            TRANS("Mass")));
    laAlpha->setFont (Font (15.00f, Font::plain));
    laAlpha->setJustificationType (Justification::centredLeft);
    laAlpha->setEditable (false, false, false);
    laAlpha->setColour (TextEditor::textColourId, Colours::black);
    laAlpha->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slAlpha = new Slider ("slAlpha"));
    slAlpha->setRange (0.1, 10, 0.1);
    slAlpha->setSliderStyle (Slider::LinearHorizontal);
    slAlpha->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slAlpha->addListener (this);

    addAndMakeVisible (laBeta = new Label ("laBeta",
                                           TRANS("Charge")));
    laBeta->setFont (Font (15.00f, Font::plain));
    laBeta->setJustificationType (Justification::centredLeft);
    laBeta->setEditable (false, false, false);
    laBeta->setColour (TextEditor::textColourId, Colours::black);
    laBeta->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slBeta = new Slider ("slBeta"));
    slBeta->setRange (100, 2000, 1);
    slBeta->setSliderStyle (Slider::LinearHorizontal);
    slBeta->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slBeta->addListener (this);

    addAndMakeVisible (laK = new Label ("laK",
                                        TRANS("Spring Constant k")));
    laK->setFont (Font (15.00f, Font::plain));
    laK->setJustificationType (Justification::centredLeft);
    laK->setEditable (false, false, false);
    laK->setColour (TextEditor::textColourId, Colours::black);
    laK->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slK = new Slider ("slK"));
    slK->setRange (0.001, 0.3, 0.001);
    slK->setSliderStyle (Slider::LinearHorizontal);
    slK->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slK->addListener (this);
    slK->setSkewFactor (0.6);

    addAndMakeVisible (laDamp = new Label ("laDamp",
                                           TRANS("Damping")));
    laDamp->setFont (Font (15.00f, Font::plain));
    laDamp->setJustificationType (Justification::centredLeft);
    laDamp->setEditable (false, false, false);
    laDamp->setColour (TextEditor::textColourId, Colours::black);
    laDamp->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slDamp = new Slider ("slDamp"));
    slDamp->setRange (0.1, 0.99, 0.01);
    slDamp->setSliderStyle (Slider::LinearHorizontal);
    slDamp->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slDamp->addListener (this);

    addAndMakeVisible (laEnergy = new Label ("laEnergy",
                                             TRANS("Energy")));
    laEnergy->setFont (Font (15.00f, Font::plain));
    laEnergy->setJustificationType (Justification::centredLeft);
    laEnergy->setEditable (false, false, false);
    laEnergy->setColour (TextEditor::textColourId, Colours::black);
    laEnergy->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slEnergy = new Slider ("slEnergy"));
    slEnergy->setRange (0.001, 10, 0.001);
    slEnergy->setSliderStyle (Slider::LinearHorizontal);
    slEnergy->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slEnergy->addListener (this);
    slEnergy->setSkewFactor (0.35);

    addAndMakeVisible (laTimestep = new Label ("laTimestep",
                                               TRANS("Timestep (ms)")));
    laTimestep->setFont (Font (15.00f, Font::plain));
    laTimestep->setJustificationType (Justification::centredLeft);
    laTimestep->setEditable (false, false, false);
    laTimestep->setColour (TextEditor::textColourId, Colours::black);
    laTimestep->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slTimestep = new Slider ("slTimestamp"));
    slTimestep->setRange (10, 200, 1);
    slTimestep->setSliderStyle (Slider::LinearHorizontal);
    slTimestep->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slTimestep->addListener (this);

    addAndMakeVisible (btOk = new TextButton ("btOk"));
    btOk->setButtonText (TRANS("Ok"));
    btOk->addListener (this);

    addAndMakeVisible (btCancel = new TextButton ("btCancel"));
    btCancel->setButtonText (TRANS("Cancel"));
    btCancel->addListener (this);

    addAndMakeVisible (btToggleRandomize = new ToggleButton ("toggleRandomize"));
    btToggleRandomize->setButtonText (TRANS("randomize nodes before start"));
    btToggleRandomize->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (400, 350);


    //[Constructor] You can add your own custom stuff here..
    readOptions();
    //[/Constructor]
}

RedrawOptionsComponent::~RedrawOptionsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    laAlpha = nullptr;
    slAlpha = nullptr;
    laBeta = nullptr;
    slBeta = nullptr;
    laK = nullptr;
    slK = nullptr;
    laDamp = nullptr;
    slDamp = nullptr;
    laEnergy = nullptr;
    slEnergy = nullptr;
    laTimestep = nullptr;
    slTimestep = nullptr;
    btOk = nullptr;
    btCancel = nullptr;
    btToggleRandomize = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void RedrawOptionsComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void RedrawOptionsComponent::resized()
{
    laAlpha->setBounds (12, 52, 104, 24);
    slAlpha->setBounds (122, 52, 250, 24);
    laBeta->setBounds (12, 92, 104, 24);
    slBeta->setBounds (122, 92, 250, 24);
    laK->setBounds (12, 132, 104, 24);
    slK->setBounds (122, 132, 250, 24);
    laDamp->setBounds (12, 172, 104, 24);
    slDamp->setBounds (122, 172, 250, 24);
    laEnergy->setBounds (12, 212, 104, 24);
    slEnergy->setBounds (122, 212, 250, 24);
    laTimestep->setBounds (14, 252, 104, 24);
    slTimestep->setBounds (122, 252, 250, 24);
    btOk->setBounds ((getWidth() / 2) + -40 - ((60) / 2), getHeight() - 20 - 20, 60, 20);
    btCancel->setBounds ((getWidth() / 2) + 40 - ((60) / 2), getHeight() - 20 - 20, 60, 20);
    btToggleRandomize->setBounds (14, 16, 348, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void RedrawOptionsComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slAlpha)
    {
        //[UserSliderCode_slAlpha] -- add your slider handling code here..
        //[/UserSliderCode_slAlpha]
    }
    else if (sliderThatWasMoved == slBeta)
    {
        //[UserSliderCode_slBeta] -- add your slider handling code here..
        //[/UserSliderCode_slBeta]
    }
    else if (sliderThatWasMoved == slK)
    {
        //[UserSliderCode_slK] -- add your slider handling code here..
        //[/UserSliderCode_slK]
    }
    else if (sliderThatWasMoved == slDamp)
    {
        //[UserSliderCode_slDamp] -- add your slider handling code here..
        //[/UserSliderCode_slDamp]
    }
    else if (sliderThatWasMoved == slEnergy)
    {
        //[UserSliderCode_slEnergy] -- add your slider handling code here..
        //[/UserSliderCode_slEnergy]
    }
    else if (sliderThatWasMoved == slTimestep)
    {
        //[UserSliderCode_slTimestep] -- add your slider handling code here..
        //[/UserSliderCode_slTimestep]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void RedrawOptionsComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btOk)
    {
        //[UserButtonCode_btOk] -- add your button handler code here..
        saveOptions();
        rop->closeButtonPressed();
        //[/UserButtonCode_btOk]
    }
    else if (buttonThatWasClicked == btCancel)
    {
        //[UserButtonCode_btCancel] -- add your button handler code here..
        rop->closeButtonPressed();
        //[/UserButtonCode_btCancel]
    }
    else if (buttonThatWasClicked == btToggleRandomize)
    {
        //[UserButtonCode_btToggleRandomize] -- add your button handler code here..
        //[/UserButtonCode_btToggleRandomize]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void RedrawOptionsComponent::readOptions()
{
    slAlpha->setValue(pf.getDoubleValue("redrawparam_alpha", 1.0));
    slBeta->setValue(pf.getDoubleValue("redrawparam_beta", 1000));
    slK->setValue(pf.getDoubleValue("redrawparam_k", 0.02));
    slDamp->setValue(pf.getDoubleValue("redrawparam_damp", 0.85));
    slEnergy->setValue(pf.getDoubleValue("redrawparam_energy", 0.01));
    slTimestep->setValue(pf.getDoubleValue("redrawparam_timestep", 0.5));
    btToggleRandomize->setToggleState(pf.getBoolValue("redrawparam_randomize", true), dontSendNotification);
}

void RedrawOptionsComponent::saveOptions()
{
    pf.setValue("redrawparam_alpha", slAlpha->getValue());
    pf.setValue("redrawparam_beta", slBeta->getValue());
    pf.setValue("redrawparam_k", slK->getValue());
    pf.setValue("redrawparam_damp", slDamp->getValue());
    pf.setValue("redrawparam_energy", slEnergy->getValue());
    pf.setValue("redrawparam_timestep", slTimestep->getValue());
    pf.setValue("redrawparam_randomize", btToggleRandomize->getToggleState());
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="RedrawOptionsComponent" componentName=""
                 parentClasses="public Component" constructorParams="RedrawOptionsPanel* rop_"
                 variableInitialisers="rop(rop_), pf(StoredSettings::getInstance()-&gt;getProps())"
                 snapPixels="2" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="400" initialHeight="350">
  <BACKGROUND backgroundColour="ffffffff"/>
  <LABEL name="laAlpha" id="6898834d3ae53f44" memberName="laAlpha" virtualName=""
         explicitFocusOrder="0" pos="12 52 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Mass" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <SLIDER name="slAlpha" id="a0467824d6e9c8e7" memberName="slAlpha" virtualName=""
          explicitFocusOrder="0" pos="122 52 250 24" min="0.10000000000000000555"
          max="10" int="0.10000000000000000555" style="LinearHorizontal"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="laBeta" id="afe5e0051dcb27a9" memberName="laBeta" virtualName=""
         explicitFocusOrder="0" pos="12 92 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Charge" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <SLIDER name="slBeta" id="ba31f1fe67c537d6" memberName="slBeta" virtualName=""
          explicitFocusOrder="0" pos="122 92 250 24" min="100" max="2000"
          int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="laK" id="b7409306b3a8c462" memberName="laK" virtualName=""
         explicitFocusOrder="0" pos="12 132 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Spring Constant k" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <SLIDER name="slK" id="99f94ecb3460b725" memberName="slK" virtualName=""
          explicitFocusOrder="0" pos="122 132 250 24" min="0.0010000000000000000208"
          max="0.2999999999999999889" int="0.0010000000000000000208" style="LinearHorizontal"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="0.5999999999999999778"/>
  <LABEL name="laDamp" id="311a6937b8c46091" memberName="laDamp" virtualName=""
         explicitFocusOrder="0" pos="12 172 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Damping" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <SLIDER name="slDamp" id="f398c64eb286e1ca" memberName="slDamp" virtualName=""
          explicitFocusOrder="0" pos="122 172 250 24" min="0.10000000000000000555"
          max="0.98999999999999999112" int="0.010000000000000000208" style="LinearHorizontal"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="laEnergy" id="57ee54815d583117" memberName="laEnergy" virtualName=""
         explicitFocusOrder="0" pos="12 212 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Energy" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <SLIDER name="slEnergy" id="31979231a22fd55b" memberName="slEnergy" virtualName=""
          explicitFocusOrder="0" pos="122 212 250 24" min="0.0010000000000000000208"
          max="10" int="0.0010000000000000000208" style="LinearHorizontal"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="0.3499999999999999778"/>
  <LABEL name="laTimestep" id="428812339125c8eb" memberName="laTimestep"
         virtualName="" explicitFocusOrder="0" pos="14 252 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Timestep (ms)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <SLIDER name="slTimestamp" id="f34538af30f4ec8d" memberName="slTimestep"
          virtualName="" explicitFocusOrder="0" pos="122 252 250 24" min="10"
          max="200" int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="btOk" id="21b281d5f8eed9a" memberName="btOk" virtualName=""
              explicitFocusOrder="0" pos="-40Cc 20Rr 60 20" buttonText="Ok"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btCancel" id="beef51685737d7ba" memberName="btCancel" virtualName=""
              explicitFocusOrder="0" pos="40Cc 20Rr 60 20" buttonText="Cancel"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="toggleRandomize" id="b2ec4b9e873e1741" memberName="btToggleRandomize"
                virtualName="" explicitFocusOrder="0" pos="14 16 348 24" buttonText="randomize nodes before start"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
