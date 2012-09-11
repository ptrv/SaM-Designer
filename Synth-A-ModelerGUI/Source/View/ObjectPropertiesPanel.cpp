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
#include "ObjectComponent.h"

#include "ObjectPropertiesPanel.h"

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
                        data[Ids::posY].toString(),false);
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
	  teVel("teVel"),
	  laLabels("laLabels", "Labels"),
	  teLabels("teLabels")
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
        teLabels.addListener(this);
		addAndMakeVisible(&teLabels);
		laLabels.attachToComponent(&teLabels, true);

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
		teLabels.setBounds(100, 130, getWidth() - 110, 22);
	}

	void readValues()
	{
		teName.setText(data[Ids::identifier].toString());
		teMass.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
		tePos.setText(data.getChildWithName(Ids::parameters).getChild(1)[Ids::value].toString());
		teVel.setText(data.getChildWithName(Ids::parameters).getChild(2)[Ids::value].toString());
		String labelText;
		StringArray labelsArray;
		for (int i = 0; i < data.getChildWithName(Ids::labels).getNumChildren(); ++i) {
			labelsArray.add(data.getChildWithName(Ids::labels).getChild(i)[Ids::value].toString());
		}
		teLabels.setText(labelsArray.joinIntoString(","));
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
        ValueTree labelsTree = data.getChildWithName(Ids::labels);
        labelsTree.removeAllChildren(undoManager);
        String labelsString = teLabels.getText();
        StringArray labelsArray;
        labelsArray.addTokens(labelsString, ",", "\"");
        for (int i = 0; i < labelsArray.size(); ++i)
        {
            ValueTree label(Ids::label);
            label.setProperty(Ids::value, labelsArray[i], undoManager);
            labelsTree.addChild(label, -1, undoManager);
        }
        
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
	Label laLabels;
	TextEditor teLabels;

};

class PortPropertiesComponent : public ObjectPropertiesComponent
{
public:
	PortPropertiesComponent(ObjectPropertiesPanel* op_,
                         ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_),
	  laLabels("laLabels", "Labels"),
	  teLabels("teLabels")
	{
        teLabels.addListener(this);
		addAndMakeVisible(&teLabels);
		laLabels.attachToComponent(&teLabels, true);

		readValues();
	}

	~PortPropertiesComponent()
	{

	}

	void resized()
	{
		ObjectPropertiesComponent::resized();

		teLabels.setBounds(100, 40, getWidth() - 110, 22);
	}

	void readValues()
	{
		teName.setText(data[Ids::identifier].toString());
		String labelText;
		StringArray labelsArray;
		for (int i = 0; i < data.getChildWithName(Ids::labels).getNumChildren(); ++i) {
			labelsArray.add(data.getChildWithName(Ids::labels).getChild(i)[Ids::value].toString());
		}
		teLabels.setText(labelsArray.joinIntoString(","));

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

		ValueTree labelsTree = data.getChildWithName(Ids::labels);
        labelsTree.removeAllChildren(undoManager);
		String labelsString = teLabels.getText();
		StringArray labelsArray;
		labelsArray.addTokens(labelsString, ",", "\"");
		for (int i = 0; i < labelsArray.size(); ++i) {
            ValueTree label(Ids::label);
            label.setProperty(Ids::value, labelsArray[i], undoManager);
            labelsTree.addChild(label, -1, undoManager);
        }
        return true;
	}
	Label laLabels;
	TextEditor teLabels;

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
	  teEqMass("teEqMass"),
	  laLabels("laLabels", "Labels"),
	  teLabels("teLabels")

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
        teLabels.addListener(this);
		addAndMakeVisible(&teLabels);
		laLabels.attachToComponent(&teLabels, true);

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
		teLabels.setBounds(100, 130, getWidth() - 110, 22);

	}

	void readValues()
	{
		teName.setText(data[Ids::identifier].toString());
		teFreq.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
		teDecay.setText(data.getChildWithName(Ids::parameters).getChild(1)[Ids::value].toString());
		teEqMass.setText(data.getChildWithName(Ids::parameters).getChild(2)[Ids::value].toString());
		String labelText;
		StringArray labelsArray;
		for (int i = 0; i < data.getChildWithName(Ids::labels).getNumChildren(); ++i) {
			labelsArray.add(data.getChildWithName(Ids::labels).getChild(i)[Ids::value].toString());
		}
		teLabels.setText(labelsArray.joinIntoString(","));

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
                        Utils::fixParameterValueIfNeeded(teFreq.getText()),
                        undoManager);
        pa2.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(teDecay.getText()),
                        undoManager);
        pa3.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(teEqMass.getText()), 
                        undoManager);
        ValueTree labelsTree = data.getChildWithName(Ids::labels);
        labelsTree.removeAllChildren(undoManager);
        String labelsString = teLabels.getText();
        StringArray labelsArray;
        labelsArray.addTokens(labelsString, ",", "\"");
        for (int i = 0; i < labelsArray.size(); ++i)
        {
            ValueTree label(Ids::label);
            label.setProperty(Ids::value, labelsArray[i], undoManager);
            labelsTree.addChild(label, -1, undoManager);
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
	Label laLabels;
	TextEditor teLabels;
};

