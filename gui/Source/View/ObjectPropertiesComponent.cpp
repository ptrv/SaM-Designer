/*
  ==============================================================================

    ObjectPropertiesComponent.cpp
    Created: 24 Jul 2013 10:46:55am
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
#include "../Controller/ObjController.h"

#include "ObjectPropertiesComponent.h"

using namespace synthamodeler;

ObjectPropertiesComponent::ObjectPropertiesComponent(ObjController* objController_,
                                                     ValueTree data_,
                                                     UndoManager* undoManager_)
:
objController(objController_),
data(data_),
undoManager(undoManager_),
laName("laName", "Identifier"),
teName("teName"),
laDebug(""),
dataChanged(false)
{
    addAndMakeVisible(&teName);
    teName.addListener(this);
    laName.attachToComponent(&teName, true);
#ifdef _DEBUG
    addAndMakeVisible(&laDebug);
    laDebug.setText("Pos: " + data[Ids::posX].toString() + String(" ") +
                    data[Ids::posY].toString(), dontSendNotification);
#endif
}

ObjectPropertiesComponent::~ObjectPropertiesComponent()
{
}

void ObjectPropertiesComponent::resized()
{
    teName.setBounds(100, 10, getWidth() - 110, 22);
    laDebug.setBounds(65, getHeight() - 60, 100, 22);
}

void ObjectPropertiesComponent::textEditorTextChanged(TextEditor& editor)
{
    dataChanged = true;
}

void ObjectPropertiesComponent::textEditorReturnKeyPressed(TextEditor& /*editor*/)
{
    applyEditing();
}

void ObjectPropertiesComponent::textEditorEscapeKeyPressed(TextEditor& /*editor*/)
{
    cancelEditing();
    if(PropertiesWindow* pw = dynamic_cast <PropertiesWindow*> (getParentComponent()))
    {
        pw->closeButtonPressed();
    }
}

void ObjectPropertiesComponent::textEditorFocusLost(TextEditor& /*editor*/)
{
}

void ObjectPropertiesComponent::applyEditing()
{
    undoManager->beginNewTransaction("Change object properties");
    if (dataChanged)
    {
        bool writeOk = writeValues();
        undoManager->beginNewTransaction();
        if (writeOk)
        {
            SAM_LOG("Change " + data.getType().toString() + " " +
                    data[Ids::identifier].toString());
        }
        else
        {
            SAM_LOG("Name already exists");
            Alerts::nameAlreadyExists();
        }
    }
}

void ObjectPropertiesComponent::cancelEditing()
{
    SAM_LOG("Cancel change " + data.getType().toString() + " " +
            data[Ids::identifier].toString());
}

bool ObjectPropertiesComponent::writeIdentifier()
{
    String newName = teName.getText();
    String oldName = data[Ids::identifier];
    if (newName != oldName)
    {
        if (objController->checkIfIdExists(data.getType(), newName))
        {
            return false;
        }
        else
        {
            if (!objController->renameId(data.getType(), oldName, newName, undoManager))
                return false;
        }

        // Change names in connected objects
        if(data.getType() == Ids::mass || data.getType() == Ids::port
            || data.getType() == Ids::ground || data.getType() == Ids::resonators)
        {
            objController->changeObjectNameInLink(oldName, newName, undoManager);
            objController->changeObjectNameInAudioSources(oldName, newName, undoManager);
        }
        else if(data.getType() == Ids::link || data.getType() == Ids::touch
            || data.getType() == Ids::pluck || data.getType() == Ids::pulsetouch)
        {
            objController->changeObjectNameInAudioSources(oldName, newName, undoManager);
        }
        else if(data.getType() == Ids::junction)
        {
            objController->changeObjectNameInLink(oldName, newName, undoManager);
            objController->changeObjectNameInAudioSources(oldName, newName, undoManager);
        }
        else if(data.getType() == Ids::termination)
        {
            objController->changeObjectNameInAudioSources(oldName, newName, undoManager);
        }

        data.setProperty(Ids::identifier, newName, undoManager);
    }
    return true;
}

MassPropertiesComponent::MassPropertiesComponent(ObjController* objController_,
                                                 ValueTree data_,
                                                 UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, data_, undoManager_),
