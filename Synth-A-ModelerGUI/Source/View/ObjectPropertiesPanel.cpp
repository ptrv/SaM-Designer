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
								  public Button::Listener
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
      laDebug("")
	{
		btOk.addListener(this);
		addAndMakeVisible(&btOk);
		btCancel.addListener(this);
		addAndMakeVisible(&btCancel);
		addAndMakeVisible(&teName);
		laName.attachToComponent(&teName, true);
#ifdef _DEBUG
        addAndMakeVisible(&laDebug);
        laDebug.setText("Pos: " + data[Ids::posX].toString() + String(" ") + 
                        data[Ids::posY].toString(),false);
#endif        
	}

	virtual ~ObjectPropertiesComponent()
	{
	}

	void resized()
	{
		btOk.setBounds(getWidth()/2 - 65, getHeight() - 30, 60, 22);
		btCancel.setBounds(getWidth()/2 + 5, getHeight() - 30, 60, 22);
		teName.setBounds(80 , 10, getWidth() -90, 22);
        laDebug.setBounds(65, getHeight() - 60, 100, 22);
	}

	void buttonClicked(Button* button)
	{
		op->returnVal = 0;
		if(button == &btOk)
		{
			undoManager->beginNewTransaction("Change object properties");
			bool writeOk = writeValues();
            undoManager->beginNewTransaction();
			if(writeOk)
            {
    			SAM_LOG("Change "+data.getType().toString()+" " +
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
		else if( button == &btCancel)
		{
			SAM_LOG("Cancel change "+data.getType().toString()+" "+
                data[Ids::identifier].toString());
			op->returnVal = 2;
			op->closeButtonPressed();
		}
	}

    bool checkIfIdExists(const String& idStr)
    {
        return objController->checkIfIdExists(idStr);
    }
	virtual void readValues() = 0;
	virtual bool writeValues() = 0;

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
		addAndMakeVisible(&teMass);
		laMass.attachToComponent(&teMass, true);
		addAndMakeVisible(&tePos);
		laPos.attachToComponent(&tePos, true);
		addAndMakeVisible(&teVel);
		laVel.attachToComponent(&teVel, true);
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

		teMass.setBounds(80 , 40, getWidth() -90, 22);
		tePos.setBounds(80 , 70, getWidth() -90, 22);
		teVel.setBounds(80 , 100, getWidth() -90, 22);
		teLabels.setBounds(80, 130, getWidth() - 90, 22);
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
            if (objController->checkIfIdExists(newName))
            {
                return false;
            }
            else
            {
                if(! objController->renameId(oldName, newName))
                    return false;
            }
        }

        objController->changeObjectNameInLink(oldName, newName, undoManager);

        data.setProperty(Ids::identifier, newName, undoManager);
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

		teLabels.setBounds(80, 40, getWidth() - 90, 22);
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
            if (objController->checkIfIdExists(newName))
            {
                return false;
            }
            else
            {
                if(! objController->renameId(oldName, newName))
                    return false;
            }
        }

        objController->changeObjectNameInLink(oldName, newName, undoManager);

        data.setProperty(Ids::identifier, newName, undoManager);

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

// TODO: implement ResonatorPropertiesComponent
class ResonatorPropertiesComponent : public ObjectPropertiesComponent {
public:
	ResonatorPropertiesComponent(ObjectPropertiesPanel* op_,
                                 ObjController* objController_,
                                 ValueTree data_,
                                 UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_)
	{

		readValues();
	}
	virtual ~ResonatorPropertiesComponent()
	{

	}

	void resized()
	{
		ObjectPropertiesComponent::resized();
	}

	void readValues()
	{

	}

	bool writeValues()
	{
        return true;
	}

private:
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
		addAndMakeVisible(&teLabels);
		laLabels.attachToComponent(&teLabels, true);
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

		tePos.setBounds(80 , 40, getWidth() -90, 22);
		teLabels.setBounds(80, 70, getWidth() - 90, 22);
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
            if (objController->checkIfIdExists(newName))
            {
                return false;
            }
            else
            {
                if(! objController->renameId(oldName, newName))
                    return false;
            }
        }

        objController->changeObjectNameInLink(oldName, newName, undoManager);

        data.setProperty(Ids::identifier, newName, undoManager);
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
        teEndVertex("teEndVertex")
    {
        addAndMakeVisible(&teStiff);
		laStiff.attachToComponent(&teStiff, true);
		addAndMakeVisible(&teDamp);
		laDamp.attachToComponent(&teDamp, true);
		addAndMakeVisible(&tePos);
		laPos.attachToComponent(&tePos, true);
		addAndMakeVisible(&teLabels);
		laLabels.attachToComponent(&teLabels, true);
        teStartVertex.setReadOnly(true);
        addAndMakeVisible(&teStartVertex);
		laStartVertex.attachToComponent(&teStartVertex, true);
        teEndVertex.setReadOnly(true);
		addAndMakeVisible(&teEndVertex);
		laEndVertex.attachToComponent(&teEndVertex, true);

		readValues();
	}
	virtual ~LinkPropertiesComponent()
	{

    }

	void resized()
	{
		ObjectPropertiesComponent::resized();
        
        teStiff.setBounds(80 , 40, getWidth() -90, 22);
		teDamp.setBounds(80 , 70, getWidth() -90, 22);
		tePos.setBounds(80 , 100, getWidth() -90, 22);
		teLabels.setBounds(80, 130, getWidth() - 90, 22);
        teStartVertex.setBounds(80, 160, getWidth() - 90, 22);
        teEndVertex.setBounds(80, 190, getWidth() - 90, 22);
	}

	void readValues()
	{
		teName.setText(data[Ids::identifier].toString());
		teStiff.setText(data.getChildWithName(Ids::parameters).getChild(0)[Ids::value].toString());
		teDamp.setText(data.getChildWithName(Ids::parameters).getChild(1)[Ids::value].toString());
		tePos.setText(data.getChildWithName(Ids::parameters).getChild(2)[Ids::value].toString());
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
        String idName = teName.getText();
        if (objController->checkIfIdExists(idName))
            return false;

        data.setProperty(Ids::identifier, idName, undoManager);
		ValueTree paramsTree = data.getChildWithName(Ids::parameters);
        ValueTree pa1 = paramsTree.getChild(0);
        ValueTree pa2 = paramsTree.getChild(1);
        ValueTree pa3 = paramsTree.getChild(2);
        pa1.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(teStiff.getText()),
                        undoManager);
        pa2.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(teDamp.getText()),
                        undoManager);
        pa3.setProperty(Ids::value,
                        Utils::fixParameterValueIfNeeded(tePos.getText()),
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
        
        teSource.setBounds(80 , 40, getWidth() -90, 22);
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
//        teSource.setText(data[Ids::sources].toString());
	}

	bool writeValues()
    {
        String idName = teName.getText();
        if (objController->checkIfIdExists(idName))
            return false;

        data.setProperty(Ids::identifier, idName, undoManager);
        
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
        
//        data.setProperty(Ids::sources, teSource.getText(), undoManager);
        
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
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_)
	{

		readValues();
	}
	virtual ~WaveguidePropertiesComponent()
	{

	}

	void resized()
	{
		ObjectPropertiesComponent::resized();
	}

	void readValues()
	{

	}

	bool writeValues()
	{
        return true;
	}