class GroundPropertiesComponent : public ObjectPropertiesComponent {
public:
	GroundPropertiesComponent(ObjectPropertiesPanel* op_,
                         ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_),
	  laLabels("laLabels", "Labels"),
	  teLabels("teLabels"),
	  laPos("laPos", "Position (m)"),
	  tePos("tePos")
	{
        teLabels.addListener(this);
		addAndMakeVisible(&teLabels);
		laLabels.attachToComponent(&teLabels, true);
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
		teLabels.setBounds(100, 70, getWidth() - 110, 22);
	}

	void readValues()
	{
		teName.setText(data[Ids::identifier].toString());
		tePos.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
		String labelText;
		StringArray labelsArray;
		for (int i = 0; i < data.getChildWithName(Ids::labels).getNumChildren(); ++i) {
			labelsArray.add(data.getChildWithName(Ids::labels).getChild(i)[Ids::value].toString());
		}
		teLabels.setText(labelsArray.joinIntoString(","));

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
		ValueTree labelsTree = data.getChildWithName(Ids::labels);
		String labelsString = teLabels.getText();
		StringArray labelsArray;
		labelsArray.addTokens(labelsString, ",", "\"");
		for (int i = 0; i < labelsArray.size(); ++i) {
            ValueTree label(Ids::label);
            label.setProperty(Ids::value, labelsArray[i], undoManager);
            labelsTree.addChild(label, -1, undoManager);
		}
        return true;
	}

private:
	Label laLabels;
	TextEditor teLabels;
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
        laPos("laPos", "Position (m)"),
        tePos("tePos"),
        laLabels("laLabels", "Labels"),
        teLabels("teLabels"),
        laStartVertex("laStartVertex", "Start Vertex"),
        teStartVertex("teStartVertex"),
        laEndVertex("laEndVertex", "End Vertex"),
        teEndVertex("teEndVertex"),
        laMinDisplace("laMinDisplace", "Minimum displce diff"),
        teMinDisplace("teMinDisplace"),
        isPluck(false)
    {
        if(data.getType() == Ids::pluck)
            isPluck = true;

        teStiff.addListener(this);
        addAndMakeVisible(&teStiff);
		laStiff.attachToComponent(&teStiff, true);
        teDamp.addListener(this);
		addAndMakeVisible(&teDamp);
		laDamp.attachToComponent(&teDamp, true);
        tePos.addListener(this);
		addAndMakeVisible(&tePos);
		laPos.attachToComponent(&tePos, true);
        teLabels.addListener(this);
		addAndMakeVisible(&teLabels);
		laLabels.attachToComponent(&teLabels, true);
        teStartVertex.setReadOnly(true);
        teStartVertex.addListener(this);
        addAndMakeVisible(&teStartVertex);
		laStartVertex.attachToComponent(&teStartVertex, true);
        teEndVertex.setReadOnly(true);
        teEndVertex.addListener(this);
		addAndMakeVisible(&teEndVertex);
		laEndVertex.attachToComponent(&teEndVertex, true);
        if(isPluck)
        {
            teMinDisplace.addListener(this);
            addAndMakeVisible(&teMinDisplace);
            laMinDisplace.attachToComponent(&teMinDisplace, true);
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
        if(isPluck)
        {
            teMinDisplace.setBounds(100 , 100, getWidth() - 110, 22);
            offset = 30;
        }
		tePos.setBounds(100 , 100+offset, getWidth() - 110, 22);
		teLabels.setBounds(100, 130+offset, getWidth() - 110, 22);
        teStartVertex.setBounds(100, 160+offset, getWidth() - 110, 22);
        teEndVertex.setBounds(100, 190+offset, getWidth() - 110, 22);
	}

	void readValues()
	{
		teName.setText(data[Ids::identifier].toString());
		teStiff.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
		teDamp.setText(data.getChildWithName(Ids::parameters).getChild(1)[Ids::value].toString());
        int offset = 0;
        if(isPluck)
        {
            teMinDisplace.setText(data.getChildWithName(Ids::parameters).getChild(2)[Ids::value].toString());
            ++offset;
        }
		tePos.setText(data.getChildWithName(Ids::parameters).getChild(2+offset)[Ids::value].toString());
		String labelText;
		StringArray labelsArray;
		for (int i = 0; i < data.getChildWithName(Ids::labels).getNumChildren(); ++i) {
			labelsArray.add(data.getChildWithName(Ids::labels).getChild(i)[Ids::value].toString());
		}
		teLabels.setText(labelsArray.joinIntoString(","));
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
        if(isPluck)
        {
            pa3.setProperty(Ids::value,
                            Utils::fixParameterValueIfNeeded(teMinDisplace.getText()),
                            undoManager);
            pa4.setProperty(Ids::value,
                            Utils::fixParameterValueIfNeeded(tePos.getText()),
                            undoManager);
        }
        else
        {
            pa3.setProperty(Ids::value,
                            Utils::fixParameterValueIfNeeded(tePos.getText()),
                            undoManager);
        }
		ValueTree labelsTree = data.getChildWithName(Ids::labels);
        labelsTree.removeAllChildren(undoManager);
		String labelsString = teLabels.getText();
		StringArray labelsArray;
		labelsArray.addTokens(labelsString, ",", "\"");
		for (int i = 0; i < labelsArray.size(); ++i) {
            ValueTree label(Ids::label);
            label.setProperty(Ids::value, labelsArray[i], undoManager);
            labelsTree.addChild(label, -1, undoManager);
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
	Label laLabels;
	TextEditor teLabels;
    Label laStartVertex;
    TextEditor teStartVertex;
    Label laEndVertex;
    TextEditor teEndVertex;
    Label laMinDisplace;
    TextEditor teMinDisplace;
    bool isPluck;
};

class AudiooutPropertiesComponent : public ObjectPropertiesComponent {
public:
    AudiooutPropertiesComponent(ObjectPropertiesPanel* op_,
                                ObjController* objController_,
                                ValueTree data_,
                                UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_),
        laSource("laSource", "Source"),
        teSource("teSource")
	{
        teSource.setReadOnly(true);
        teSource.addListener(this);
        addAndMakeVisible(&teSource);
        laSource.attachToComponent(&teSource,true);
        
		readValues();
	}
	virtual ~AudiooutPropertiesComponent()
	{

	}

	void resized()
	{
		ObjectPropertiesComponent::resized();
        
        teSource.setBounds(100 , 40, getWidth() - 110, 22);
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
            sourceText << "*";
            sourceText << source[Ids::gain].toString();
            if(i != sourcesTree.getNumChildren()-1)
                sourceText << "+";
        }

        teSource.setText(sourceText);
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
            StringArray sourceParams;
            sourceParams.addTokens(sourcesList[i], "*", "\"");
            if(sourceParams.size() != 2)
                return false;

            source.setProperty(Ids::value, sourceParams[0], undoManager);
            source.setProperty(Ids::gain, sourceParams[1], undoManager);
            sourcesTree.addChild(source, -1, undoManager);
        }
        
        return true;
	}

private:
    Label laSource;
    TextEditor teSource;
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
      laMaxTime("laMaxTime", "Max delay (s)"),
      teMaxTime("teMaxTime"),
      laCurrTime("laCurrTime", "Current delay (s)"),
      teCurrTime("teCurrTime"),
      laLabels("laLabels", "Labels"),
      teLabels("teLabels"),
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
        teMaxTime.addListener(this);
		addAndMakeVisible(&teMaxTime);
		laMaxTime.attachToComponent(&teMaxTime, true);
        teLabels.addListener(this);
		addAndMakeVisible(&teLabels);
		laLabels.attachToComponent(&teLabels, true);
        teCurrTime.addListener(this);
        addAndMakeVisible(&teCurrTime);
		laCurrTime.attachToComponent(&teCurrTime, true);
        teLeftObj.setReadOnly(true);
        teLeftObj.addListener(this);
		addAndMakeVisible(&teLeftObj);
		laLeftObj.attachToComponent(&teLeftObj, true);
        teRightObj.setReadOnly(true);
        teRightObj.addListener(this);
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
		teMaxTime.setBounds(100 , 100, getWidth() - 110, 22);
		teCurrTime.setBounds(100, 130, getWidth() - 110, 22);
        teLabels.setBounds(100, 160, getWidth() - 110, 22);
        teLeftObj.setBounds(100, 190, getWidth() - 110, 22);
        teRightObj.setBounds(100, 220, getWidth() - 110, 22);
	}

