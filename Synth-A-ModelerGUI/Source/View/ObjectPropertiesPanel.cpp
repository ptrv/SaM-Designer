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
#include "ObjectComponent.h"

#include "ObjectPropertiesPanel.h"

//==============================================================================

class ObjectPropertiesComponent : public Component,
								  public Button::Listener
{
public:
	ObjectPropertiesComponent(ObjectPropertiesPanel* op_,
			ValueTree data_, UndoManager* undoManager_)
	: op(op_),
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
        laDebug.setText("Pos: " + data[Ids::posX].toString() + String(" ") + data[Ids::posY].toString(),false);
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
			writeValues();
			SAM_LOG("Change "+data.getType().toString()+" "+data[Ids::identifier].toString());
			op->returnVal = 1;
			op->closeButtonPressed();
		}
		else if( button == &btCancel)
		{
			SAM_LOG("Cancel change "+data.getType().toString()+" "+data[Ids::identifier].toString());
			op->returnVal = 2;
			op->closeButtonPressed();
		}
	}

	virtual void readValues() = 0;
	virtual void writeValues() = 0;

protected:
	ObjectPropertiesPanel* op;
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
			ValueTree data_, UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, data_, undoManager_),
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
		teMass.setText(data.getChildWithName(Ids::parameters)[Ids::idx[0]].toString());
		tePos.setText(data.getChildWithName(Ids::parameters)[Ids::idx[1]].toString());
		teVel.setText(data.getChildWithName(Ids::parameters)[Ids::idx[2]].toString());
		String labelText;
		StringArray labelsArray;
		for (int i = 0; i < data.getChildWithName(Ids::labels).getNumProperties(); ++i) {
			labelsArray.add(data.getChildWithName(Ids::labels)[Ids::idx[i]].toString());
		}
		teLabels.setText(labelsArray.joinIntoString(","));
	}
	void writeValues()
	{
		data.setProperty(Ids::identifier, teName.getText(), undoManager);
		ValueTree paramsTree = data.getChildWithName(Ids::parameters);
		paramsTree.setProperty(Ids::idx[0], Utils::fixParameterValueIfNeeded(teMass.getText()), undoManager);
		paramsTree.setProperty(Ids::idx[1], Utils::fixParameterValueIfNeeded(tePos.getText()), undoManager);
		paramsTree.setProperty(Ids::idx[2], Utils::fixParameterValueIfNeeded(teVel.getText()), undoManager);
		ValueTree labelsTree = data.getChildWithName(Ids::labels);
		String labelsString = teLabels.getText();
		StringArray labelsArray;
		labelsArray.addTokens(labelsString, ",", "\"");
		for (int i = 0; i < labelsArray.size(); ++i) {
			labelsTree.setProperty(Ids::idx[i], labelsArray[i], undoManager);
		}
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
			ValueTree data_, UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, data_, undoManager_),
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
		for (int i = 0; i < data.getChildWithName(Ids::labels).getNumProperties(); ++i) {
			labelsArray.add(data.getChildWithName(Ids::labels)[Ids::idx[i]].toString());
		}
		teLabels.setText(labelsArray.joinIntoString(","));

	}

	void writeValues()
	{
		data.setProperty(Ids::identifier, teName.getText(), undoManager);
		ValueTree labelsTree = data.getChildWithName(Ids::labels);
		String labelsString = teLabels.getText();
		StringArray labelsArray;
		labelsArray.addTokens(labelsString, ",", "\"");
		for (int i = 0; i < labelsArray.size(); ++i) {
			labelsTree.setProperty(Ids::idx[i], labelsArray[i], undoManager);
		}

	}
	Label laLabels;
	TextEditor teLabels;

private:
};

// TODO: implement ResonatorPropertiesComponent
class ResonatorPropertiesComponent : public ObjectPropertiesComponent {
public:
	ResonatorPropertiesComponent(ObjectPropertiesPanel* op_,
	ValueTree data_, UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, data_, undoManager_)
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

	void writeValues()
	{

	}

private:
};