laMass("laMass", "Mass (kg)"),
teMass("teMass"),
laPos("laPos", "Position (m)"),
tePos("tePos"),
laVel("laVel", "Velocity (m/s)"),
teVel("teVel")
{
    teMass.addListener(this);
    addAndMakeVisible(&teMass);
    laMass.attachToComponent(&teMass, true);
    tePos.addListener(this);
    addAndMakeVisible(&tePos);
    laPos.attachToComponent(&tePos, true);
    teVel.addListener(this);
    addAndMakeVisible(&teVel);
    laVel.attachToComponent(&teVel, true);

    readValues();
}

MassPropertiesComponent::~MassPropertiesComponent()
{

}

void MassPropertiesComponent::resized()
{
    ObjectPropertiesComponent::resized();

    teMass.setBounds(100, 40, getWidth() - 110, 22);
    tePos.setBounds(100, 70, getWidth() - 110, 22);
    teVel.setBounds(100, 100, getWidth() - 110, 22);
}

void MassPropertiesComponent::readValues()
{
    teName.setText(data[Ids::identifier].toString());
    teMass.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
    tePos.setText(data.getChildWithName(Ids::parameters).getChild(1)[Ids::value].toString());
    teVel.setText(data.getChildWithName(Ids::parameters).getChild(2)[Ids::value].toString());

}

bool MassPropertiesComponent::writeValues()
{
    if(! writeIdentifier())
        return false;

    ValueTree paramsTree = data.getChildWithName(Ids::parameters);
    ValueTree pa1 = paramsTree.getChild(0);
    ValueTree pa2 = paramsTree.getChild(1);
    ValueTree pa3 = paramsTree.getChild(2);
    pa1.setProperty(Ids::value,
                    Utils::fixParameterValueIfNeeded(teMass.getText()),
                    undoManager);
    pa2.setProperty(Ids::value,
                    Utils::fixParameterValueIfNeeded(tePos.getText()),
                    undoManager);
    pa3.setProperty(Ids::value,
                    Utils::fixParameterValueIfNeeded(teVel.getText()),
                    undoManager);

    // Change object reference in attached links
    //        objController->changeObjectNameInLink(oldName, newName, undoManager);
    return true;
}

PortPropertiesComponent::PortPropertiesComponent(ObjController* objController_,
                                                 ValueTree data_,
                                                 UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, data_, undoManager_)
{
    readValues();
}

PortPropertiesComponent::~PortPropertiesComponent()
{

}

void PortPropertiesComponent::resized()
{
    ObjectPropertiesComponent::resized();
}

void PortPropertiesComponent::readValues()
{
    teName.setText(data[Ids::identifier].toString());
}

bool PortPropertiesComponent::writeValues()
{
    if(writeIdentifier())
        return true;
    else
        return false;
}

ResonatorPropertiesComponent::ResonatorPropertiesComponent(ObjController* objController_,
                             ValueTree data_,
                             UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, data_, undoManager_),
laFreq("laFreq", "Frequency (Hz)"),
teFreq("teFreq"),
laDecay("laDecay", "Decay (sec)"),
teDecay("teDecay"),
laEqMass("laEqMass", "Mass (kg)"),
teEqMass("teEqMass")
{
    teFreq.addListener(this);
    addAndMakeVisible(&teFreq);
    laFreq.attachToComponent(&teFreq, true);
    teDecay.addListener(this);
    addAndMakeVisible(&teDecay);
    laDecay.attachToComponent(&teDecay, true);
    teEqMass.addListener(this);
    addAndMakeVisible(&teEqMass);
    laEqMass.attachToComponent(&teEqMass, true);

    readValues();
}

ResonatorPropertiesComponent::~ResonatorPropertiesComponent()
{

}

void ResonatorPropertiesComponent::resized()
{
    ObjectPropertiesComponent::resized();

    teFreq.setBounds(100, 40, getWidth() - 110, 22);
    teDecay.setBounds(100, 70, getWidth() - 110, 22);
    teEqMass.setBounds(100, 100, getWidth() - 110, 22);
}

