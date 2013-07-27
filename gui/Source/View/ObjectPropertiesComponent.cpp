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

TextEditor* ObjectPropertiesComponent::createRow(const String& name,
                                                 const String& text,
                                                 bool isMultiline)
{
    TextEditor* te = new TextEditor("te" + name);
    Label* la = new Label("la" + name, text);

    te->addListener(this);
    if(isMultiline)
        te->setMultiLine(true,true);
    addAndMakeVisible(te);
    la->attachToComponent(te, true);

    return te;
}

void ObjectPropertiesComponent::setRowMultipleSelection()
{
    for (int i = 0; i < editors.size(); ++i)
    {
        TextEditor* editor = editors[i];
        editor->setText(String::empty);
        editor->setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
    }

}

void ObjectPropertiesComponent::readRowsSingleSelection(ValueTree data)
{
    for (int i = 0; i < editors.size(); ++i)
    {
        TextEditor* editor = editors[i];

        editor->setText(data.getChild(i)[Ids::value].toString());
        editor->setTextToShowWhenEmpty(String::empty, Colours::black);
    }
}

void ObjectPropertiesComponent::writeRowsSingleSelection(ValueTree data_,
                                                         bool fixValues)
{
    for (int i = 0; i < editors.size(); ++i)
    {
        TextEditor* editor = editors[i];

        ValueTree pa = data_.getChild(i);

        String val = editor->getText();
        if(fixValues)
            val = Utils::fixParameterValueIfNeeded(val);

        pa.setProperty(Ids::value, val, undoManager);
    }
}

MassPropertiesComponent::MassPropertiesComponent(ObjController* objController_,
                                                 Array<ValueTree> datas_,
                                                 UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_)
{
    editors.add(createRow("Mass", "Mass (kg)", true));
    editors.add(createRow("Pos", "Position (m)", true));
    editors.add(createRow("Vel", "Valocity (m/s)", true));

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
        setRowMultipleSelection();
    }
    else
    {
        ValueTree data = datas[0];

        readRowsSingleSelection(data.getChildWithName(Ids::parameters));
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
        writeRowsSingleSelection(paramsTree, true);
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
    editors.add(createRow("Freq", "Frequency (Hz)", true));
    editors.add(createRow("Decay", "Decay (sec)", true));
    editors.add(createRow("EqMass", "Mass (kg)", true));

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
        setRowMultipleSelection();
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
        editors[0]->setText(valFreq);
        editors[1]->setText(valDecay);
        editors[2]->setText(valEqMass);
        editors[0]->setTextToShowWhenEmpty(String::empty, Colours::black);
        editors[1]->setTextToShowWhenEmpty(String::empty, Colours::black);
        editors[2]->setTextToShowWhenEmpty(String::empty, Colours::black);
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
    editors.add(createRow("Pos", "Position (m)", true));

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
        setRowMultipleSelection();
    }
    else
    {
        readRowsSingleSelection(datas[0].getChildWithName(Ids::parameters));
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
    editors.add(createRow("Stiff", "Stiffness (N/m)", true));
    editors.add(createRow("Damp", "Damping (N/(m/s))", true));
    if(datas[0].getType() == Ids::pluck)
    {
        editors.add(createRow("MinDisplace", "Minimum displace diff", true));
//        editors.add(createRow("Pos", "Position offste (m)", true));
    }
    editors.add(createRow("Pos", "Position offste (m)", true));
    if(datas[0].getType() == Ids::pulsetouch)
    {
//        editors.add(createRow("Pos", "Position offste (m)", true));
        editors.add(createRow("PulseMult", "Pulse multiplier", true));
        editors.add(createRow("PulseTau", "Pulse tau", true));
        editors.add(createRow("PulseLen", "Pulse length", true));
    }

    TextEditor* teStartVertex = new TextEditor("teStartVertex");
    Label* laStartVertex = new Label("laStartVertex", "Start vertex");
    teStartVertex->setReadOnly(true);
    teStartVertex->addListener(this);
    teStartVertex->setColour(TextEditor::textColourId, Colours::darkgrey);
    addAndMakeVisible(teStartVertex);
    laStartVertex->attachToComponent(teStartVertex, true);

    TextEditor* teEndVertex = new TextEditor("teEndVertex");
    Label* laEndVertex = new Label("laEndVertex", "End vertex");
    teEndVertex->setReadOnly(true);
    teEndVertex->addListener(this);
    teEndVertex->setColour(TextEditor::textColourId, Colours::darkgrey);
    addAndMakeVisible(teEndVertex);
    laEndVertex->attachToComponent(teEndVertex, true);

    vertices.add(teStartVertex);
    vertices.add(teEndVertex);
    
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
        setRowMultipleSelection();
        
        vertices[0]->setText(String::empty);
        vertices[0]->setTextToShowWhenEmpty(multipleSelectionText, Colours::black);
        vertices[1]->setText(String::empty);
        vertices[1]->setTextToShowWhenEmpty(multipleSelectionText, Colours::black);

    }
    else
    {
        ValueTree data = datas[0];

        readRowsSingleSelection(data.getChildWithName(Ids::parameters));
        
        vertices[0]->setText(data[Ids::startVertex].toString());
        vertices[0]->setTextToShowWhenEmpty(String::empty, Colours::black);
        vertices[1]->setText(data[Ids::endVertex].toString());
        vertices[1]->setTextToShowWhenEmpty(String::empty, Colours::black);
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

        writeRowsSingleSelection(paramsTree, true);
    }

    return true;
}

