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

#include "VariablesPanel.h"

//class VariableInputPanel;
class VariableInputPanel : public DialogWindow
{
public:
	VariableInputPanel(ValueTree data_, bool edit_)
	: DialogWindow("Variable", Colours::lightgrey, true)
	{
		VariableInputComponent* const vic = new VariableInputComponent(*this, data_, edit_);
		vic->setSize(350, 100);

		setContentOwned (vic, true);

		centreAroundComponent (0, getWidth(), getHeight());

		setResizable (false, false);
		setVisible(true);

	}

	~VariableInputPanel()
	{

	}
    void closeButtonPressed()
    {
    	setVisible(false);
    }

    static void show(ValueTree data, bool edit)
    {
    	VariableInputPanel vip(data, edit);
        vip.runModalLoop();
    }

private:
    class VariableInputComponent : public Component,
    								public Button::Listener
    {
    public:
    	VariableInputComponent(VariableInputPanel& parent_, ValueTree data_, bool edit_)
    	: parent(parent_),
    	  labelVarName("Variable"),
    	  labelVarValue("Value"),
    	  inputVarName("Input variable"),
    	  inputVarValue("Input value"),
    	  data(data_),
    	  edit(edit_),
    	  btOk("Ok"),
    	  btCancel("Cancel")
    	{
    		labelVarName.setText("Variable", false);
    		addAndMakeVisible(&labelVarName);
    		labelVarValue.setText("Value", false);
    		addAndMakeVisible(&labelVarValue);
    		addAndMakeVisible(&inputVarName);
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
				ValueTree newData(Ids::label);
				newData.setProperty(Ids::identifier, inputVarName.getText(), nullptr);
				newData.setProperty(Ids::faustCode, inputVarValue.getText(), nullptr);
    			if(edit)
    			{
					data.setProperty(Ids::identifier, newData[Ids::identifier].toString(), nullptr);
					data.setProperty(Ids::faustCode, newData[Ids::faustCode].toString(), nullptr);
    			}
    			else
    			{
					data.addChild(newData, -1, nullptr);
    			}
				parent.closeButtonPressed();
    		}
    		else if(button == &btCancel)
    		{
    			parent.closeButtonPressed();
    		}
    	}
    private:
    	VariableInputPanel& parent;
    	Label labelVarName;
    	Label labelVarValue;
    	TextEditor inputVarName;
    	TextEditor inputVarValue;
    	ValueTree data;
    	bool edit;
    	TextButton btOk;
    	TextButton btCancel;
    };

};

class VariablesTable :public Component,
					public TableListBoxModel
{
public:
	VariablesTable(ValueTree data_)
	: table("VariablesTable", this),
		data(data_)
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
		return data.getChildWithName(Objects::labels).getNumChildren();
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
		String varName = data.getChildWithName(Objects::labels).getChild(rowNumber)[Ids::identifier].toString();
		String varValue = data.getChildWithName(Objects::labels).getChild(rowNumber)[Ids::faustCode].toString();
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

		VariableInputPanel::show(data.getOrCreateChildWithName(Objects::labels, nullptr), false);
		table.updateContent();
		SAM_LOG(data[Ids::mdlName].toString() + ": Add variable");
	}

	void editRow()
	{
		int rowIndex = table.getSelectedRow();
		if(rowIndex >= 0)
		{
			ValueTree editData = data.getChildWithName(Objects::labels).getChild(rowIndex);
			VariableInputPanel::show(editData, true);
			table.updateContent();
			table.repaint();
			SAM_LOG(data[Ids::mdlName].toString() + ": Edit variable " + editData[Ids::identifier].toString());
		}
	}

	void removeSelectedRow()
	{
		int rowIndex = table.getLastRowSelected();
		if(rowIndex >= 0)
		{
			String varName = data.getChildWithName(Objects::labels).getChild(rowIndex)[Ids::identifier].toString();
			data.getChildWithName(Objects::labels).removeChild(rowIndex,nullptr);
			table.updateContent();
			SAM_LOG(data[Ids::mdlName].toString() + ": Remove variable " + varName);
		}
	}
	ValueTree getData() { return data; }
private:
	bool checkVariableCorrectness(const String& varText)
	{
		return true;
	}
	TableListBox table;
	ValueTree data;
};

class VariablesComponent : public Component,
							public Button::Listener
{
public:
	VariablesComponent(ValueTree data)
	: btAdd("Add"),
	  btRemove("Remove"),
	  btEdit("Edit"),
	  varTable(data)
	{
		btAdd.addListener(this);
		addAndMakeVisible(&btAdd);
		btRemove.addListener(this);
		addAndMakeVisible(&btRemove);
		btEdit.addListener(this);
		addAndMakeVisible(&btEdit);
		addAndMakeVisible(&varTable);
	}

	~VariablesComponent()
	{
	}

	void resized()
	{
		btAdd.setBounds(5, 5, 70, 22);
		btRemove.setBounds(getWidth()- 75, 5, 70, 22);
		btEdit.setBounds(getWidth() / 2 - 35, 5, 70, 22);
		varTable.setBounds(5, 60, getWidth()-10, getHeight() - 60);
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
		else if( button == &btEdit)
		{
			varTable.editRow();
		}
	}

private:
	TextButton btAdd;
	TextButton btRemove;
	TextButton btEdit;
	VariablesTable varTable;
};

static String variablesWindowPos;


VariablesPanel::VariablesPanel(ValueTree data)
    : DialogWindow ("Define variables", Colour::greyLevel (0.92f), true)
{
	VariablesComponent* const vc = new VariablesComponent(data);
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

void VariablesPanel::show(ValueTree data)
{
	VariablesPanel vp(data);
    vp.runModalLoop();
}


