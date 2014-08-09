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
    ObjectPropertiesComponent(ObjController& objController_,
                              const Array<ValueTree>& datas_,
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
    TextEditor* createEditor(const String& name, const String& text,
                             bool isMultiline, bool isReadOnly = false);

    static void readEditorsMultipleSelection(const Array<TextEditor*>& editors_,
                                             const Colour c = Colours::black);
    static void readEditorsSingleSelection(const Array<TextEditor*>& editors_,
                                           const StringArray& params,
                                           const Colour c = Colours::black);
    void writeEditors(ValueTree params, bool fixValues);
    StringArray getParamsStrings(const ValueTree& params);

    ObjController& objController;
	Array<ValueTree> datas;
    UndoManager* undoManager;
	Label* laName;
	TextEditor* teName;
    bool dataChanged;
    bool multipleEdit;
    bool isEditing;

    Array<TextEditor*> editors;
    HashMap<TextEditor*,bool> editorsModified;
    void resetEditorsModifiedState();

    bool nameExistAlertActive;
    bool canceledEditing;

    friend class PropertiesWindow;
};

class MassPropertiesComponent : public ObjectPropertiesComponent
{
public:
    MassPropertiesComponent(ObjController& objController_,
                            const Array<ValueTree>& datas_,
                            UndoManager* undoManager_);
	~MassPropertiesComponent();
	void resized();
	void readValues();
	bool writeValues();
};

class PortPropertiesComponent : public ObjectPropertiesComponent
{
public:
    PortPropertiesComponent(ObjController& objController_,
                            const Array<ValueTree>& datas_,
                            UndoManager* undoManager_);
    virtual ~PortPropertiesComponent();
	void resized();
	void readValues();
	bool writeValues();
};

class ResonatorPropertiesComponent : public ObjectPropertiesComponent {
public:
    ResonatorPropertiesComponent(ObjController& objController_,
                                 const Array<ValueTree>& datas_,
                                 UndoManager* undoManager_);
	virtual ~ResonatorPropertiesComponent();
	void resized();
	void readValues();
	bool writeValues();
};

class GroundPropertiesComponent : public ObjectPropertiesComponent {
public:
    GroundPropertiesComponent(ObjController& objController_,
                              const Array<ValueTree>& datas_,
                              UndoManager* undoManager_);
	virtual ~GroundPropertiesComponent();

	void resized();
	void readValues();
	bool writeValues();
};

class LinkPropertiesComponent : public ObjectPropertiesComponent {
public:
    LinkPropertiesComponent(ObjController& objController_,
                            const Array<ValueTree>& datas_,
                            UndoManager* undoManager_);
	virtual ~LinkPropertiesComponent();

	void resized();
	void readValues();
	bool writeValues();

private:
    Array<TextEditor*> vertices;
};

class AudiooutPropertiesComponent : public ObjectPropertiesComponent {
public:
    AudiooutPropertiesComponent(ObjController& objController_,
                                const Array<ValueTree>& datas_,
                                UndoManager* undoManager_);
	virtual ~AudiooutPropertiesComponent();
    void resized();
	void readValues();
	bool writeValues();
};

class WaveguidePropertiesComponent : public ObjectPropertiesComponent {
public:
    WaveguidePropertiesComponent(ObjController& objController_,
                                 const Array<ValueTree>& datas_,
                                 UndoManager* undoManager_);
	virtual ~WaveguidePropertiesComponent();
	void resized();
	void readValues();
	bool writeValues();
private:
    Array<TextEditor*> vertices;
};

class TerminationPropertiesComponent : public ObjectPropertiesComponent {
public:
    TerminationPropertiesComponent(ObjController& objController_,
                                   const Array<ValueTree>& datas_,
                                   UndoManager* undoManager_);
	virtual ~TerminationPropertiesComponent();
	void resized();
	void readValues();
	bool writeValues();
};

class JunctionPropertiesComponent : public ObjectPropertiesComponent {
public:
    JunctionPropertiesComponent(ObjController& objController_,
                                const Array<ValueTree>& datas_,
                                UndoManager* undoManager_);
	virtual ~JunctionPropertiesComponent();
	void resized();
	void readValues();
	bool writeValues();
};

class GainComponent : public Component,
                      public TextEditor::Listener
{
public:

    GainComponent(const StringArray& sourceIds_,
                  const Array<ValueTree>& datas_,
                  UndoManager* undoManager_);
    ~GainComponent();

    void resized();

    void textEditorTextChanged(TextEditor& editor);

    void textEditorReturnKeyPressed(TextEditor& editor);
    void textEditorEscapeKeyPressed(TextEditor& editor);

    void textEditorFocusLost(TextEditor& editor);

    void readValues();

    void applyEditing();
private:
    Label* laGain;
    TextEditor* teGain;
    StringArray sourceIds;
    Array<ValueTree> datas;
    UndoManager* undoManager;
    bool multipleEdit;
    bool dataChanged;
};
}

#endif  // __OBJECTPROPERTIESCOMPONENT_H_C1992334__
