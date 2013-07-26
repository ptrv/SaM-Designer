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

const char* multipleSelectionText = "(multiple selection)";

ObjectPropertiesComponent::ObjectPropertiesComponent(ObjController* objController_,
                                                     Array<ValueTree> datas_,
                                                     UndoManager* undoManager_)
:
objController(objController_),
datas(datas_),
undoManager(undoManager_),
laName("laName", "Identifier"),
teName("teName"),
//laDebug(""),
dataChanged(false),
isEditing(false)
{
    addAndMakeVisible(&teName);
    teName.addListener(this);
    laName.attachToComponent(&teName, true);
    multipleEdit = datas.size() > 1;
//    addAndMakeVisible(&laDebug);
}

ObjectPropertiesComponent::~ObjectPropertiesComponent()
{
}

void ObjectPropertiesComponent::resized()
{
    teName.setBounds(100, 10, getWidth() - 110, 22);
//    laDebug.setBounds(65, getHeight() - 60, 200, 22);
}

void ObjectPropertiesComponent::textEditorTextChanged(TextEditor& /*editor*/)
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
        isEditing = true;
        bool writeOk = writeValues();
        isEditing = false;
        undoManager->beginNewTransaction();
        if (writeOk)
        {
            for (int i = 0; i < datas.size(); ++i)
            {
                ValueTree data = datas[i];

                SAM_LOG("Change " + data.getType().toString() + " " +
                        data[Ids::identifier].toString());
            }
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
    for (int i = 0; i < datas.size(); ++i)
    {
        ValueTree data = datas[i];

        SAM_LOG("Cancel change " + data.getType().toString() + " " +
                data[Ids::identifier].toString());
    }
}

bool ObjectPropertiesComponent::writeIdentifier()
{
    ValueTree data = datas[0];
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

void ObjectPropertiesComponent::readValues()
{
    if(multipleEdit)
    {
//        laDebug.setText("Pos: " + String(multipleSelectionText),
//                        dontSendNotification);

        teName.setReadOnly(true);
        teName.setTextToShowWhenEmpty(multipleSelectionText, Colours::darkgrey);
        teName.setText(String::empty);

        propertiesWindow->setName("Properties: " + String(multipleSelectionText));
    }
    else
    {
//        laDebug.setText("Pos: " + datas[0][Ids::posX].toString() + String(" ") +
//                        datas[0][Ids::posY].toString(), dontSendNotification);

        teName.setReadOnly(false);
        teName.setTextToShowWhenEmpty(String::empty, Colours::black);
        teName.setText(datas[0][Ids::identifier].toString());

        if(datas[0].getType() == Ids::mass
            || datas[0].getType() == Ids::port
            || datas[0].getType() == Ids::resonators
            || datas[0].getType() == Ids::ground
            || datas[0].getType() == Ids::termination
            || datas[0].getType() == Ids::junction
            || datas[0].getType() == Ids::audioout
            || datas[0].getType() == Ids::comment)
        {
            propertiesWindow->setName("Properties: " + teName.getText() + " ("
                                      + datas[0][Ids::posX].toString() + String(", ")
                                      + datas[0][Ids::posY].toString() + ")");
        }
        else
        {
            propertiesWindow->setName("Properties: " + teName.getText());
        }
    }
}

bool ObjectPropertiesComponent::writeValues()
{
    DBG("write in base");

    if(! multipleEdit)
        if(! writeIdentifier())
            return false;

    return true;
}

MassPropertiesComponent::MassPropertiesComponent(ObjController* objController_,
                                                 Array<ValueTree> datas_,
                                                 UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_),
laMass("laMass", "Mass (kg)"),
teMass("teMass"),
laPos("laPos", "Position (m)"),
tePos("tePos"),
laVel("laVel", "Velocity (m/s)"),
teVel("teVel")
{
    teMass.addListener(this);
    teMass.setMultiLine(true,true);
    addAndMakeVisible(&teMass);
    laMass.attachToComponent(&teMass, true);
    tePos.addListener(this);
    tePos.setMultiLine(true,true);
    addAndMakeVisible(&tePos);
    laPos.attachToComponent(&tePos, true);
    teVel.addListener(this);
    teVel.setMultiLine(true,true);
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

    teMass.setBounds(100, 40, getWidth() - 110, 50);
    tePos.setBounds(100, 100, getWidth() - 110, 50);
    teVel.setBounds(100, 160, getWidth() - 110, 50);
}

void MassPropertiesComponent::readValues()
{
    if (isEditing)
        return;

    ObjectPropertiesComponent::readValues();

    if(multipleEdit)
    {
        teMass.setText(String::empty);
        tePos.setText(String::empty);
        teVel.setText(String::empty);
        teMass.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
        tePos.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
        teVel.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
    }
    else
    {
        ValueTree data = datas[0];
        teMass.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
        tePos.setText(data.getChildWithName(Ids::parameters).getChild(1)[Ids::value].toString());
        teVel.setText(data.getChildWithName(Ids::parameters).getChild(2)[Ids::value].toString());
        teMass.setTextToShowWhenEmpty(String::empty, Colours::black);
        tePos.setTextToShowWhenEmpty(String::empty, Colours::black);
        teVel.setTextToShowWhenEmpty(String::empty, Colours::black);
    }
}

bool MassPropertiesComponent::writeValues()
{
    if(! ObjectPropertiesComponent::writeValues())
        return false;

    for (int i = 0; i < datas.size(); ++i)
    {
        ValueTree data = datas[i];

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
    }

    return true;
}

PortPropertiesComponent::PortPropertiesComponent(ObjController* objController_,
                                                 Array<ValueTree> datas_,
                                                 UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_)
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
    if (isEditing)
        return;

    ObjectPropertiesComponent::readValues();
}

