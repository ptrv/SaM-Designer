/*
  ==============================================================================

    VariablesPanel.cpp
    Created: 15 May 2012 7:50:53pm
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
#include "../Utilities/IdManager.h"

#include "VariablesPanel.h"

using namespace synthamodeler;
//class VariableInputPanel;
class VariableInputPanel : public DialogWindow
{
public:
	VariableInputPanel(ObjController* objController_,
                       ValueTree data_, bool edit_, UndoManager* undoManager_)
	: DialogWindow("Variable", Colours::lightgrey, true),
	  returnVal(0)
	{
		VariableInputComponent* const vic = new VariableInputComponent(*this,
				objController_, data_, edit_, undoManager_);
		vic->setSize(350, 100);

		setContentOwned (vic, true);

		centreAroundComponent (0, getWidth(), getHeight());

		setResizable (false, false);
	}

	~VariableInputPanel()
	{

	}
    void closeButtonPressed()
    {
    	setVisible(false);
    }

    static int show(ObjController* objController_,
                    ValueTree data, bool edit, UndoManager* undoManager)
    {
    	VariableInputPanel vip(objController_, data, edit, undoManager);
        vip.runModalLoop();
        return vip.returnVal;
    }

private:
	int returnVal;
    class VariableInputComponent : public Component,
                                   public Button::Listener,
                                   public TextEditor::Listener
    {
    public:
    	VariableInputComponent(VariableInputPanel& parent_,
                               ObjController* objController_,
                               ValueTree data_,
                               bool edit_,
                               UndoManager* undoManager_)
    	: parent(parent_),
          objController(objController_),
    	  labelVarName("Variable"),
    	  labelVarValue("Value"),
    	  inputVarName("Input variable"),
    	  inputVarValue("Input value"),
    	  data(data_),
    	  edit(edit_),
    	  btOk("Ok"),
    	  btCancel("Cancel"),
    	  undoManager(undoManager_),
          dataChanged(false)
    	{
    		labelVarName.setText("Variable", false);
    		addAndMakeVisible(&labelVarName);
    		labelVarValue.setText("Value", false);
    		addAndMakeVisible(&labelVarValue);
            inputVarName.addListener(this);
    		addAndMakeVisible(&inputVarName);
            inputVarValue.addListener(this);
    		addAndMakeVisible(&inputVarValue);
    		btOk.addListener(this);
    		addAndMakeVisible(&btOk);
    		btCancel.addListener(this);
    		addAndMakeVisible(&btCancel);

    		if(edit)
    		{
    			inputVarName.setText(data[Ids::identifier].toString());
    			inputVarValue.setText(data[Ids::faustCode].toString());
    		}
    	}

    	~VariableInputComponent()
    	{
    	}

    	void resized()
    	{
    		labelVarName.setBounds(0, 5, 60, 22);
    		inputVarName.setBounds(60, 5, getWidth() - 65, 22);
    		labelVarValue.setBounds(0, 40, 60, 22);
    		inputVarValue.setBounds(60, 40, getWidth() - 65, 22);
    		btOk.setBounds(getWidth()/2 - 65, getHeight() - 30, 60, 22);
    		btCancel.setBounds(getWidth()/2 + 5, getHeight() - 30, 60, 22);
    	}

    	void buttonClicked(Button* button)
    	{
    		if(button == &btOk)
    		{
                applyEdits();
    		}
    		else if(button == &btCancel)
    		{
                cancelEdits();
    		}
    	}

        void applyEdits()
        {
            if(! dataChanged)
                return;

            ValueTree newData(Ids::variable);
            newData.setProperty(Ids::identifier, inputVarName.getText(), nullptr);
            newData.setProperty(Ids::faustCode, inputVarValue.getText(), nullptr);
            if(data[Ids::identifier] != newData[Ids::identifier])
            {
                if(objController->checkIfIdExists(data.getType(), inputVarName.getText()))
                {
                    Alerts::nameAlreadyExists();
                    return;
                }
            }

            if (edit)
            {
                undoManager->beginNewTransaction("Edit variable");
                String oldName = data[Ids::identifier].toString();
                data.setProperty(Ids::identifier,
                                 newData[Ids::identifier].toString(),
                                 undoManager);
                data.setProperty(Ids::faustCode,
                                 newData[Ids::faustCode].toString(),
                                 undoManager);
                objController->getIdManager()->renameId(data.getType(),
                                                        oldName,
                                                        data[Ids::identifier].toString(),
                                                        undoManager);
                DBG(undoManager->getNumActionsInCurrentTransaction());
            }
            else
            {
                undoManager->beginNewTransaction("Add variable");
                //                    data.addChild(newData, -1, undoManager);
                data.setProperty(Ids::identifier,
                                 newData[Ids::identifier].toString(),
                                 undoManager);
                data.setProperty(Ids::faustCode,
                                 newData[Ids::faustCode].toString(),
                                 undoManager);
                objController->getIdManager()->addId(newData.getType(),
                                                     newData[Ids::identifier].toString(),
                                                     undoManager);
            }
            parent.returnVal = 1;
            parent.closeButtonPressed();
        }

        void cancelEdits()
        {
            parent.returnVal = 2;
            parent.closeButtonPressed();
        }

        void textEditorTextChanged(TextEditor& editor)
        {
            dataChanged = true;
        }
        void textEditorReturnKeyPressed(TextEditor& editor)
        {
            applyEdits();
        }
        void textEditorEscapeKeyPressed(TextEditor& editor)
        {
            cancelEdits();
        }

    private:
    	VariableInputPanel& parent;
        ObjController* objController;
    	Label labelVarName;
    	Label labelVarValue;
    	TextEditor inputVarName;
    	TextEditor inputVarValue;
    	ValueTree data;
    	bool edit;
    	TextButton btOk;
    	TextButton btCancel;
    	UndoManager* undoManager;
        bool dataChanged;

    };

};

class VariablesTable :public Component,
					public TableListBoxModel
{
public:
	VariablesTable(ObjController* objController_, ValueTree data_, UndoManager* undoManager_)
	: table("VariablesTable", this),
        objController(objController_),
		data(data_),
        undoManager(undoManager_)
	{
	    table.setColour (ListBox::outlineColourId, Colours::grey);
	    table.setOutlineThickness (1);

	    table.getHeader().addColumn("variable",1,200);
	    table.getHeader().addColumn("value",2,380);

	    table.setMultipleSelectionEnabled(false);
		addAndMakeVisible(&table);

	}
	~VariablesTable()
	{

	}

	void resized()
	{
		table.setBounds(0, 0, getWidth(), getHeight());
	}
	int getNumRows()
	{
		return data.getChildWithName(Objects::variables).getNumChildren();
	}

	void paintRowBackground(Graphics& g, int rowNumber, int width, int height,
			bool rowIsSelected)
	{
		if(rowIsSelected)
			g.fillAll(Colours::lightseagreen);
	}

	void paintCell(Graphics& g, int rowNumber, int columnId, int width,
			int height, bool rowIsSelected)
	{
		g.setColour(Colours::black);
		String varName = data.getChildWithName(Objects::variables).getChild(rowNumber)[Ids::identifier].toString();
		String varValue = data.getChildWithName(Objects::variables).getChild(rowNumber)[Ids::faustCode].toString();
		if(columnId == 1)
		{
			g.drawText(varName, 2, 0, width - 4, height, Justification::left, true);
		}
		else if(columnId == 2)
		{
			g.drawText(varValue, 2, 0, width -4, height, Justification::left, true);
		}
	}

	void addRow()
	{
        ValueTree var(Ids::variable);
		int r = VariableInputPanel::show(objController, var, false, undoManager);
		if(r == 1)
        {
			SAM_LOG(data[Ids::mdlName].toString() + ": Add variable");
            ValueTree variables = data.getOrCreateChildWithName(Objects::variables,
                                                                undoManager);
            variables.addChild(var, -1, undoManager);
        }
		else if(r == 2)
        {
			SAM_LOG(data[Ids::mdlName].toString() + ": Canceled add variable");
        }
		table.updateContent();
	}

	void removeSelectedRow()
	{
		int rowIndex = table.getLastRowSelected();
		if(rowIndex >= 0)
		{
            String varName = data.getChildWithName(Objects::variables).getChild(rowIndex)[Ids::identifier].toString();
			undoManager->beginNewTransaction("Remove variable");
            objController->getIdManager()->removeId(data.getChildWithName(Objects::variables).getChild(rowIndex).getType(),
                                                    varName, undoManager);
            data.getChildWithName(Objects::variables).removeChild(rowIndex, undoManager);

			table.updateContent();
			SAM_LOG(data[Ids::mdlName].toString() + ": Remove variable " + varName);
		}
	}
    void cellDoubleClicked (int rowNumber, int columnId, const MouseEvent& e)
    {
        ValueTree editData = data.getChildWithName(Objects::variables).getChild(rowNumber);
        int r = VariableInputPanel::show(objController, editData, true, undoManager);
        table.updateContent();
        table.repaintRow(rowNumber);
        if (r == 1)
            SAM_LOG(data[Ids::mdlName].toString() + ": Edit variable " + editData[Ids::identifier].toString());
        else if (r == 2)
            SAM_LOG(data[Ids::mdlName].toString() + ": Canceled edit variable");
    }
	ValueTree getData() { return data; }
private:
	bool checkVariableCorrectness(const String& varText)
	{
		return true;
	}
	TableListBox table;
    ObjController* objController;
	ValueTree data;
    UndoManager* undoManager;
};

class VariablesComponent : public Component,
							public Button::Listener
{
public:
    VariablesComponent(ObjController* objController,
                       ValueTree data,
                       UndoManager* undoManager_)
	: btAdd("+"),
	  btRemove("-"),
      btHelp("?"),
	  varTable(objController, data, undoManager_)
	{
		btAdd.addListener(this);
		addAndMakeVisible(&btAdd);
		btRemove.addListener(this);
		addAndMakeVisible(&btRemove);
        btHelp.addListener(this);
        addAndMakeVisible(&btHelp);
		addAndMakeVisible(&varTable);
	}

	~VariablesComponent()
	{
	}

	void resized()
	{
		varTable.setBounds(5, 5, getWidth()-10, getHeight() - 40);
		btAdd.setBounds(5, getHeight() - 30, 22, 22);
		btRemove.setBounds(30, getHeight() - 30, 22, 22);
        btHelp.setBounds(getWidth() - 30, getHeight() - 30, 22, 22);
	}

	void buttonClicked(Button* button)
	{
		if(button == &btAdd)
		{
			varTable.addRow();
		}
		else if( button == &btRemove)
		{
			varTable.removeSelectedRow();
		}
        else if( button == &btHelp)
        {
//            Utils::openHelpUrl("variables");
            HintComponent* content = new HintComponent();
            content->setSize(550, 50);

            CallOutBox& callout = CallOutBox::launchAsynchronously(content, button->getScreenBounds(), nullptr);
//            Rectangle<int> r = content->getScreenBounds();
//            r.setX(getScreenBounds().getRight()- 550);
//            r.setY(r.getY() - 200);
//            callout.updatePosition(button->getScreenBounds(), r);
        }
	}

private:
	TextButton btAdd;
	TextButton btRemove;
    TextButton btHelp;
	VariablesTable varTable;

    class HintComponent : public Component{
    public:
        HintComponent()
        {

        }
        ~HintComponent()
        {

        }
        void paint(Graphics& g)
        {
//            g.fillAll(Colours::white);
            g.setColour(Colours::white);
            g.drawText("Example:",0, 0, getWidth(), 20,Justification::left, false);
            g.drawMultiLineText("hslider(\"DescriptiveLabelForVariable\",defaultValue,minimumValue,maximumValue,stepSize)",
                       0, 30, 550);
        }
    private:

    };

};

static String variablesWindowPos;


VariablesPanel::VariablesPanel(ObjController* objController,
                               ValueTree data,
                               UndoManager* undoManager)
    : DialogWindow ("Define variables for "+data[Ids::mdlName].toString(),
                    Colour::greyLevel (0.92f), true)
{
    VariablesComponent * const vc = new VariablesComponent(objController,
                                                           data,
                                                           undoManager);
    vc->setSize (600, 400);

    setContentOwned (vc, true);

    if (! restoreWindowStateFromString (variablesWindowPos))
        centreWithSize(getWidth(), getHeight());

    setResizable (false, false);
    setVisible(true);
}

VariablesPanel::~VariablesPanel()
{
}

void VariablesPanel::closeButtonPressed()
{
    setVisible (false);
}

void VariablesPanel::show(ObjController* objController,
                          ValueTree data,
                          UndoManager* undoManager)
{
	VariablesPanel vp(objController, data, undoManager);
    vp.runModalLoop();
}