void ResonatorPropertiesComponent::readValues()
{
    teName.setText(data[Ids::identifier].toString());

    String valFreq, valDecay, valEqMass;

    int numRes = data.getChildWithName(Ids::parameters).getNumChildren() / 3;
    jassert(numRes != 0);
    for (int i = 0; i < numRes; ++i)
    {
        String delim = ",";
        if (i == 0)
            delim = "";
        valFreq << delim << data.getChildWithName(Ids::parameters).getChild(i * 3 + 0)[Ids::value].toString();
        valDecay << delim << data.getChildWithName(Ids::parameters).getChild(i * 3 + 1)[Ids::value].toString();
        valEqMass << delim << data.getChildWithName(Ids::parameters).getChild(i * 3 + 2)[Ids::value].toString();
    }
    teFreq.setText(valFreq);
    teDecay.setText(valDecay);
    teEqMass.setText(valEqMass);

}

bool ResonatorPropertiesComponent::writeValues()
{
    if(! writeIdentifier())
        return false;
    
    ValueTree paramsTree = data.getChildWithName(Ids::parameters);

    String valFreq = teFreq.getText();
    String valDecay = teDecay.getText();
    String valEqMass = teEqMass.getText();

    StringArray freqArr;
    freqArr.addTokens(valFreq, ",", "\"");
    StringArray decayArr;
    decayArr.addTokens(valDecay, ",", "\"");
    StringArray eqMassArr;
    eqMassArr.addTokens(valEqMass, ",", "\"");
    paramsTree.removeAllChildren(undoManager);

    int numRes = jmin<int>(freqArr.size(), decayArr.size(), eqMassArr.size());
    for (int k = 0; k < numRes; ++k)
    {
        ValueTree pa1(Ids::parameter);
        ValueTree pa2(Ids::parameter);
        ValueTree pa3(Ids::parameter);
        pa1.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(freqArr[k]),
                        undoManager);
        pa2.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(decayArr[k]),
                        undoManager);
        pa3.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(eqMassArr[k]),
                        undoManager);
        paramsTree.addChild(pa1, -1, undoManager);
        paramsTree.addChild(pa2, -1, undoManager);
        paramsTree.addChild(pa3, -1, undoManager);
    }
    return true;
}

GroundPropertiesComponent::GroundPropertiesComponent(ObjController* objController_,
                          ValueTree data_,
                          UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, data_, undoManager_),
laPos("laPos", "Position (m)"),
tePos("tePos")
{
    tePos.addListener(this);
    addAndMakeVisible(&tePos);
    laPos.attachToComponent(&tePos, true);

    readValues();
}

GroundPropertiesComponent::~GroundPropertiesComponent()
{

}

void GroundPropertiesComponent::resized()
{
    ObjectPropertiesComponent::resized();

    tePos.setBounds(100, 40, getWidth() - 110, 22);
}

void GroundPropertiesComponent::readValues()
{
    teName.setText(data[Ids::identifier].toString());
    tePos.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
}

bool GroundPropertiesComponent::writeValues()
{
    if(! writeIdentifier())
        return false;

    ValueTree paramsTree = data.getChildWithName(Ids::parameters);
    ValueTree param = paramsTree.getChild(0);
    param.setProperty(Ids::value, tePos.getText(), undoManager);
    return true;
}

LinkPropertiesComponent::LinkPropertiesComponent(ObjController* objController_,
                        ValueTree data_,
                        UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, data_, undoManager_),
