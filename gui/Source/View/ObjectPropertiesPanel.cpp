/*
  ==============================================================================

    ObjectPropertiesPanel.cpp
    Created: 13 May 2012 7:22:03pm
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
#include "../Controller/MDLController.h"
#include "../Controller/ObjController.h"
#include "../Graph/Node.h"
#include "../View/BaseObjectComponent.h"
#include "../View/SelectableObject.h"
#include "ObjectComponent.h"

#include "ObjectPropertiesPanel.h"

using namespace synthamodeler;
//==============================================================================

class ObjectPropertiesComponent : public Component,
								  public Button::Listener,
                                  public TextEditor::Listener
{
public:
	ObjectPropertiesComponent(ObjectPropertiesPanel* op_,
                             ObjController* objController_,
                             ValueTree data_, 
                             UndoManager* undoManager_)
	: op(op_),
      objController(objController_),
	  data(data_),
	  undoManager(undoManager_),
	  btOk("Ok"),
	  btCancel("Cancel"),
	  laName("laName", "Identifier"),
	  teName("teName"),
      laDebug(""),
      dataChanged(false)
	{
		btOk.addListener(this);
		addAndMakeVisible(&btOk);
		btCancel.addListener(this);
		addAndMakeVisible(&btCancel);
		addAndMakeVisible(&teName);
        teName.addListener(this);
		laName.attachToComponent(&teName, true);
#ifdef _DEBUG
        addAndMakeVisible(&laDebug);
        laDebug.setText("Pos: " + data[Ids::posX].toString() + String(" ") + 
                        data[Ids::posY].toString(), dontSendNotification);
#endif
        op->returnVal = 0;
	}

	virtual ~ObjectPropertiesComponent()
	{
	}

	void resized()
	{
		btOk.setBounds(getWidth()/2 - 65, getHeight() - 30, 60, 22);
		btCancel.setBounds(getWidth()/2 + 5, getHeight() - 30, 60, 22);
		teName.setBounds(100 , 10, getWidth() - 110, 22);
        laDebug.setBounds(65, getHeight() - 60, 100, 22);
	}

	void buttonClicked(Button* button)
	{
		
		if(button == &btOk)
		{
			applyEditing();
		}
		else if( button == &btCancel)
		{
			cancelEditing();
		}
	}

    bool checkIfIdExists(const String& idStr)
    {
        return objController->checkIfIdExists(data.getType(), idStr);
    }
	virtual void readValues() = 0;
	virtual bool writeValues() = 0;

    void textEditorTextChanged(TextEditor& editor)
    {
        dataChanged = true;
    }
    void textEditorReturnKeyPressed(TextEditor& editor)
    {
        applyEditing();
    }
    void textEditorEscapeKeyPressed(TextEditor& editor)
    {
        cancelEditing();
    }
    void textEditorFocusLost(TextEditor& editor)
    {
    }
    void applyEditing()
    {
        undoManager->beginNewTransaction("Change object properties");
        if(dataChanged)
        {
            bool writeOk = writeValues();
            undoManager->beginNewTransaction();
            if (writeOk)
            {
                SAM_LOG("Change " + data.getType().toString() + " " +
                        data[Ids::identifier].toString());
                op->returnVal = 1;
                op->closeButtonPressed();
            }
            else
            {
                SAM_LOG("Name already exists");
                Alerts::nameAlreadyExists();
            }
        }
        else
        {
            op->returnVal = 1;
            op->closeButtonPressed();
        }
    }
    void cancelEditing()
    {
        SAM_LOG("Cancel change " + data.getType().toString() + " " +
                data[Ids::identifier].toString());
        op->returnVal = 2;
        op->closeButtonPressed();
    }
protected:
	ObjectPropertiesPanel* op;
    ObjController* objController;
	ValueTree data;
	UndoManager* undoManager;
	TextButton btOk;
	TextButton btCancel;
	Label laName;
	TextEditor teName;
    Label laDebug;
    bool dataChanged;
};

class MassPropertiesComponent : public ObjectPropertiesComponent
{
public:
	MassPropertiesComponent(ObjectPropertiesPanel* op_,
                         ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_),
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

	~MassPropertiesComponent()
	{

	}

	void resized()
	{
		ObjectPropertiesComponent::resized();

		teMass.setBounds(100 , 40, getWidth() - 110, 22);
		tePos.setBounds(100 , 70, getWidth() - 110, 22);
		teVel.setBounds(100 , 100, getWidth() - 110, 22);
	}

	void readValues()
	{
		teName.setText(data[Ids::identifier].toString());
        teMass.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
        tePos.setText(data.getChildWithName(Ids::parameters).getChild(1)[Ids::value].toString());
        teVel.setText(data.getChildWithName(Ids::parameters).getChild(2)[Ids::value].toString());

	}
	bool writeValues()
	{
        String newName = teName.getText();
        String oldName = data[Ids::identifier];
        if(newName != oldName)
        {
            if (objController->checkIfIdExists(data.getType(), newName))
            {
                return false;
            }
            else
            {
                if(! objController->renameId(data.getType(), oldName, newName, undoManager))
                    return false;
            }
            objController->changeObjectNameInLink(oldName, newName, undoManager);
            objController->changeObjectNameInAudioSources(oldName, newName, undoManager);

            data.setProperty(Ids::identifier, newName, undoManager);
        }

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
private:
	Label laMass;
	TextEditor teMass;
	Label laPos;
	TextEditor tePos;
	Label laVel;
	TextEditor teVel;

};

class PortPropertiesComponent : public ObjectPropertiesComponent
{
public:
	PortPropertiesComponent(ObjectPropertiesPanel* op_,
                         ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_)
	{
		readValues();
	}

	~PortPropertiesComponent()
	{

	}

	void resized()
	{
		ObjectPropertiesComponent::resized();
	}

	void readValues()
	{
		teName.setText(data[Ids::identifier].toString());
	}

	bool writeValues()
	{
        String newName = teName.getText();
        String oldName = data[Ids::identifier];
        if(newName != oldName)
        {
            if (objController->checkIfIdExists(data.getType(), newName))
            {
                return false;
            }
            else
            {
                if(! objController->renameId(data.getType(), oldName, newName, undoManager))
                    return false;
            }
            objController->changeObjectNameInLink(oldName, newName, undoManager);
            objController->changeObjectNameInAudioSources(oldName, newName, undoManager);

            data.setProperty(Ids::identifier, newName, undoManager);
        }

        return true;
	}
private:
};

class ResonatorPropertiesComponent : public ObjectPropertiesComponent {
public:
	ResonatorPropertiesComponent(ObjectPropertiesPanel* op_,
                                 ObjController* objController_,
                                 ValueTree data_,
                                 UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_),
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
	virtual ~ResonatorPropertiesComponent()
	{

	}

	void resized()
	{
		ObjectPropertiesComponent::resized();

		teFreq.setBounds(100 , 40, getWidth() - 110, 22);
		teDecay.setBounds(100 , 70, getWidth() - 110, 22);
		teEqMass.setBounds(100 , 100, getWidth() - 110, 22);
	}

	void readValues()
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

	bool writeValues()
	{
        String newName = teName.getText();
        String oldName = data[Ids::identifier];
        if(newName != oldName)
        {
            if (objController->checkIfIdExists(data.getType(), newName))
            {
                return false;
            }
            else
            {
                if(! objController->renameId(data.getType(), oldName, newName, undoManager))
                    return false;
            }
            objController->changeObjectNameInLink(oldName, newName, undoManager);
            objController->changeObjectNameInAudioSources(oldName, newName, undoManager);

            data.setProperty(Ids::identifier, newName, undoManager);
        }

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

private:
    Label laFreq;
	TextEditor teFreq;
	Label laDecay;
	TextEditor teDecay;
	Label laEqMass;
	TextEditor teEqMass;
};

class GroundPropertiesComponent : public ObjectPropertiesComponent {
public:
	GroundPropertiesComponent(ObjectPropertiesPanel* op_,
                         ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_),
	  laPos("laPos", "Position (m)"),
	  tePos("tePos")
	{
        tePos.addListener(this);
		addAndMakeVisible(&tePos);
		laPos.attachToComponent(&tePos, true);

		readValues();
	}
	virtual ~GroundPropertiesComponent()
	{

	}

	void resized()
	{
		ObjectPropertiesComponent::resized();

		tePos.setBounds(100 , 40, getWidth() - 110, 22);
	}

	void readValues()
	{
		teName.setText(data[Ids::identifier].toString());
		tePos.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
	}

	bool writeValues()
	{
        String newName = teName.getText();
        String oldName = data[Ids::identifier];
        if(newName != oldName)
        {
            if (objController->checkIfIdExists(data.getType(), newName))
            {
                return false;
            }
            else
            {
                if(! objController->renameId(data.getType(), oldName, newName, undoManager))
                    return false;
            }
            objController->changeObjectNameInLink(oldName, newName, undoManager);
            objController->changeObjectNameInAudioSources(oldName, newName, undoManager);

            data.setProperty(Ids::identifier, newName, undoManager);
        }

		ValueTree paramsTree = data.getChildWithName(Ids::parameters);
        ValueTree param = paramsTree.getChild(0);
        param.setProperty(Ids::value, tePos.getText(), undoManager);
        return true;
	}

private:
	Label laPos;
	TextEditor tePos;

};

class LinkPropertiesComponent : public ObjectPropertiesComponent {
public:
    LinkPropertiesComponent(ObjectPropertiesPanel* op_,
                            ObjController* objController_,
                            ValueTree data_,
                            UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_),
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
        laPulseFreq("laPulseFreq", "Pulse frequency"),
        tePulseFreq("tePulseFreq")
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
        if(data.getType() == Ids::pluck)
        {
            teMinDisplace.addListener(this);
            addAndMakeVisible(&teMinDisplace);
            laMinDisplace.attachToComponent(&teMinDisplace, true);
        }
        else if (data.getType() == Ids::pulsetouch)
        {
            tePulseFreq.addListener(this);
            addAndMakeVisible(&tePulseFreq);
            laPulseFreq.attachToComponent(&tePulseFreq, true);
        }
		readValues();
	}
	virtual ~LinkPropertiesComponent()
	{

    }

	void resized()
	{
		ObjectPropertiesComponent::resized();
        int offset = 0;
        teStiff.setBounds(100 , 40, getWidth() - 110, 22);
		teDamp.setBounds(100 , 70, getWidth() - 110, 22);
        if(data.getType() == Ids::pluck)
        {
            teMinDisplace.setBounds(100 , 100, getWidth() - 110, 22);
            offset = 30;
        }
        else if (data.getType() == Ids::pulsetouch)
        {
            tePulseFreq.setBounds(100, 100, getWidth() - 110, 22);
            offset = 30;
        }
		tePos.setBounds(100 , 100+offset, getWidth() - 110, 22);
        teStartVertex.setBounds(100, 130+offset, getWidth() - 110, 22);
        teEndVertex.setBounds(100, 160+offset, getWidth() - 110, 22);
	}

	void readValues()
	{
		teName.setText(data[Ids::identifier].toString());
		teStiff.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
		teDamp.setText(data.getChildWithName(Ids::parameters).getChild(1)[Ids::value].toString());
        int offset = 0;
        if(data.getType() == Ids::pluck)
        {
            teMinDisplace.setText(data.getChildWithName(Ids::parameters).getChild(2)[Ids::value].toString());
            ++offset;
        }
        else if(data.getType() == Ids::pulsetouch)
        {
            tePulseFreq.setText(data.getChildWithName(Ids::parameters).getChild(3)[Ids::value].toString());
        }
		tePos.setText(data.getChildWithName(Ids::parameters).getChild(2+offset)[Ids::value].toString());
        teStartVertex.setText(data[Ids::startVertex].toString());
        teEndVertex.setText(data[Ids::endVertex].toString());
	}

	bool writeValues()
	{
        String newName = teName.getText();
        String oldName = data[Ids::identifier].toString();
        
        if (oldName != newName)
        {
            if (objController->checkIfIdExists(data.getType(), newName))
            {
                return false;
            }
            else
            {
                if(! objController->renameId(data.getType(), oldName, newName, undoManager))
                    return false;
            }

            objController->changeObjectNameInAudioSources(oldName, newName, undoManager);

            data.setProperty(Ids::identifier, newName, undoManager);
        }
        
		ValueTree paramsTree = data.getChildWithName(Ids::parameters);
        ValueTree pa1 = paramsTree.getChild(0);
        ValueTree pa2 = paramsTree.getChild(1);
        ValueTree pa3 = paramsTree.getChild(2);
        ValueTree pa4 = paramsTree.getChild(3);
        
        pa1.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(teStiff.getText()),
                        undoManager);
        pa2.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(teDamp.getText()),
                        undoManager);
        if(data.getType() == Ids::pluck)
        {
            pa3.setProperty(Ids::value,
                            Utils::fixParameterValueIfNeeded(teMinDisplace.getText()),
                            undoManager);
            pa4.setProperty(Ids::value,
                            Utils::fixParameterValueIfNeeded(tePos.getText()),
                            undoManager);
        }
        else if(data.getType() == Ids::pulsetouch)
        {
            pa3.setProperty(Ids::value,
                            Utils::fixParameterValueIfNeeded(tePos.getText()),
                            undoManager);
            pa4.setProperty(Ids::value,
                            Utils::fixParameterValueIfNeeded(tePulseFreq.getText()),
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

private:
    Label laStiff;
	TextEditor teStiff;
	Label laDamp;
	TextEditor teDamp;
	Label laPos;
	TextEditor tePos;
    Label laStartVertex;
    TextEditor teStartVertex;
    Label laEndVertex;
    TextEditor teEndVertex;
    Label laMinDisplace;
    TextEditor teMinDisplace;
    Label laPulseFreq;
    TextEditor tePulseFreq;
};

class AudiooutPropertiesComponent : public ObjectPropertiesComponent {
public:
    AudiooutPropertiesComponent(ObjectPropertiesPanel* op_,
                                ObjController* objController_,
                                ValueTree data_,
                                UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_),
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
        laSource.attachToComponent(&teSource,true);
        teOpt.addListener(this);
        addAndMakeVisible(&teOpt);
        laOpt.attachToComponent(&teOpt, true);
        
		readValues();
	}
	virtual ~AudiooutPropertiesComponent()
	{

	}

	void resized()
	{
		ObjectPropertiesComponent::resized();
        
        teSource.setBounds(100 , 40, getWidth() - 110, 22);
        teOpt.setBounds(100 , 70, getWidth() - 110, 22);
	}

	void readValues()
	{
        teName.setText(data[Ids::identifier].toString());
        String sourceText;
        ValueTree sourcesTree = data.getChildWithName(Ids::sources);
        for (int i = 0; i < sourcesTree.getNumChildren(); ++i)
        {
            ValueTree source = sourcesTree.getChild(i);
            sourceText << source[Ids::value].toString();
            if(i != sourcesTree.getNumChildren()-1)
                sourceText << "+";
        }
        teSource.setText(sourceText);
        teOpt.setText(data.getProperty(Ids::optional).toString());
	}

	bool writeValues()
    {
        String newName = teName.getText();
        String oldName = data[Ids::identifier].toString();
        
        if(newName != oldName)
        {
            if (objController->checkIfIdExists(data.getType(), newName))
            {
                return false;
            }
            else
            {
                if(! objController->renameId(data.getType(), oldName, newName, undoManager))
                    return false;
            }

            data.setProperty(Ids::identifier, newName, undoManager);
        }
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

private:
    Label laSource;
    TextEditor teSource;
    Label laOpt;
    TextEditor teOpt;
};

class WaveguidePropertiesComponent : public ObjectPropertiesComponent {
public:
	WaveguidePropertiesComponent(ObjectPropertiesPanel* op_,
                         ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_),
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
	virtual ~WaveguidePropertiesComponent()
    {
	}

	void resized()
	{
		ObjectPropertiesComponent::resized();

        teWaveImp.setBounds(100, 40, getWidth() - 110, 22);
		teStringType.setBounds(100 , 70, getWidth() - 110, 22);
        teLeftObj.setBounds(100, 130, getWidth() - 110, 22);
        teRightObj.setBounds(100, 160, getWidth() - 110, 22);
	}

	void readValues()
    {
        teName.setText(data[Ids::identifier].toString());
		teWaveImp.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
		teStringType.setText(data.getChildWithName(Ids::parameters).getChild(1)[Ids::value].toString());
        teLeftObj.setText(data[Ids::startVertex].toString());
        teRightObj.setText(data[Ids::endVertex].toString());
	}

	bool writeValues()
	{
        String newName = teName.getText();
        String oldName = data[Ids::identifier].toString();

        if (oldName != newName)
        {
            if (objController->checkIfIdExists(data.getType(), newName))
            {
                return false;
            }
            else
            {
                if(! objController->renameId(data.getType(), oldName, newName, undoManager))
                    return false;
            }

//            objController->changeObjectNameInAudioSources(oldName, newName, undoManager);

            data.setProperty(Ids::identifier, newName, undoManager);
        }

		ValueTree paramsTree = data.getChildWithName(Ids::parameters);
        ValueTree pa1 = paramsTree.getChild(0);
        ValueTree pa2 = paramsTree.getChild(1);

        pa1.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(teWaveImp.getText()),
                        undoManager);
        pa2.setProperty(Ids::value, teStringType.getText(), undoManager);

        return true;
	}

private:
    Label laWaveImp;
	TextEditor teWaveImp;
    Label laStringType;
	TextEditor teStringType;
    Label laLeftObj;
    TextEditor teLeftObj;
    Label laRightObj;
    TextEditor teRightObj;
};

class TerminationPropertiesComponent : public ObjectPropertiesComponent {
public:
	TerminationPropertiesComponent(ObjectPropertiesPanel* op_,
                         ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_),
      laTermType("laTermType", "Type"),
	  teTermType("teTermType")
	{
        teTermType.addListener(this);
		addAndMakeVisible(&teTermType);
		laTermType.attachToComponent(&teTermType, true);

		readValues();
	}
	virtual ~TerminationPropertiesComponent()
	{

	}

	void resized()
	{
		ObjectPropertiesComponent::resized();

        teTermType.setBounds(100, 40, getWidth() - 110, 22);
	}

	void readValues()
    {
        teName.setText(data[Ids::identifier].toString());
		teTermType.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
	}

	bool writeValues()
    {
        String newName = teName.getText();
        String oldName = data[Ids::identifier];
        if(newName != oldName)
        {
            if (objController->checkIfIdExists(data.getType(), newName))
            {
                return false;
            }
            else
            {
                if(! objController->renameId(data.getType(), oldName, newName, undoManager))
                    return false;
            }
            objController->changeObjectNameInLink(oldName, newName, undoManager);
//            objController->changeObjectNameInAudioSources(oldName, newName, undoManager);

            data.setProperty(Ids::identifier, newName, undoManager);
        }

		ValueTree paramsTree = data.getChildWithName(Ids::parameters);
        ValueTree param = paramsTree.getChild(0);
        param.setProperty(Ids::value, teTermType.getText(), undoManager);
        return true;
	}

private:
	Label laTermType;
	TextEditor teTermType;
};

class JunctionPropertiesComponent : public ObjectPropertiesComponent {
public:
	JunctionPropertiesComponent(ObjectPropertiesPanel* op_,
                         ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_),
	  laPos("laPos", "Displacement (m)"),
	  tePos("tePos")
	{
        tePos.addListener(this);
		addAndMakeVisible(&tePos);
		laPos.attachToComponent(&tePos, true);

		readValues();
	}
	virtual ~JunctionPropertiesComponent()
	{

	}

	void resized()
	{
		ObjectPropertiesComponent::resized();

        tePos.setBounds(100, 40, getWidth() - 110, 22);
	}

	void readValues()
    {
        teName.setText(data[Ids::identifier].toString());
		tePos.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
	}

	bool writeValues()
	{
        String newName = teName.getText();
        String oldName = data[Ids::identifier];
        if(newName != oldName)
        {
            if (objController->checkIfIdExists(data.getType(), newName))
            {
                return false;
            }
            else
            {
                if(! objController->renameId(data.getType(), oldName, newName, undoManager))
                    return false;
            }
            objController->changeObjectNameInLink(oldName, newName, undoManager);
//            objController->changeObjectNameInAudioSources(oldName, newName, undoManager);

            data.setProperty(Ids::identifier, newName, undoManager);
        }

		ValueTree paramsTree = data.getChildWithName(Ids::parameters);
        ValueTree param = paramsTree.getChild(0);
        param.setProperty(Ids::value, tePos.getText(), undoManager);
        return true;
	}

private:
	Label laPos;
	TextEditor tePos;
};

//==============================================================================
static HashMap<String, String> perpertiesWindowPos;
static String currentObjectId;

ObjectPropertiesPanel::ObjectPropertiesPanel(ObjController* objController,
                                             BaseObjectComponent* caller, 
                                             UndoManager* undoManager_)
    : DialogWindow (caller->getData().getType().toString() + " properties",
                    Colour::fromRGBA((uint8)200, (uint8)200, (uint8)200, (uint8)200), true),
      returnVal(0)
{

	Component* comp;
    currentObjectId = caller->getData()[Ids::identifier].toString();

	if(caller->getData().getType() == Ids::mass)
    {
        comp = new MassPropertiesComponent(this, objController,
                                           caller->getData(), undoManager_);
	}
	else if(caller->getData().getType() == Ids::port)
    {
        comp = new PortPropertiesComponent(this, objController,
                                           caller->getData(), undoManager_);
	}
	else if(caller->getData().getType() == Ids::resonators)
    {
        comp = new ResonatorPropertiesComponent(this, objController,
                                                caller->getData(), undoManager_);
	}
	else if(caller->getData().getType() == Ids::ground)
	{
		comp = new GroundPropertiesComponent(this, objController, 
                                             caller->getData(), undoManager_);
	}
	else if(caller->getData().getType() == Ids::link
        || caller->getData().getType() == Ids::touch
        || caller->getData().getType() == Ids::pluck
        || caller->getData().getType() == Ids::pulsetouch)
    {
        comp = new LinkPropertiesComponent(this, objController,
                                           caller->getData(), undoManager_);
    }
	else if(caller->getData().getType() == Ids::waveguide)
    {
        comp = new WaveguidePropertiesComponent(this, objController,
                                                caller->getData(), undoManager_);
	}
	else if(caller->getData().getType() == Ids::junction)
    {
        comp = new JunctionPropertiesComponent(this, objController,
                                               caller->getData(), undoManager_);
	}
	else if(caller->getData().getType() == Ids::termination)
    {
        comp = new TerminationPropertiesComponent(this, objController,
                                                  caller->getData(), undoManager_);
	}
	else if(caller->getData().getType() == Ids::audioout)
    {
        comp = new AudiooutPropertiesComponent(this, objController,
                                               caller->getData(), undoManager_);
	}
	else
	{
		comp = new Component();
        currentObjectId = "";
	}

	comp->setSize (280, 300);
	setContentOwned (comp, true);

    if (! restoreWindowStateFromString (perpertiesWindowPos[currentObjectId]))
        centreAroundComponent(caller, getWidth(), getHeight());

    setResizable (true, true);
    setVisible(true);
}

ObjectPropertiesPanel::~ObjectPropertiesPanel()
{
    perpertiesWindowPos.set(currentObjectId, getWindowStateAsString());
}

void ObjectPropertiesPanel::closeButtonPressed()
{
    setVisible (false);
}

int ObjectPropertiesPanel::show(ObjController* objController, 
                                BaseObjectComponent* caller,
                                UndoManager* undoManager_)
{
	ObjectPropertiesPanel ep(objController, caller, undoManager_);
    ep.runModalLoop();
    return ep.returnVal;
}