AudiooutPropertiesComponent::AudiooutPropertiesComponent(ObjController* objController_,
                                                         Array<ValueTree> datas_,
                                                         UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_)
{
    editors.add(createRow("Source", "Source", true));
    editors[0]->addListener(this);
    editors[0]->setTextToShowWhenEmpty("0.0", Colours::darkgrey);
    editors[0]->setColour(TextEditor::textColourId, Colours::darkgrey);

    editors.add(createRow("Opt", "Optional", true));

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
        setRowMultipleSelection();
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
        editors[0]->setText(sourceText);
        editors[0]->setTextToShowWhenEmpty("0.0", Colours::darkgrey);
        editors[1]->setText(data.getProperty(Ids::optional).toString());
        editors[1]->setTextToShowWhenEmpty(String::empty, Colours::black);
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
    editors.add(createRow("WaveImp", "Wave impedance (N/(m/s))", true));
    editors.add(createRow("StringType", "String Type", true));

    TextEditor* teLeftObj = new TextEditor("teLeftObj");
    Label* laLeftObj = new Label("laLeftObj", "Left object");
    teLeftObj->setReadOnly(true);
    teLeftObj->addListener(this);
    teLeftObj->setColour(TextEditor::textColourId, Colours::darkgrey);
    addAndMakeVisible(teLeftObj);
    laLeftObj->attachToComponent(teLeftObj, true);

    TextEditor* teRightObj = new TextEditor("teRightObj");
    Label* laRightObj = new Label("laRightObj", "Right object");
    teRightObj->setReadOnly(true);
    teRightObj->addListener(this);
    teRightObj->setColour(TextEditor::textColourId, Colours::darkgrey);
    addAndMakeVisible(teRightObj);
    laRightObj->attachToComponent(teRightObj, true);

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
        setRowMultipleSelection();
        vertices[0]->setText(String::empty);
        vertices[0]->setTextToShowWhenEmpty(multipleSelectionText, Colours::darkgrey);
        vertices[1]->setText(String::empty);
        vertices[1]->setTextToShowWhenEmpty(multipleSelectionText, Colours::darkgrey);
    }
    else
    {
        ValueTree data = datas[0];
        readRowsSingleSelection(data.getChildWithName(Ids::parameters));
        vertices[0]->setText(data[Ids::startVertex].toString());
        vertices[0]->setTextToShowWhenEmpty(String::empty, Colours::darkgrey);
        vertices[1]->setText(data[Ids::endVertex].toString());
        vertices[1]->setTextToShowWhenEmpty(String::empty, Colours::darkgrey);
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
    editors.add(createRow("TermType", "Type", true));

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
        setRowMultipleSelection();
    }
    else
    {
        readRowsSingleSelection(datas[0].getChildWithName(Ids::parameters));
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

        writeRowsSingleSelection(paramsTree, false);
    }

    return true;
}

JunctionPropertiesComponent::JunctionPropertiesComponent(ObjController* objController_,
                                                         Array<ValueTree> datas_,
                                                         UndoManager* undoManager_)
: ObjectPropertiesComponent(objController_, datas_, undoManager_)
{
    editors.add(createRow("Pos", "Displacement (m)", true));

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
        setRowMultipleSelection();
    }
    else
    {
        readRowsSingleSelection(datas[0].getChildWithName(Ids::parameters));
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

        writeRowsSingleSelection(paramsTree, false);
    }

    return true;
}