	void readValues()
    {
        teName.setText(data[Ids::identifier].toString());
		teWaveImp.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
		teStringType.setText(data.getChildWithName(Ids::parameters).getChild(1)[Ids::value].toString());
        teMaxTime.setText(data.getChildWithName(Ids::parameters).getChild(1)[Ids::maxtime].toString());
        teCurrTime.setText(data.getChildWithName(Ids::parameters).getChild(1)[Ids::curtime].toString());

		String labelText;
		StringArray labelsArray;
		for (int i = 0; i < data.getChildWithName(Ids::labels).getNumChildren(); ++i) {
			labelsArray.add(data.getChildWithName(Ids::labels).getChild(i)[Ids::value].toString());
		}
		teLabels.setText(labelsArray.joinIntoString(","));
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
        pa2.setProperty(Ids::maxtime, 
                        Utils::fixParameterValueIfNeeded(teMaxTime.getText()),
                        undoManager);
        pa2.setProperty(Ids::curtime,
                        Utils::fixParameterValueIfNeeded(teCurrTime.getText()),
                        undoManager);

		ValueTree labelsTree = data.getChildWithName(Ids::labels);
        labelsTree.removeAllChildren(undoManager);
		String labelsString = teLabels.getText();
		StringArray labelsArray;
		labelsArray.addTokens(labelsString, ",", "\"");
		for (int i = 0; i < labelsArray.size(); ++i) {
            ValueTree label(Ids::label);
            label.setProperty(Ids::value, labelsArray[i], undoManager);
            labelsTree.addChild(label, -1, undoManager);
		}
        return true;
	}

private:
    Label laWaveImp;
	TextEditor teWaveImp;
    Label laStringType;
	TextEditor teStringType;
	Label laMaxTime;
	TextEditor teMaxTime;
	Label laCurrTime;
	TextEditor teCurrTime;
    Label laLeftObj;
    TextEditor teLeftObj;
    Label laRightObj;
    TextEditor teRightObj;
	Label laLabels;
	TextEditor teLabels;
};

