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

const char* multipleSelectionText = "(multiple selected)";

ObjectPropertiesComponent::ObjectPropertiesComponent(ObjController* objController_,
                                                     Array<ValueTree> datas_,
                                                     UndoManager* undoManager_)
:
objController(objController_),
datas(datas_),
undoManager(undoManager_),
laName(nullptr),
teName(nullptr),
//laDebug(nullptr),
dataChanged(false),
isEditing(false)
{
    laName = new Label("laName", "Identifier");
    teName = new TextEditor("teName");
    addAndMakeVisible(teName);
    teName->addListener(this);
    laName->attachToComponent(teName, true);
    multipleEdit = datas.size() > 1;
//    addAndMakeVisible(&laDebug);
}

ObjectPropertiesComponent::~ObjectPropertiesComponent()
{
    deleteAllChildren();
}

void ObjectPropertiesComponent::resized()
{
    teName->setBounds(100, 10, getWidth() - 110, 22);
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
    applyEditing();
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
    String newName = teName->getText();
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

        teName->setReadOnly(true);
        teName->setTextToShowWhenEmpty(multipleSelectionText, Colours::darkgrey);
        teName->setText(String::empty);

        propertiesWindow->setName("Properties: " + String(multipleSelectionText));
    }
    else
    {
//        laDebug.setText("Pos: " + datas[0][Ids::posX].toString() + String(" ") +
//                        datas[0][Ids::posY].toString(), dontSendNotification);

        teName->setReadOnly(false);
        teName->setTextToShowWhenEmpty(String::empty, Colours::black);
        teName->setText(datas[0][Ids::identifier].toString());

        if(datas[0].getType() == Ids::mass
            || datas[0].getType() == Ids::port
            || datas[0].getType() == Ids::resonators
            || datas[0].getType() == Ids::ground
            || datas[0].getType() == Ids::termination
            || datas[0].getType() == Ids::junction
            || datas[0].getType() == Ids::audioout
            || datas[0].getType() == Ids::comment)
        {
            propertiesWindow->setName("Properties: " + teName->getText() + " ("
                                      + datas[0][Ids::posX].toString() + String(", ")
                                      + datas[0][Ids::posY].toString() + ")");
        }
        else
        {
            propertiesWindow->setName("Properties: " + teName->getText());
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

TextEditor* ObjectPropertiesComponent::createEditor(const String& name,
                                                    const String& text,
                                                    bool isMultiline,
                                                    bool isReadOnly)
{
    TextEditor* te = new TextEditor("te" + name);
    Label* la = new Label("la" + name, text);

    te->setReadOnly(isReadOnly);
    if(isReadOnly)
        te->setColour(TextEditor::textColourId, Colours::darkgrey);

    te->addListener(this);
    if(isMultiline)
        te->setMultiLine(true,true);
    addAndMakeVisible(te);
    la->attachToComponent(te, true);

    return te;
}

void ObjectPropertiesComponent::readEditorsMultipleSelection(Array<TextEditor*>& editors_,
                                                             Colour c)
{
    for (int i = 0; i < editors_.size(); ++i)
    {
        TextEditor* editor = editors_[i];
        editor->setText(String::empty);
        editor->setTextToShowWhenEmpty(multipleSelectionText, c);
    }

}

void ObjectPropertiesComponent::readEditorsSingleSelection(Array<TextEditor*>& editors_,
                                                           StringArray params,
                                                           Colour c)
{
    for (int i = 0; i < editors_.size(); ++i)
    {
        TextEditor* editor = editors_[i];

        editor->setText(params[i]);
        editor->setTextToShowWhenEmpty(String::empty, c);
    }
}

void ObjectPropertiesComponent::writeEditors(Array<TextEditor*> editors_,
                                             ValueTree params_,
                                             bool fixValues)
{
    for (int i = 0; i < editors_.size(); ++i)
    {
        TextEditor* editor = editors_[i];

        ValueTree pa = params_.getChild(i);

        String val = editor->getText();
        if(fixValues)
            val = Utils::fixParameterValueIfNeeded(val);

        pa.setProperty(Ids::value, val, undoManager);
    }
}

StringArray ObjectPropertiesComponent::getParamsStrings(int numParams,
                                                        ValueTree params)
{
    StringArray res;
    for (int i = 0; i < numParams; ++i)
    {
        res.add(params.getChild(i)[Ids::value].toString());
    }
    return res;
}

MassPropertiesComponent::MassPropertiesComponent(ObjController* objController_,
                                                 Array<ValueTree> datas_,
                                                 UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_)
{
    editors.add(createEditor("Mass", "Mass (kg)", true));
    editors.add(createEditor("Pos", "Position (m)", true));
    editors.add(createEditor("Vel", "Valocity (m/s)", true));

    readValues();
}

MassPropertiesComponent::~MassPropertiesComponent()
{
//    deleteAllChildren();
}

void MassPropertiesComponent::resized()
{
    ObjectPropertiesComponent::resized();

    editors[0]->setBounds(100, 40, getWidth() - 110, 50);
    editors[1]->setBounds(100, 100, getWidth() - 110, 50);
    editors[2]->setBounds(100, 160, getWidth() - 110, 50);
}

void MassPropertiesComponent::readValues()
{
    if (isEditing)
        return;

    ObjectPropertiesComponent::readValues();

    if(multipleEdit)
    {
        readEditorsMultipleSelection(editors);
    }
    else
    {
        ValueTree data = datas[0];

        StringArray params = getParamsStrings(editors.size(),
                                              data.getChildWithName(Ids::parameters));
        readEditorsSingleSelection(editors, params);
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
        
        writeEditors(editors, paramsTree, true);
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
//    deleteAllChildren();
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
: ObjectPropertiesComponent(objController_, datas_, undoManager_)
{
    editors.add(createEditor("Freq", "Frequency (Hz)", true));
    editors.add(createEditor("Decay", "Decay (sec)", true));
    editors.add(createEditor("EqMass", "Mass (kg)", true));

    readValues();
}

ResonatorPropertiesComponent::~ResonatorPropertiesComponent()
{
//    deleteAllChildren();
}

void ResonatorPropertiesComponent::resized()
{
    ObjectPropertiesComponent::resized();

    editors[0]->setBounds(100, 40, getWidth() - 110, 50);
    editors[1]->setBounds(100, 100, getWidth() - 110, 50);
    editors[2]->setBounds(100, 160, getWidth() - 110, 50);
}

void ResonatorPropertiesComponent::readValues()
{
    if (isEditing)
        return;

    ObjectPropertiesComponent::readValues();

   if(multipleEdit)
    {
        readEditorsMultipleSelection(editors);
    }
    else
    {
        ValueTree paramsTree = datas[0].getChildWithName(Ids::parameters);

        String valFreq, valDecay, valEqMass;

        int numRes = paramsTree.getNumChildren() / 3;
        jassert(numRes != 0);
        for (int i = 0; i < numRes; ++i)
        {
            String delim = ",";
            if (i == 0)
                delim = "";
            valFreq << delim << paramsTree.getChild(i * 3 + 0)[Ids::value].toString();
            valDecay << delim << paramsTree.getChild(i * 3 + 1)[Ids::value].toString();
            valEqMass << delim << paramsTree.getChild(i * 3 + 2)[Ids::value].toString();
        }

        StringArray params;
        params.add(valFreq);
        params.add(valDecay);
        params.add(valEqMass);
        readEditorsSingleSelection(editors, params);
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

        String valFreq = editors[0]->getText();
        String valDecay = editors[1]->getText();
        String valEqMass = editors[2]->getText();

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
: ObjectPropertiesComponent(objController_, datas_, undoManager_)
{
    editors.add(createEditor("Pos", "Position (m)", true));

    readValues();
}

GroundPropertiesComponent::~GroundPropertiesComponent()
{
//    deleteAllChildren();
}

void GroundPropertiesComponent::resized()
{
    ObjectPropertiesComponent::resized();

    editors[0]->setBounds(100, 40, getWidth() - 110, 50);
}

void GroundPropertiesComponent::readValues()
{
    if (isEditing)
        return;

    ObjectPropertiesComponent::readValues();

    if(multipleEdit)
    {
        readEditorsMultipleSelection(editors);
    }
    else
    {
        StringArray params = getParamsStrings(editors.size(),
                                              datas[0].getChildWithName(Ids::parameters));
        readEditorsSingleSelection(editors,params);
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
        param.setProperty(Ids::value, editors[0]->getText(), undoManager);
    }

    return true;
}

LinkPropertiesComponent::LinkPropertiesComponent(ObjController* objController_,
                                                 Array<ValueTree> datas_,
                                                 UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_)
{
    editors.add(createEditor("Stiff", "Stiffness (N/m)", true));
    editors.add(createEditor("Damp", "Damping (N/(m/s))", true));
    if(datas[0].getType() == Ids::pluck)
    {
        editors.add(createEditor("MinDisplace", "Minimum displace diff", true));
//        editors.add(createRow("Pos", "Position offste (m)", true));
    }
    editors.add(createEditor("Pos", "Position offste (m)", true));
    if(datas[0].getType() == Ids::pulsetouch)
    {
//        editors.add(createRow("Pos", "Position offste (m)", true));
        editors.add(createEditor("PulseMult", "Pulse multiplier", true));
        editors.add(createEditor("PulseTau", "Pulse tau", true));
        editors.add(createEditor("PulseLen", "Pulse length", true));
    }

    vertices.add(createEditor("StartVertex", "Start vertex", false, true));
    vertices.add(createEditor("EndVertex", "End vertex", false, true));
    
    readValues();
}

LinkPropertiesComponent::~LinkPropertiesComponent()
{
//    deleteAllChildren();
}

void LinkPropertiesComponent::resized()
{
    ObjectPropertiesComponent::resized();
    int offset = 0;
    editors[0]->setBounds(100, 40, getWidth() - 110, 50);
    editors[1]->setBounds(100, 100, getWidth() - 110, 50);
    int idOffset = 0;
    if (datas[0].getType() == Ids::pluck)
    {
        editors[2]->setBounds(100, 160, getWidth() - 110, 50);
        offset = 60;
        ++idOffset;
    }
    else if (datas[0].getType() == Ids::pulsetouch)
    {
        editors[3]->setBounds(100, 160, getWidth() - 110, 50);
        editors[4]->setBounds(100, 220, getWidth() - 110, 50);
        editors[5]->setBounds(100, 280, getWidth() - 110, 50);
        offset = 180;
    }
    editors[2+idOffset]->setBounds(100, 160 + offset, getWidth() - 110, 50);
    vertices[0]->setBounds(100, 220 + offset, getWidth() - 110, 22);
    vertices[1]->setBounds(100, 250 + offset, getWidth() - 110, 22);
}

void LinkPropertiesComponent::readValues()
{
    if (isEditing)
        return;

    ObjectPropertiesComponent::readValues();

    if(multipleEdit)
    {
        readEditorsMultipleSelection(editors);
        readEditorsMultipleSelection(vertices, Colours::darkgrey);
    }
    else
    {
        ValueTree data = datas[0];

        StringArray params = getParamsStrings(editors.size(),
                                              data.getChildWithName(Ids::parameters));
        readEditorsSingleSelection(editors, params);

        StringArray vertexVals;
        vertexVals.add(data[Ids::startVertex].toString());
        vertexVals.add(data[Ids::endVertex].toString());
        readEditorsSingleSelection(vertices, vertexVals, Colours::darkgrey);
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

        writeEditors(editors, paramsTree, true);
    }

    return true;
}

AudiooutPropertiesComponent::AudiooutPropertiesComponent(ObjController* objController_,
                                                         Array<ValueTree> datas_,
                                                         UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_)
{
    editors.add(createEditor("Source", "Source", true, true));
    editors[0]->setTextToShowWhenEmpty("0.0", Colours::darkgrey);

    editors.add(createEditor("Opt", "Optional", true));

    readValues();
}

AudiooutPropertiesComponent::~AudiooutPropertiesComponent()
{
//    deleteAllChildren();
}

void AudiooutPropertiesComponent::resized()
{
    ObjectPropertiesComponent::resized();

    editors[0]->setBounds(100, 40, getWidth() - 110, 50);
    editors[1]->setBounds(100, 100, getWidth() - 110, 50);
}

void AudiooutPropertiesComponent::readValues()
{
    if (isEditing)
        return;

    ObjectPropertiesComponent::readValues();

    if(multipleEdit)
    {
        readEditorsMultipleSelection(editors);
        editors[0]->setTextToShowWhenEmpty(multipleSelectionText, Colours::darkgrey);
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

        StringArray vals;
        vals.add(sourceText);
        vals.add(data.getProperty(Ids::optional).toString());
        readEditorsSingleSelection(editors, vals);
        editors[0]->setTextToShowWhenEmpty("0.0", Colours::darkgrey);
    }
}

bool AudiooutPropertiesComponent::writeValues()
{
    if(! ObjectPropertiesComponent::writeValues())
        return false;

    for (int i = 0; i < datas.size(); ++i)
    {
        ValueTree data = datas[i];

        String sourceText = editors[0]->getText();
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
        data.setProperty(Ids::optional, editors[1]->getText(), undoManager);
    }

    return true;
}

WaveguidePropertiesComponent::WaveguidePropertiesComponent(ObjController* objController_,
                                                           Array<ValueTree> datas_,
                                                           UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_)
{
    editors.add(createEditor("WaveImp", "Wave impedance (N/(m/s))", true));
    editors.add(createEditor("StringType", "String Type", true));

    vertices.add(createEditor("LeftObj", "Left object", false, true));
    vertices.add(createEditor("RightObj", "Right object", false, true));

    readValues();
}

WaveguidePropertiesComponent::~WaveguidePropertiesComponent()
{
//    deleteAllChildren();
}

void WaveguidePropertiesComponent::resized()
{
    ObjectPropertiesComponent::resized();

    editors[0]->setBounds(100, 40, getWidth() - 110, 50);
    editors[1]->setBounds(100, 100, getWidth() - 110, 50);
    vertices[0]->setBounds(100, 160, getWidth() - 110, 22);
    vertices[1]->setBounds(100, 190, getWidth() - 110, 22);
}

void WaveguidePropertiesComponent::readValues()
{
    if (isEditing)
        return;

    ObjectPropertiesComponent::readValues();

    if(multipleEdit)
    {
        readEditorsMultipleSelection(editors);
        readEditorsMultipleSelection(vertices, Colours::darkgrey);
    }
    else
    {
        ValueTree data = datas[0];

        StringArray params = getParamsStrings(editors.size(),
                                              data.getChildWithName(Ids::parameters));
        readEditorsSingleSelection(editors, params);

        StringArray vertexVals;
        vertexVals.add(data[Ids::startVertex].toString());
        vertexVals.add(data[Ids::endVertex].toString());
        readEditorsSingleSelection(vertices, vertexVals, Colours::darkgrey);
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
                        Utils::fixParameterValueIfNeeded(editors[0]->getText()),
                        undoManager);
        pa2.setProperty(Ids::value, editors[1]->getText(), undoManager);
    }

    return true;
}

TerminationPropertiesComponent::TerminationPropertiesComponent(ObjController* objController_,
                                                               Array<ValueTree> datas_,
                                                               UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_)
{
    editors.add(createEditor("TermType", "Type", true));

    readValues();
}

TerminationPropertiesComponent::~TerminationPropertiesComponent()
{
//    deleteAllChildren();
}

void TerminationPropertiesComponent::resized()
{
    ObjectPropertiesComponent::resized();

    editors[0]->setBounds(100, 40, getWidth() - 110, 50);
}

void TerminationPropertiesComponent::readValues()
{
    if (isEditing)
        return;

    ObjectPropertiesComponent::readValues();

    if(multipleEdit)
    {
        readEditorsMultipleSelection(editors);
    }
    else
    {
        StringArray params = getParamsStrings(editors.size(),
                                              datas[0].getChildWithName(Ids::parameters));
        readEditorsSingleSelection(editors, params);
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

        writeEditors(editors, paramsTree, false);
    }

    return true;
}

JunctionPropertiesComponent::JunctionPropertiesComponent(ObjController* objController_,
                                                         Array<ValueTree> datas_,
                                                         UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_)
{
    editors.add(createEditor("Pos", "Displacement (m)", true));

    readValues();
}

JunctionPropertiesComponent::~JunctionPropertiesComponent()
{
//    deleteAllChildren();
}

void JunctionPropertiesComponent::resized()
{
    ObjectPropertiesComponent::resized();

    editors[0]->setBounds(100, 40, getWidth() - 110, 50);
}

void JunctionPropertiesComponent::readValues()
{
    if (isEditing)
        return;

    ObjectPropertiesComponent::readValues();

    if(multipleEdit)
    {
        readEditorsMultipleSelection(editors);
    }
    else
    {
        StringArray params = getParamsStrings(editors.size(),
                                              datas[0].getChildWithName(Ids::parameters));
        readEditorsSingleSelection(editors, params);
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

        writeEditors(editors, paramsTree, false);
    }

    return true;
}

GainComponent::GainComponent(StringArray sourceIds_,
                             Array<ValueTree> datas_,
                             UndoManager* undoManager_)
: labelGain("Gain", "Gain"),
teGain("Gain value"),
sourceIds(sourceIds_),
datas(datas_),
undoManager(undoManager_),
dataChanged(false)
{
    teGain.setText("");
    teGain.addListener(this);
    addAndMakeVisible(&teGain);
    labelGain.attachToComponent(&teGain, true);

    multipleEdit = datas.size() > 1;

    readValues();
}

GainComponent::~GainComponent()
{
}

void GainComponent::resized()
{
    labelGain.setBounds(0, 5, 60, 22);
    teGain.setBounds(60, 5, getWidth() - 65, 22);
}

void GainComponent::textEditorTextChanged(TextEditor& /*editor*/)
{
    dataChanged = true;
}

void GainComponent::textEditorReturnKeyPressed(TextEditor& /*editor*/)
{
    applyEditing();
}

void GainComponent::textEditorEscapeKeyPressed(TextEditor& /*editor*/)
{
    if(PropertiesWindow* pw = dynamic_cast <PropertiesWindow*> (getParentComponent()))
    {
        pw->closeButtonPressed();
    }
}

void GainComponent::textEditorFocusLost(TextEditor& /*editor*/)
{
    applyEditing();
}

void GainComponent::readValues()
{
    if(multipleEdit)
    {
        teGain.setText(String::empty);
        teGain.setTextToShowWhenEmpty(multipleSelectionText, Colours::black);

        propertiesWindow->setName("Properties: " + String(multipleSelectionText));
    }
    else
    {
        ValueTree sources = datas[0].getChildWithName(Ids::sources);

        String oldGain = Utils::getGainForSourceId(sources, sourceIds[0]);

        teGain.setText(oldGain);

        propertiesWindow->setName("Properties: " + sourceIds[0]);
    }

}
void GainComponent::applyEditing()
{
    if(dataChanged)
    {
        String newGain = teGain.getText();
        undoManager->beginNewTransaction("Edit gain");
        for (int i = 0; i < datas.size(); ++i)
        {
            ValueTree sources = datas[i].getChildWithName(Ids::sources);
            Utils::setGainForSourceId(sources, sourceIds[i], newGain, undoManager);
        }
        undoManager->beginNewTransaction();
        dataChanged = false;
    }
}