class GroundPropertiesComponent : public ObjectPropertiesComponent {
public:
	GroundPropertiesComponent(ObjectPropertiesPanel* op_,
	ValueTree data_, UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, data_, undoManager_),
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
		tePos.setText(data.getChildWithName(Ids::parameters)[Ids::idx[0]].toString());
		String labelText;
		StringArray labelsArray;
		for (int i = 0; i < data.getChildWithName(Ids::labels).getNumProperties(); ++i) {
			labelsArray.add(data.getChildWithName(Ids::labels)[Ids::idx[i]].toString());
		}
		teLabels.setText(labelsArray.joinIntoString(","));

	}

	void writeValues()
	{
		data.setProperty(Ids::identifier, teName.getText(), undoManager);
		ValueTree paramsTree = data.getChildWithName(Ids::parameters);
		paramsTree.setProperty(Ids::idx[0], tePos.getText(), undoManager);
		ValueTree labelsTree = data.getChildWithName(Ids::labels);
		String labelsString = teLabels.getText();
		StringArray labelsArray;
		labelsArray.addTokens(labelsString, ",", "\"");
		for (int i = 0; i < labelsArray.size(); ++i) {
			labelsTree.setProperty(Ids::idx[i], labelsArray[i], undoManager);
		}
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
	ValueTree data_, UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, data_, undoManager_),
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
		teStiff.setText(data.getChildWithName(Ids::parameters)[Ids::idx[0]].toString());
		teDamp.setText(data.getChildWithName(Ids::parameters)[Ids::idx[1]].toString());
		tePos.setText(data.getChildWithName(Ids::parameters)[Ids::idx[2]].toString());
		String labelText;
		StringArray labelsArray;
		for (int i = 0; i < data.getChildWithName(Ids::labels).getNumProperties(); ++i) {
			labelsArray.add(data.getChildWithName(Ids::labels)[Ids::idx[i]].toString());
		}
		teLabels.setText(labelsArray.joinIntoString(","));
        teStartVertex.setText(data[Ids::startVertex].toString());
        teEndVertex.setText(data[Ids::endVertex].toString());
	}

	void writeValues()
	{
        data.setProperty(Ids::identifier, teName.getText(), undoManager);
		ValueTree paramsTree = data.getChildWithName(Ids::parameters);
		paramsTree.setProperty(Ids::idx[0], Utils::fixParameterValueIfNeeded(teStiff.getText()), undoManager);
		paramsTree.setProperty(Ids::idx[1], Utils::fixParameterValueIfNeeded(teDamp.getText()), undoManager);
		paramsTree.setProperty(Ids::idx[2], Utils::fixParameterValueIfNeeded(tePos.getText()), undoManager);
		ValueTree labelsTree = data.getChildWithName(Ids::labels);
		String labelsString = teLabels.getText();
		StringArray labelsArray;
		labelsArray.addTokens(labelsString, ",", "\"");
		for (int i = 0; i < labelsArray.size(); ++i) {
			labelsTree.setProperty(Ids::idx[i], labelsArray[i], undoManager);
		}
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

//class TouchPropertiesComponent : public ObjectPropertiesComponent {
//public:
//	TouchPropertiesComponent(ObjectPropertiesPanel* op_,
//	ValueTree data_, UndoManager* undoManager_)
//	: ObjectPropertiesComponent(op_, data_, undoManager_)
//    {
//		
//        readValues();
//	}
//	virtual ~TouchPropertiesComponent()
//	{
//
//	}
//
//	void resized()
//	{
//		ObjectPropertiesComponent::resized();
//	}
//
//	void readValues()
//    {
//
//	}
//
//	void writeValues()
//    {
//
//	}
//
//private:
//};
//
//class PluckPropertiesComponent : public ObjectPropertiesComponent {
//public:
//	PluckPropertiesComponent(ObjectPropertiesPanel* op_,
//	ValueTree data_, UndoManager* undoManager_)
//	: ObjectPropertiesComponent(op_, data_, undoManager_)
//	{
//
//		readValues();
//	}
//	virtual ~PluckPropertiesComponent()
//	{
//
//	}
//
//	void resized()
//	{
//		ObjectPropertiesComponent::resized();
//	}
//
//	void readValues()
//	{
//
//	}
//
//	void writeValues()
//	{
//
//	}
//
//private:
//};

class AudiooutPropertiesComponent : public ObjectPropertiesComponent {
public:
	AudiooutPropertiesComponent(ObjectPropertiesPanel* op_,
	ValueTree data_, UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, data_, undoManager_),
        laSource("laSource", "Source"),
        teSource("teSource")
	{
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
        teSource.setText(data[Ids::sources].toString());
	}

	void writeValues()
    {
        data.setProperty(Ids::identifier, teName.getText(), undoManager);
        data.setProperty(Ids::sources, laSource.getText(), undoManager);
	}

private:
    Label laSource;
    TextEditor teSource;
};

class WaveguidePropertiesComponent : public ObjectPropertiesComponent {
public:
	WaveguidePropertiesComponent(ObjectPropertiesPanel* op_,
	ValueTree data_, UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, data_, undoManager_)
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

	void writeValues()
	{

	}

private:
};

class TerminationPropertiesComponent : public ObjectPropertiesComponent {
public:
	TerminationPropertiesComponent(ObjectPropertiesPanel* op_,
	ValueTree data_, UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, data_, undoManager_)
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

	void writeValues()
	{

	}

private:
};

class JunctionPropertiesComponent : public ObjectPropertiesComponent {
public:
	JunctionPropertiesComponent(ObjectPropertiesPanel* op_,
	ValueTree data_, UndoManager* undoManager_)
	: ObjectPropertiesComponent(op_, data_, undoManager_)
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

	void writeValues()
	{

	}

private:
};

//==============================================================================
static String exportWindowPos;


ObjectPropertiesPanel::ObjectPropertiesPanel(BaseObjectComponent* caller, UndoManager* undoManager_)
    : DialogWindow (Utils::getObjectTypeAsString(caller->getData().getType())+ " properties",
    		Colour::greyLevel (0.92f), true),
      returnVal(0)
{

	Component* comp;
	if(caller->getData().getType() == Ids::mass)
	{
		comp = new MassPropertiesComponent(this, caller->getData(), undoManager_);
	}
	else if(caller->getData().getType() == Ids::port)
	{
		comp = new PortPropertiesComponent(this, caller->getData(), undoManager_);
	}
	else if(caller->getData().getType() == Ids::resonator)
	{
		comp = new ResonatorPropertiesComponent(this, caller->getData(), undoManager_);
	}
	else if(caller->getData().getType() == Ids::ground)
	{
		comp = new GroundPropertiesComponent(this, caller->getData(), undoManager_);
	}
	else if(caller->getData().getType() == Ids::link
        || caller->getData().getType() == Ids::touch
        || caller->getData().getType() == Ids::pluck)
	{
		comp = new LinkPropertiesComponent(this, caller->getData(), undoManager_);
	}
//	else if(caller->getData().getType() == Ids::pluck)
//	{
//		comp = new LinkPropertiesComponent(this, caller->getData(), undoManager_);
//	}
//	else if(caller->getData().getType() == Ids::touch)
//	{
//		comp = new TouchPropertiesComponent(this, caller->getData(), undoManager_);
//	}
	else if(caller->getData().getType() == Ids::waveguide)
	{
		comp = new WaveguidePropertiesComponent(this, caller->getData(), undoManager_);
	}
	else if(caller->getData().getType() == Ids::junction)
	{
		comp = new JunctionPropertiesComponent(this, caller->getData(), undoManager_);
	}
	else if(caller->getData().getType() == Ids::termination)
	{
		comp = new TerminationPropertiesComponent(this, caller->getData(), undoManager_);
	}
	else if(caller->getData().getType() == Ids::audioout)
	{
		comp = new AudiooutPropertiesComponent(this, caller->getData(), undoManager_);
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

int ObjectPropertiesPanel::show(BaseObjectComponent* caller, UndoManager* undoManager_)
{
	ObjectPropertiesPanel ep(caller, undoManager_);
    ep.runModalLoop();
    return ep.returnVal;
}
