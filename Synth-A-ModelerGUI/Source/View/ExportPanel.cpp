/*
  ==============================================================================

    ExportPanel.cpp
    Created: 22 Apr 2012 8:26:07pm
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

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Controller/MDLController.h"
#include "ExportPanel.h"
#include "../Utilities/StoredSettings.h"

//==============================================================================
class ExportComp  : public Component,
					public ButtonListener,
					public KeyListener
{
public:
    ExportComp(ExportPanel& exportPanel_)
    :	exportPanel(exportPanel_),
     	button2Faust2supercollider("SuperCollider"),
     	button2Faust2puredata("PureData"),
     	buttonOk("Ok"),
     	buttonCancel("Cancel")
    {
        addAndMakeVisible (&button2Faust2supercollider);
        addAndMakeVisible (&button2Faust2puredata);
        addAndMakeVisible (&buttonOk);
        buttonOk.addListener(this);
        addAndMakeVisible (&buttonCancel);
        buttonCancel.addListener(this);

        getExporters();
    }

    ~ExportComp()
    {
    }

    void resized()
    {
        button2Faust2supercollider.setBounds (10, 36, getWidth()-10, 22);
        button2Faust2puredata.setBounds (10, 56, getWidth()-10, 22);
        buttonOk.setBounds(getWidth()/2+10, getHeight()- 40, 60, 22);
        buttonCancel.setBounds(getWidth()/2-70, getHeight()- 40, 60, 22);
    }

    void buttonClicked(Button* btn)
    {
    	exportPanel.returnVal = 0;
    	if(btn == &buttonOk)
    	{
//    		String
    		DBG("Clicked Ok");
    		setExporters();
    		exportPanel.returnVal = 1;
    		exportPanel.closeButtonPressed();
    	}
    	else if(btn == &buttonCancel)
    	{
    		DBG("Clicked Cancel");
    		exportPanel.returnVal = 2;
    		exportPanel.closeButtonPressed();
    	}
    }

    bool keyPressed(const KeyPress& key, Component* comp)
    {
    	return false;
    }
private:

    void getExporters()
    {
    	String exportersStr = StoredSettings::getInstance()->getExporters();
    	StringArray exporters;
    	exporters.addTokens(exportersStr, "|", "\"");

		bool toggleState = exporters.indexOf("sc") != -1 ? true : false;
		button2Faust2supercollider.setToggleState(toggleState, false);
		toggleState = exporters.indexOf("pd") != -1 ? true : false;
		button2Faust2puredata.setToggleState(toggleState, false);
    }

    void setExporters()
    {
    	StringArray exporters;
    	if(button2Faust2supercollider.getToggleState())
    	{
    		exporters.add("sc");
    	}
    	if(button2Faust2puredata.getToggleState())
    	{
    		exporters.add("pd");
    	}
    	String exportersStr = exporters.joinIntoString("|");
    	StoredSettings::getInstance()->setExporters(exportersStr);
    }

    ExportPanel& exportPanel;
    ToggleButton button2Faust2supercollider;
    ToggleButton button2Faust2puredata;
    TextButton buttonOk;
    TextButton buttonCancel;
};
//==============================================================================
//static String exportWindowPos;


ExportPanel::ExportPanel()
    : DialogWindow ("Export externals", Colour::greyLevel (0.92f), true),
      returnVal(0)
{
    ExportComp* const p = new ExportComp(*this);
    p->setSize (250, 150);

    setContentOwned (p, true);

    centreAroundComponent (0, getWidth(), getHeight());

    setResizable (false, false);
    setVisible(true);
}

ExportPanel::~ExportPanel()
{
}

void ExportPanel::closeButtonPressed()
{
    setVisible (false);
}

int ExportPanel::show()
{
	ExportPanel ep;
    ep.runModalLoop();
    return ep.returnVal;
}