private:
};

class TerminationPropertiesComponent : public ObjectPropertiesComponent {
public:
	TerminationPropertiesComponent(ObjectPropertiesPanel* op_,
                         ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_)
	{

		readValues();
	}
	virtual ~TerminationPropertiesComponent()
	{

	}

	void resized()
	{
		ObjectPropertiesComponent::resized();
	}

	void readValues()
	{

	}

	bool writeValues()
	{
        return true;
	}

private:
};

class JunctionPropertiesComponent : public ObjectPropertiesComponent {
public:
	JunctionPropertiesComponent(ObjectPropertiesPanel* op_,
                         ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, objController_, data_, undoManager_)
	{

		readValues();
	}
	virtual ~JunctionPropertiesComponent()
	{

	}

	void resized()
	{
		ObjectPropertiesComponent::resized();
	}

	void readValues()
	{

	}

	bool writeValues()
	{
        return true;
	}

private:
};

//==============================================================================
static String exportWindowPos;


ObjectPropertiesPanel::ObjectPropertiesPanel(ObjController* objController,
                                             BaseObjectComponent* caller, 
                                             UndoManager* undoManager_)
    : DialogWindow (Utils::getObjectTypeAsString(caller->getData().getType())+ " properties",
    		Colour::greyLevel (0.92f), true),
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

	comp->setSize (400, 300);
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