laStiff("laStiff", "Stiffness (N/m)"),
teStiff("teStiff"),
laDamp("laDamp", "Damping (N/(m/s))"),
teDamp("teDamp"),
laPos("laPos", "Position offset (m)"),
tePos("tePos"),
laStartVertex("laStartVertex", "Start Vertex"),
teStartVertex("teStartVertex"),
laEndVertex("laEndVertex", "End Vertex"),
teEndVertex("teEndVertex"),
laMinDisplace("laMinDisplace", "Minimum displce diff"),
teMinDisplace("teMinDisplace"),
laPulseMult("laPulseMult", "Pulse multiplier"),
tePulseMult("tePulseMult"),
laPulseTau("laPulseTau", "Pulse tau"),
tePulseTau("tePulseTau"),
laPulseLen("laPulseLen", "Pulse length"),
tePulseLen("tePulseLen")
{
    teStiff.addListener(this);
    addAndMakeVisible(&teStiff);
    laStiff.attachToComponent(&teStiff, true);
    teDamp.addListener(this);
    addAndMakeVisible(&teDamp);
    laDamp.attachToComponent(&teDamp, true);
    tePos.addListener(this);
    addAndMakeVisible(&tePos);
    laPos.attachToComponent(&tePos, true);
    teStartVertex.setReadOnly(true);
    teStartVertex.addListener(this);
    teStartVertex.setColour(TextEditor::textColourId, Colours::darkgrey);
    addAndMakeVisible(&teStartVertex);
    laStartVertex.attachToComponent(&teStartVertex, true);
    teEndVertex.setReadOnly(true);
    teEndVertex.addListener(this);
    teEndVertex.setColour(TextEditor::textColourId, Colours::darkgrey);
    addAndMakeVisible(&teEndVertex);
    laEndVertex.attachToComponent(&teEndVertex, true);
    if (data.getType() == Ids::pluck)
    {
        teMinDisplace.addListener(this);
        addAndMakeVisible(&teMinDisplace);
        laMinDisplace.attachToComponent(&teMinDisplace, true);
    }
    else if (data.getType() == Ids::pulsetouch)
    {
        tePulseMult.addListener(this);
        addAndMakeVisible(&tePulseMult);
        laPulseMult.attachToComponent(&tePulseMult, true);
        tePulseTau.addListener(this);
        addAndMakeVisible(&tePulseTau);
        laPulseTau.attachToComponent(&tePulseTau, true);
        tePulseLen.addListener(this);
        addAndMakeVisible(&tePulseLen);
        laPulseLen.attachToComponent(&tePulseLen, true);
    }
    readValues();
}

LinkPropertiesComponent::~LinkPropertiesComponent()
{

}

void LinkPropertiesComponent::resized()
{
    ObjectPropertiesComponent::resized();
    int offset = 0;
    teStiff.setBounds(100, 40, getWidth() - 110, 22);
    teDamp.setBounds(100, 70, getWidth() - 110, 22);
    if (data.getType() == Ids::pluck)
    {
        teMinDisplace.setBounds(100, 100, getWidth() - 110, 22);
        offset = 30;
    }
    else if (data.getType() == Ids::pulsetouch)
    {
        tePulseMult.setBounds(100, 100, getWidth() - 110, 22);
        tePulseTau.setBounds(100, 130, getWidth() - 110, 22);
        tePulseLen.setBounds(100, 190, getWidth() - 110, 22);
        offset = 90;
    }
    tePos.setBounds(100, 100 + offset, getWidth() - 110, 22);
    teStartVertex.setBounds(100, 130 + offset, getWidth() - 110, 22);
    teEndVertex.setBounds(100, 160 + offset, getWidth() - 110, 22);
}

void LinkPropertiesComponent::readValues()
{
    teName.setText(data[Ids::identifier].toString());
    teStiff.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
    teDamp.setText(data.getChildWithName(Ids::parameters).getChild(1)[Ids::value].toString());
    int offset = 0;
    if (data.getType() == Ids::pluck)
    {
        teMinDisplace.setText(data.getChildWithName(Ids::parameters).getChild(2)[Ids::value].toString());
        ++offset;
    }
    else if (data.getType() == Ids::pulsetouch)
    {
        tePulseMult.setText(data.getChildWithName(Ids::parameters).getChild(3)[Ids::value].toString());
        tePulseTau.setText(data.getChildWithName(Ids::parameters).getChild(4)[Ids::value].toString());
        tePulseLen.setText(data.getChildWithName(Ids::parameters).getChild(5)[Ids::value].toString());
    }
    tePos.setText(data.getChildWithName(Ids::parameters).getChild(2 + offset)[Ids::value].toString());
    teStartVertex.setText(data[Ids::startVertex].toString());
    teEndVertex.setText(data[Ids::endVertex].toString());
}

