/*
  ==============================================================================

    ObjectPropertiesComponent.h
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

#ifndef __OBJECTPROPERTIESCOMPONENT_H_C1992334__
#define __OBJECTPROPERTIESCOMPONENT_H_C1992334__


namespace synthamodeler
{
class ObjectPropertiesComponent : public Component,
                                  public TextEditor::Listener
{
public:
	ObjectPropertiesComponent(ObjController* objController_,
                             ValueTree data_,
                             UndoManager* undoManager_);

	virtual ~ObjectPropertiesComponent();

	void resized();

	virtual void readValues() = 0;
	virtual bool writeValues() = 0;

    void textEditorTextChanged(TextEditor& editor);
    void textEditorReturnKeyPressed(TextEditor& editor);
    void textEditorEscapeKeyPressed(TextEditor& editor);
    void textEditorFocusLost(TextEditor& editor);
    void applyEditing();
    void cancelEditing();

protected:
    bool writeIdentifier();

    ObjController* objController;
	ValueTree data;
	UndoManager* undoManager;
	Label laName;
	TextEditor teName;
    Label laDebug;
    bool dataChanged;
};

class MassPropertiesComponent : public ObjectPropertiesComponent
{
public:
	MassPropertiesComponent(ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_);
	~MassPropertiesComponent();
	void resized();
	void readValues();
	bool writeValues();
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
	PortPropertiesComponent(ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_);
    virtual ~PortPropertiesComponent();
	void resized();
	void readValues();
	bool writeValues();
private:
};

class ResonatorPropertiesComponent : public ObjectPropertiesComponent {
public:
	ResonatorPropertiesComponent(ObjController* objController_,
                                 ValueTree data_,
                                 UndoManager* undoManager_);
	virtual ~ResonatorPropertiesComponent();
	void resized();
	void readValues();
	bool writeValues();
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
	GroundPropertiesComponent(ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_);
	virtual ~GroundPropertiesComponent();

	void resized();
	void readValues();
	bool writeValues();

private:
	Label laPos;
	TextEditor tePos;

};

class LinkPropertiesComponent : public ObjectPropertiesComponent {
public:
    LinkPropertiesComponent(ObjController* objController_,
                            ValueTree data_,
                            UndoManager* undoManager_);
	virtual ~LinkPropertiesComponent();

	void resized();
	void readValues();
	bool writeValues();

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
    Label laPulseMult;
    TextEditor tePulseMult;
    Label laPulseTau;
    TextEditor tePulseTau;
    Label laPulseLen;
    TextEditor tePulseLen;
};

class AudiooutPropertiesComponent : public ObjectPropertiesComponent {
public:
    AudiooutPropertiesComponent(ObjController* objController_,
                                ValueTree data_,
                                UndoManager* undoManager_);
	virtual ~AudiooutPropertiesComponent();
    void resized();
	void readValues();
	bool writeValues();
private:
    Label laSource;
    TextEditor teSource;
    Label laOpt;
    TextEditor teOpt;
};

class WaveguidePropertiesComponent : public ObjectPropertiesComponent {
public:
	WaveguidePropertiesComponent(ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_);
	virtual ~WaveguidePropertiesComponent();
	void resized();
	void readValues();
	bool writeValues();
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
	TerminationPropertiesComponent(ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_);
	virtual ~TerminationPropertiesComponent();
	void resized();
	void readValues();
	bool writeValues();

private:
	Label laTermType;
	TextEditor teTermType;
};

class JunctionPropertiesComponent : public ObjectPropertiesComponent {
public:
	JunctionPropertiesComponent(ObjController* objController_,
                         ValueTree data_,
                         UndoManager* undoManager_);
	virtual ~JunctionPropertiesComponent();
	void resized();
	void readValues();
	bool writeValues();

private:
	Label laPos;
	TextEditor tePos;
};

}

#endif  // __OBJECTPROPERTIESCOMPONENT_H_C1992334__