class TerminationPropertiesComponent : public ObjectPropertiesComponent {
public:
	TerminationPropertiesComponent(ObjectPropertiesPanel* op_,
                         ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_),
      laTermType("laTermType", "Type"),
	  teTermType("teTermType"),
      laReflection("laReflection", "Reflection (-1 to 1)"),
	  teReflection("teTermType"),
      laLowPass("laLowPass", "Lowpass"),
	  teLowPass("teLowPass"),
      laLabels("laLabels", "Labels"),
	  teLabels("teLabels")
	{
        teLabels.addListener(this);
		addAndMakeVisible(&teLabels);
		laLabels.attachToComponent(&teLabels, true);
        teTermType.addListener(this);
		addAndMakeVisible(&teTermType);
		laTermType.attachToComponent(&teTermType, true);
        teReflection.addListener(this);
		addAndMakeVisible(&teReflection);
		laReflection.attachToComponent(&teReflection, true);
        teLowPass.addListener(this);
		addAndMakeVisible(&teLowPass);
		laLowPass.attachToComponent(&teLowPass, true);

		readValues();
	}
	virtual ~TerminationPropertiesComponent()
	{

	}

	void resized()
	{
		ObjectPropertiesComponent::resized();

        teTermType.setBounds(100, 40, getWidth() - 110, 22);
		teReflection.setBounds(100, 70, getWidth() - 110, 22);
        teLowPass.setBounds(100, 100, getWidth() - 110, 22);
		teLabels.setBounds(100, 130, getWidth() - 110, 22);
	}