bool LinkPropertiesComponent::writeValues()
{
    if(! writeIdentifier())
        return false;

    ValueTree paramsTree = data.getChildWithName(Ids::parameters);
    ValueTree pa1 = paramsTree.getChild(0);
    ValueTree pa2 = paramsTree.getChild(1);
    ValueTree pa3 = paramsTree.getChild(2);
    ValueTree pa4 = paramsTree.getChild(3);

    ValueTree pa5, pa6;

    pa1.setProperty(Ids::value,
                    Utils::fixParameterValueIfNeeded(teStiff.getText()),
                    undoManager);
    pa2.setProperty(Ids::value,
                    Utils::fixParameterValueIfNeeded(teDamp.getText()),
                    undoManager);
    if (data.getType() == Ids::pluck)
    {
        pa3.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(teMinDisplace.getText()),
                        undoManager);
        pa4.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(tePos.getText()),
                        undoManager);
    }
    else if (data.getType() == Ids::pulsetouch)
    {
        pa3.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(tePos.getText()),
                        undoManager);
        pa4.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(tePulseMult.getText()),
                        undoManager);
        pa5 = paramsTree.getChild(4);
        pa5.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(tePulseTau.getText()),
                        undoManager);
        pa6 = paramsTree.getChild(5);
        pa6.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(tePulseLen.getText()),
                        undoManager);
    }
    else
    {
        pa3.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(tePos.getText()),
                        undoManager);
    }
    return true;
}

AudiooutPropertiesComponent::AudiooutPropertiesComponent(ObjController* objController_,
                            ValueTree data_,
                            UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, data_, undoManager_),
laSource("laSource", "Source"),
teSource("teSource"),
laOpt("laOpt", "Optional"),
teOpt("teopt")
{
    teSource.setReadOnly(true);
    teSource.addListener(this);
    teSource.setTextToShowWhenEmpty("0.0", Colours::darkgrey);
    teSource.setColour(TextEditor::textColourId, Colours::darkgrey);
    addAndMakeVisible(&teSource);
    laSource.attachToComponent(&teSource, true);
    teOpt.addListener(this);
    addAndMakeVisible(&teOpt);
    laOpt.attachToComponent(&teOpt, true);

    readValues();
}

AudiooutPropertiesComponent::~AudiooutPropertiesComponent()
{

}

void AudiooutPropertiesComponent::resized()
{
    ObjectPropertiesComponent::resized();

    teSource.setBounds(100, 40, getWidth() - 110, 22);
    teOpt.setBounds(100, 70, getWidth() - 110, 22);
}

void AudiooutPropertiesComponent::readValues()
{
    teName.setText(data[Ids::identifier].toString());
    String sourceText;
    ValueTree sourcesTree = data.getChildWithName(Ids::sources);
    for (int i = 0; i < sourcesTree.getNumChildren(); ++i)
    {
        ValueTree source = sourcesTree.getChild(i);
        sourceText << source[Ids::value].toString();
        if (i != sourcesTree.getNumChildren() - 1)
            sourceText << "+";
    }
    teSource.setText(sourceText);
    teOpt.setText(data.getProperty(Ids::optional).toString());
}

bool AudiooutPropertiesComponent::writeValues()
{
    if(! writeIdentifier())
        return false;
    
    String sourceText = teSource.getText();
    StringArray sourcesList;
    sourcesList.addTokens(sourceText, "+", "\"");
    ValueTree sourcesTree = data.getChildWithName(Ids::sources);
    sourcesTree.removeAllChildren(undoManager);
    for (int i = 0; i < sourcesList.size(); ++i)
    {
        ValueTree source(Ids::audiosource);
        source.setProperty(Ids::value, sourcesList[i], undoManager);
        sourcesTree.addChild(source, -1, undoManager);
    }
    data.setProperty(Ids::optional, teOpt.getText(), undoManager);

    return true;
}

WaveguidePropertiesComponent::WaveguidePropertiesComponent(ObjController* objController_,
                             ValueTree data_,
                             UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, data_, undoManager_),