bool PortPropertiesComponent::writeValues()
{
    if(! ObjectPropertiesComponent::writeValues())
        return false;
    
    return true;
}

ResonatorPropertiesComponent::ResonatorPropertiesComponent(ObjController* objController_,
                                                           Array<ValueTree> datas_,
                                                           UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_),
laFreq("laFreq", "Frequency (Hz)"),
teFreq("teFreq"),
laDecay("laDecay", "Decay (sec)"),
teDecay("teDecay"),
laEqMass("laEqMass", "Mass (kg)"),
teEqMass("teEqMass")
{
    teFreq.addListener(this);
    teFreq.setMultiLine(true, true);
    addAndMakeVisible(&teFreq);
    laFreq.attachToComponent(&teFreq, true);
    teDecay.addListener(this);
    teDecay.setMultiLine(true, true);
    addAndMakeVisible(&teDecay);
    laDecay.attachToComponent(&teDecay, true);
    teEqMass.addListener(this);
    teEqMass.setMultiLine(true, true);
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

    teFreq.setBounds(100, 40, getWidth() - 110, 50);
    teDecay.setBounds(100, 100, getWidth() - 110, 50);
    teEqMass.setBounds(100, 160, getWidth() - 110, 50);
}

void ResonatorPropertiesComponent::readValues()
{
    if (isEditing)
        return;

    ObjectPropertiesComponent::readValues();

   if(multipleEdit)
    {
        teFreq.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
        teDecay.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
        teEqMass.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
        teFreq.setText(String::empty);
        teDecay.setText(String::empty);
        teEqMass.setText(String::empty);
    }
    else
    {
        ValueTree data = datas[0];

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
        teFreq.setTextToShowWhenEmpty(String::empty, Colours::black);
        teDecay.setTextToShowWhenEmpty(String::empty, Colours::black);
        teEqMass.setTextToShowWhenEmpty(String::empty, Colours::black);
    }
}

bool ResonatorPropertiesComponent::writeValues()
{
    if(! ObjectPropertiesComponent::writeValues())
        return false;

    for (int i = 0; i < datas.size(); ++i)
    {
        ValueTree data = datas[i];

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
    }

    return true;
}

GroundPropertiesComponent::GroundPropertiesComponent(ObjController* objController_,
                                                     Array<ValueTree> datas_,
                                                     UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_),
laPos("laPos", "Position (m)"),
tePos("tePos")
{
    tePos.addListener(this);
    tePos.setMultiLine(true, true);
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

    tePos.setBounds(100, 40, getWidth() - 110, 50);
}

void GroundPropertiesComponent::readValues()
{
    if (isEditing)
        return;

    ObjectPropertiesComponent::readValues();

    if(multipleEdit)
    {
        tePos.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
        tePos.setText(String::empty);
    }
    else
    {
        ValueTree data = datas[0];
        tePos.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
        tePos.setTextToShowWhenEmpty(String::empty, Colours::black);
    }
}