	void readValues()
    {
        teName.setText(data[Ids::identifier].toString());
		teTermType.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
		teReflection.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::reflection].toString());
		teLowPass.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::lowpass].toString());
		String labelText;
		StringArray labelsArray;
		for (int i = 0; i < data.getChildWithName(Ids::labels).getNumChildren(); ++i) {
			labelsArray.add(data.getChildWithName(Ids::labels).getChild(i)[Ids::value].toString());
		}
		teLabels.setText(labelsArray.joinIntoString(","));

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
        param.setProperty(Ids::reflection, 
                          Utils::fixParameterValueIfNeeded(teReflection.getText()),
                          undoManager);
        param.setProperty(Ids::lowpass,
                          Utils::fixParameterValueIfNeeded(teLowPass.getText()),
                          undoManager);
		ValueTree labelsTree = data.getChildWithName(Ids::labels);
		String labelsString = teLabels.getText();
		StringArray labelsArray;
		labelsArray.addTokens(labelsString, ",", "\"");
		for (int i = 0; i < labelsArray.size(); ++i) {
            ValueTree label(Ids::label);
            label.setProperty(Ids::value, labelsArray[i], undoManager);
            labelsTree.addChild(label, -1, undoManager);
		}
        return true;
	}

private:
	Label laTermType;
	TextEditor teTermType;
   	Label laReflection;
	TextEditor teReflection;
	Label laLowPass;
	TextEditor teLowPass;
    Label laLabels;
	TextEditor teLabels;
};

class JunctionPropertiesComponent : public ObjectPropertiesComponent {
public:
	JunctionPropertiesComponent(ObjectPropertiesPanel* op_,
                         ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_),
      laLabels("laLabels", "Labels"),
	  teLabels("teLabels"),
	  laPos("laPos", "Displacement (m)"),
	  tePos("tePos")
	{
        teLabels.addListener(this);
		addAndMakeVisible(&teLabels);
		laLabels.attachToComponent(&teLabels, true);
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
		teLabels.setBounds(100, 70, getWidth() - 110, 22);
	}

	void readValues()
    {
        teName.setText(data[Ids::identifier].toString());
		tePos.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
		String labelText;
		StringArray labelsArray;
		for (int i = 0; i < data.getChildWithName(Ids::labels).getNumChildren(); ++i) {
			labelsArray.add(data.getChildWithName(Ids::labels).getChild(i)[Ids::value].toString());
		}
		teLabels.setText(labelsArray.joinIntoString(","));
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
		ValueTree labelsTree = data.getChildWithName(Ids::labels);
		String labelsString = teLabels.getText();
		StringArray labelsArray;
		labelsArray.addTokens(labelsString, ",", "\"");
		for (int i = 0; i < labelsArray.size(); ++i) {
            ValueTree label(Ids::label);
            label.setProperty(Ids::value, labelsArray[i], undoManager);
            labelsTree.addChild(label, -1, undoManager);
		}
        return true;
	}

private:
    Label laLabels;
	TextEditor teLabels;
	Label laPos;
	TextEditor tePos;
};

//==============================================================================
static String exportWindowPos;


ObjectPropertiesPanel::ObjectPropertiesPanel(ObjController* objController,
                                             BaseObjectComponent* caller, 
                                             UndoManager* undoManager_)
    : DialogWindow (Utils::getObjectTypeAsString(caller->getData().getType())+ " properties",
    		Colour::fromRGBA((uint8)200, (uint8)200, (uint8)200, (uint8)150), true),
      returnVal(0)
{

	Component* comp;
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
	else if(caller->getData().getType() == Ids::resonator)
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
        || caller->getData().getType() == Ids::pluck)
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
	}

	comp->setSize (280, 300);
	setContentOwned (comp, true);

	centreAroundComponent (caller, getWidth(), getHeight());

    setResizable (false, false);
    setVisible(true);
}

ObjectPropertiesPanel::~ObjectPropertiesPanel()
{
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