laWaveImp("laWaveImp", "Wave impedance (N/(m/s))"),
teWaveImp("teWaveImp"),
laStringType("laStringType", "String type"),
teStringType("teStringType"),
laLeftObj("laLeftObj", "Left object"),
teLeftObj("teLeftObj"),
laRightObj("laRightObj", "Right object"),
teRightObj("teRightObj")
{
    teWaveImp.addListener(this);
    addAndMakeVisible(&teWaveImp);
    laWaveImp.attachToComponent(&teWaveImp, true);
    teStringType.addListener(this);
    addAndMakeVisible(&teStringType);
    laStringType.attachToComponent(&teStringType, true);
    teLeftObj.setReadOnly(true);
    teLeftObj.addListener(this);
    teLeftObj.setColour(TextEditor::textColourId, Colours::darkgrey);
    addAndMakeVisible(&teLeftObj);
    laLeftObj.attachToComponent(&teLeftObj, true);
    teRightObj.setReadOnly(true);
    teRightObj.addListener(this);
    teRightObj.setColour(TextEditor::textColourId, Colours::darkgrey);
    addAndMakeVisible(&teRightObj);
    laRightObj.attachToComponent(&teRightObj, true);
    readValues();
}

WaveguidePropertiesComponent::~WaveguidePropertiesComponent()
{
}

void WaveguidePropertiesComponent::resized()
{
    ObjectPropertiesComponent::resized();

    teWaveImp.setBounds(100, 40, getWidth() - 110, 22);
    teStringType.setBounds(100, 70, getWidth() - 110, 22);
    teLeftObj.setBounds(100, 130, getWidth() - 110, 22);
    teRightObj.setBounds(100, 160, getWidth() - 110, 22);
}

void WaveguidePropertiesComponent::readValues()
{
    teName.setText(data[Ids::identifier].toString());
    teWaveImp.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
    teStringType.setText(data.getChildWithName(Ids::parameters).getChild(1)[Ids::value].toString());
    teLeftObj.setText(data[Ids::startVertex].toString());
    teRightObj.setText(data[Ids::endVertex].toString());
}

bool WaveguidePropertiesComponent::writeValues()
{
    if(! writeIdentifier())
        return false;

    ValueTree paramsTree = data.getChildWithName(Ids::parameters);
    ValueTree pa1 = paramsTree.getChild(0);
    ValueTree pa2 = paramsTree.getChild(1);

    pa1.setProperty(Ids::value,
                    Utils::fixParameterValueIfNeeded(teWaveImp.getText()),
                    undoManager);
    pa2.setProperty(Ids::value, teStringType.getText(), undoManager);

    return true;
}

TerminationPropertiesComponent::TerminationPropertiesComponent(ObjController* objController_,
                               ValueTree data_,
                               UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, data_, undoManager_),
laTermType("laTermType", "Type"),
teTermType("teTermType")
{
    teTermType.addListener(this);
    addAndMakeVisible(&teTermType);
    laTermType.attachToComponent(&teTermType, true);

    readValues();
}

TerminationPropertiesComponent::~TerminationPropertiesComponent()
{

}

void TerminationPropertiesComponent::resized()
{
    ObjectPropertiesComponent::resized();

    teTermType.setBounds(100, 40, getWidth() - 110, 22);
}

void TerminationPropertiesComponent::readValues()
{
    teName.setText(data[Ids::identifier].toString());
    teTermType.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
}

bool TerminationPropertiesComponent::writeValues()
{
    if(! writeIdentifier())
        return false;

    ValueTree paramsTree = data.getChildWithName(Ids::parameters);
    ValueTree param = paramsTree.getChild(0);
    param.setProperty(Ids::value, teTermType.getText(), undoManager);
    return true;
}

JunctionPropertiesComponent::JunctionPropertiesComponent(ObjController* objController_,
                            ValueTree data_,
                            UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, data_, undoManager_),
laPos("laPos", "Displacement (m)"),
tePos("tePos")
{
    tePos.addListener(this);
    addAndMakeVisible(&tePos);
    laPos.attachToComponent(&tePos, true);

    readValues();
}

JunctionPropertiesComponent::~JunctionPropertiesComponent()
{

}

void JunctionPropertiesComponent::resized()
{
    ObjectPropertiesComponent::resized();

    tePos.setBounds(100, 40, getWidth() - 110, 22);
}

void JunctionPropertiesComponent::readValues()
{
    teName.setText(data[Ids::identifier].toString());
    tePos.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
}

bool JunctionPropertiesComponent::writeValues()
{
    if(! writeIdentifier())
        return true;
    
    ValueTree paramsTree = data.getChildWithName(Ids::parameters);
    ValueTree param = paramsTree.getChild(0);
    param.setProperty(Ids::value, tePos.getText(), undoManager);
    return true;
}