bool GroundPropertiesComponent::writeValues()
{
    if(! ObjectPropertiesComponent::writeValues())
        return false;

    for (int i = 0; i < datas.size(); ++i)
    {
        ValueTree data = datas[i];

        ValueTree paramsTree = data.getChildWithName(Ids::parameters);
        ValueTree param = paramsTree.getChild(0);
        param.setProperty(Ids::value, tePos.getText(), undoManager);
    }

    return true;
}

LinkPropertiesComponent::LinkPropertiesComponent(ObjController* objController_,
                                                 Array<ValueTree> datas_,
                                                 UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_),
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
    teStiff.setMultiLine(true, true);
    addAndMakeVisible(&teStiff);
    laStiff.attachToComponent(&teStiff, true);
    teDamp.addListener(this);
    teDamp.setMultiLine(true, true);
    addAndMakeVisible(&teDamp);
    laDamp.attachToComponent(&teDamp, true);
    tePos.addListener(this);
    tePos.setMultiLine(true, true);
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
    if (datas[0].getType() == Ids::pluck)
    {
        teMinDisplace.addListener(this);
        teMinDisplace.setMultiLine(true, true);
        addAndMakeVisible(&teMinDisplace);
        laMinDisplace.attachToComponent(&teMinDisplace, true);
    }
    else if (datas[0].getType() == Ids::pulsetouch)
    {
        tePulseMult.addListener(this);
        tePulseMult.setMultiLine(true,true);
        addAndMakeVisible(&tePulseMult);
        laPulseMult.attachToComponent(&tePulseMult, true);
        tePulseTau.addListener(this);
        tePulseTau.setMultiLine(true, true);
        addAndMakeVisible(&tePulseTau);
        laPulseTau.attachToComponent(&tePulseTau, true);
        tePulseLen.addListener(this);
        tePulseLen.setMultiLine(true, true);
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
    teStiff.setBounds(100, 40, getWidth() - 110, 50);
    teDamp.setBounds(100, 100, getWidth() - 110, 50);
    if (datas[0].getType() == Ids::pluck)
    {
        teMinDisplace.setBounds(100, 160, getWidth() - 110, 50);
        offset = 60;
    }
    else if (datas[0].getType() == Ids::pulsetouch)
    {
        tePulseMult.setBounds(100, 160, getWidth() - 110, 50);
        tePulseTau.setBounds(100, 220, getWidth() - 110, 50);
        tePulseLen.setBounds(100, 280, getWidth() - 110, 50);
        offset = 180;
    }
    tePos.setBounds(100, 160 + offset, getWidth() - 110, 50);
    teStartVertex.setBounds(100, 220 + offset, getWidth() - 110, 22);
    teEndVertex.setBounds(100, 250 + offset, getWidth() - 110, 22);
}

void LinkPropertiesComponent::readValues()
{
    if (isEditing)
        return;

    ObjectPropertiesComponent::readValues();

    if(multipleEdit)
    {
        teStiff.setText(String::empty);
        teStiff.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
        teDamp.setText(String::empty);
        teDamp.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
        int offset = 0;
        if (datas[0].getType() == Ids::pluck)
        {
            teMinDisplace.setText(String::empty);
            teMinDisplace.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
            ++offset;
        }
        else if (datas[0].getType() == Ids::pulsetouch)
        {
            tePulseMult.setText(String::empty);
            tePulseMult.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
            tePulseTau.setText(String::empty);
            tePulseTau.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
            tePulseLen.setText(String::empty);
            tePulseLen.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
        }
        tePos.setText(String::empty);
        tePos.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
        teStartVertex.setText(String::empty);
        teStartVertex.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
        teEndVertex.setText(String::empty);
        teEndVertex.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);

    }
    else
    {
        ValueTree data = datas[0];
        teStiff.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
        teStiff.setTextToShowWhenEmpty(String::empty, Colours::black);
        teDamp.setText(data.getChildWithName(Ids::parameters).getChild(1)[Ids::value].toString());
        teDamp.setTextToShowWhenEmpty(String::empty, Colours::black);
        int offset = 0;
        if (data.getType() == Ids::pluck)
        {
            teMinDisplace.setText(data.getChildWithName(Ids::parameters).getChild(2)[Ids::value].toString());
            teMinDisplace.setTextToShowWhenEmpty(String::empty, Colours::black);
            ++offset;
        }
        else if (data.getType() == Ids::pulsetouch)
        {
            tePulseMult.setText(data.getChildWithName(Ids::parameters).getChild(3)[Ids::value].toString());
            tePulseTau.setText(data.getChildWithName(Ids::parameters).getChild(4)[Ids::value].toString());
            tePulseLen.setText(data.getChildWithName(Ids::parameters).getChild(5)[Ids::value].toString());
            tePulseMult.setTextToShowWhenEmpty(String::empty, Colours::black);
            tePulseTau.setTextToShowWhenEmpty(String::empty, Colours::black);
            tePulseLen.setTextToShowWhenEmpty(String::empty, Colours::black);
        }
        tePos.setText(data.getChildWithName(Ids::parameters).getChild(2 + offset)[Ids::value].toString());
        teStartVertex.setText(data[Ids::startVertex].toString());
        teEndVertex.setText(data[Ids::endVertex].toString());
        tePos.setTextToShowWhenEmpty(String::empty, Colours::black);
        teStartVertex.setTextToShowWhenEmpty(String::empty, Colours::black);
        teEndVertex.setTextToShowWhenEmpty(String::empty, Colours::black);
    }
}

bool LinkPropertiesComponent::writeValues()
{
    if(! ObjectPropertiesComponent::writeValues())
        return false;

    for (int i = 0; i < datas.size(); ++i)
    {
        ValueTree data = datas[i];

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
    }

    return true;
}

AudiooutPropertiesComponent::AudiooutPropertiesComponent(ObjController* objController_,
                                                         Array<ValueTree> datas_,
                                                         UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_),
laSource("laSource", "Source"),
teSource("teSource"),
laOpt("laOpt", "Optional"),
teOpt("teopt")
{
    teSource.setReadOnly(true);
    teSource.addListener(this);
    teSource.setTextToShowWhenEmpty("0.0", Colours::darkgrey);
    teSource.setColour(TextEditor::textColourId, Colours::darkgrey);
    teSource.setMultiLine(true,true);
    addAndMakeVisible(&teSource);
    laSource.attachToComponent(&teSource, true);
    teOpt.addListener(this);
    teOpt.setMultiLine(true,true);
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

    teSource.setBounds(100, 40, getWidth() - 110, 50);
    teOpt.setBounds(100, 100, getWidth() - 110, 50);
}

void AudiooutPropertiesComponent::readValues()
{
    if (isEditing)
        return;

    ObjectPropertiesComponent::readValues();

    if(multipleEdit)
    {
        teSource.setText(String::empty);
        teOpt.setText(String::empty);
        teSource.setTextToShowWhenEmpty(multipleSelectionText, Colours::darkgrey);
        teOpt.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
    }
    else
    {
        ValueTree data = datas[0];
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
        teSource.setTextToShowWhenEmpty("0.0", Colours::darkgrey);
        teOpt.setText(data.getProperty(Ids::optional).toString());
        teOpt.setTextToShowWhenEmpty(String::empty, Colours::black);
    }
}

bool AudiooutPropertiesComponent::writeValues()
{
    if(! ObjectPropertiesComponent::writeValues())
        return false;

    for (int i = 0; i < datas.size(); ++i)
    {
        ValueTree data = datas[i];

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
    }

    return true;
}

WaveguidePropertiesComponent::WaveguidePropertiesComponent(ObjController* objController_,
                                                           Array<ValueTree> datas_,
                                                           UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_),
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
    teWaveImp.setMultiLine(true, true);
    addAndMakeVisible(&teWaveImp);
    laWaveImp.attachToComponent(&teWaveImp, true);
    teStringType.addListener(this);
    teStringType.setMultiLine(true, true);
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

    teWaveImp.setBounds(100, 40, getWidth() - 110, 50);
    teStringType.setBounds(100, 100, getWidth() - 110, 50);
    teLeftObj.setBounds(100, 160, getWidth() - 110, 22);
    teRightObj.setBounds(100, 190, getWidth() - 110, 22);
}

void WaveguidePropertiesComponent::readValues()
{
    if (isEditing)
        return;

    ObjectPropertiesComponent::readValues();

    if(multipleEdit)
    {
        teWaveImp.setText(String::empty);
        teStringType.setText(String::empty);
        teLeftObj.setText(String::empty);
        teRightObj.setText(String::empty);
        teName.setTextToShowWhenEmpty(multipleSelectionText, Colours::darkgrey);
        teWaveImp.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
        teStringType.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
        teLeftObj.setTextToShowWhenEmpty(multipleSelectionText, Colours::darkgrey);
        teRightObj.setTextToShowWhenEmpty(multipleSelectionText, Colours::darkgrey);
    }
    else
    {
        ValueTree data = datas[0];
        teWaveImp.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
        teStringType.setText(data.getChildWithName(Ids::parameters).getChild(1)[Ids::value].toString());
        teLeftObj.setText(data[Ids::startVertex].toString());
        teRightObj.setText(data[Ids::endVertex].toString());
        teWaveImp.setTextToShowWhenEmpty(String::empty, Colours::black);
        teStringType.setTextToShowWhenEmpty(String::empty, Colours::black);
        teLeftObj.setTextToShowWhenEmpty(String::empty, Colours::darkgrey);
        teRightObj.setTextToShowWhenEmpty(String::empty, Colours::darkgrey);
    }
}

bool WaveguidePropertiesComponent::writeValues()
{
    if(! ObjectPropertiesComponent::writeValues())
        return false;

    for (int i = 0; i < datas.size(); ++i)
    {
        ValueTree data = datas[i];

        ValueTree paramsTree = data.getChildWithName(Ids::parameters);
        ValueTree pa1 = paramsTree.getChild(0);
        ValueTree pa2 = paramsTree.getChild(1);

        pa1.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(teWaveImp.getText()),
                        undoManager);
        pa2.setProperty(Ids::value, teStringType.getText(), undoManager);
    }

    return true;
}

TerminationPropertiesComponent::TerminationPropertiesComponent(ObjController* objController_,
                                                               Array<ValueTree> datas_,
                                                               UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_),
laTermType("laTermType", "Type"),
teTermType("teTermType")
{
    teTermType.addListener(this);
    teTermType.setMultiLine(true, true);
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

    teTermType.setBounds(100, 40, getWidth() - 110, 50);
}

void TerminationPropertiesComponent::readValues()
{
    if (isEditing)
        return;

    ObjectPropertiesComponent::readValues();

    if(multipleEdit)
    {
        teTermType.setText(String::empty);
        teTermType.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
    }
    else
    {
        ValueTree data = datas[0];
        teTermType.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
        teTermType.setTextToShowWhenEmpty(String::empty, Colours::black);
    }
}

bool TerminationPropertiesComponent::writeValues()
{
    if(! ObjectPropertiesComponent::writeValues())
        return false;

    for (int i = 0; i < datas.size(); ++i)
    {
        ValueTree data = datas[i];

        ValueTree paramsTree = data.getChildWithName(Ids::parameters);
        ValueTree param = paramsTree.getChild(0);
        param.setProperty(Ids::value, teTermType.getText(), undoManager);
    }

    return true;
}

JunctionPropertiesComponent::JunctionPropertiesComponent(ObjController* objController_,
                                                         Array<ValueTree> datas_,
                                                         UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_),
laPos("laPos", "Displacement (m)"),
tePos("tePos")
{
    tePos.addListener(this);
    tePos.setMultiLine(true, true);
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

    tePos.setBounds(100, 40, getWidth() - 110, 50);
}

void JunctionPropertiesComponent::readValues()
{
    if (isEditing)
        return;

    ObjectPropertiesComponent::readValues();

    if(multipleEdit)
    {
        tePos.setText(String::empty);
        tePos.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
    }
    else
    {
        ValueTree data = datas[0];
        tePos.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
        tePos.setTextToShowWhenEmpty(String::empty, Colours::black);
    }
}

bool JunctionPropertiesComponent::writeValues()
{
    if(! ObjectPropertiesComponent::writeValues())
        return false;

    for (int i = 0; i < datas.size(); ++i)
    {
        ValueTree data = datas[i];

        ValueTree paramsTree = data.getChildWithName(Ids::parameters);
        ValueTree param = paramsTree.getChild(0);
        param.setProperty(Ids::value, tePos.getText(), undoManager);
    }

    return true;
}
