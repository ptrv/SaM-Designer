/*
  ==============================================================================

    PrefsPanel.cpp
    Created: 19 Apr 2012 2:25:09pm
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
#include "../Utilities/StoredSettings.h"
#include "PrefsMiscPage.h"

#include "PrefsPanel.h"


using namespace synthamodeler;
//==============================================================================
class ExporterInputPanel : public DialogWindow
{
public:
	ExporterInputPanel(int exporterIndex)
    : DialogWindow(TRANS("Exporter"), Colours::lightgrey, true)
	{
		ExporterInputComponent* const eic = new ExporterInputComponent(*this, exporterIndex);
		eic->setSize(350, 200);

		setContentOwned (eic, true);

        restoreWindowStateFromString (StoredSettings::getInstance()->getProps()
                                      .getValue ("lastPrefsPanelExporterInputPos"));

		centreAroundComponent (0, getWidth(), getHeight());

		setResizable (true, false);
    }

	~ExporterInputPanel()
	{
        StoredSettings::getInstance()->getProps()
            .setValue ("lastPrefsPanelExporterInputPos", getWindowStateAsString());
    }
    void closeButtonPressed()
    {
    	setVisible(false);
    }

    static void show(int exporterIndex)
    {
    	ExporterInputPanel eip(exporterIndex);
        eip.runModalLoop();
    }

private:
    class ExporterInputComponent : public Component,
                                   public Button::Listener,
                                   public TextEditor::Listener
    {
    public:
    	ExporterInputComponent(ExporterInputPanel& parent_, int exporterIndex_)
    	: parent(parent_),
          exporterIndex(exporterIndex_)
        {
            addAndMakeVisible(laVarName = new Label("Exporter"));
            laVarName->setText(TRANS("Exporter"), dontSendNotification);

            addAndMakeVisible(laVarValue = new Label("Command"));
            laVarValue->setText(TRANS("Command"), dontSendNotification);

            addAndMakeVisible(teVarName = new TextEditor(TRANS("Input Variable")));
            teVarName->addListener(this);

            addAndMakeVisible(teVarValue = new TextEditor(TRANS("Input Value")));
            teVarValue->addListener(this);
            teVarValue->setMultiLine(true);

            addAndMakeVisible(btOk = new TextButton("Ok"));
            btOk->addListener(this);

            addAndMakeVisible(btCancel = new TextButton(TRANS("Cancel")));
            btCancel->addListener(this);

            if(exporterIndex >= 0)
            {
                StringPairArray spa = StoredSettings::getInstance()->getExporters().getAllProperties();
                teVarName->setText(spa.getAllKeys()[exporterIndex], false);
                teVarValue->setText(spa.getAllValues()[exporterIndex], false);
            }

    	}

    	~ExporterInputComponent()
    	{
            laVarName = nullptr;
            laVarValue = nullptr;
            teVarName = nullptr;
            teVarValue = nullptr;
            btOk = nullptr;
            btCancel = nullptr;
    	}

    	void resized()
    	{
            laVarName->setBounds(0, 5, 60, 22);
            teVarName->setBounds(60, 5, getWidth() - 65, 22);
            laVarValue->setBounds(0, 40, 60, 22);
            teVarValue->setBounds(60, 40, getWidth() - 65, getHeight()-78);
            btOk->setBounds(getWidth()/2 - 65, getHeight() - 30, 60, 22);
            btCancel->setBounds(getWidth()/2 + 5, getHeight() - 30, 60, 22);
    	}

    	void buttonClicked(Button* button)
    	{
            if(button == btOk)
    		{
                applyEdits();
    		}
            else if(button == btCancel)
    		{
                cancelEdits();
    		}
    	}

        void applyEdits()
        {
            StringPairArray& spa = StoredSettings::getInstance()->getExporters().getAllProperties();
            if(exporterIndex >= 0)
            {
                // we have to copy the array first to keep order when editing a value.
                StringPairArray other;
                for (int i = 0; i < spa.size(); ++i)
                {
                    if(i == exporterIndex)
                    {
                        String currExporter = spa.getAllKeys()[i];
                        if(currExporter.compare(StoredSettings::getInstance()->getCurrentExporter()) == 0)
                            StoredSettings::getInstance()->setCurrentExporter(teVarName->getText());
                        other.set(teVarName->getText(), teVarValue->getText());
                    }
                    else
                    {
                        other.set(spa.getAllKeys()[i], spa.getAllValues()[i]);
                    }
                }

                spa.clear();
                spa.addArray(other);
            }
            else
            {
                spa.set(teVarName->getText(), teVarValue->getText());
            }
            StoredSettings::getInstance()->getExporters().save();
            parent.closeButtonPressed();
        }

        void cancelEdits()
        {
            parent.closeButtonPressed();
        }

        void textEditorTextChanged(TextEditor& /*editor*/)
        {
        }
        void textEditorReturnKeyPressed(TextEditor& /*editor*/)
        {
            applyEdits();
        }
        void textEditorEscapeKeyPressed(TextEditor& /*editor*/)
        {
            cancelEdits();
        }

    private:
    	ExporterInputPanel& parent;
        ScopedPointer<Label> laVarName;
        ScopedPointer<Label> laVarValue;
        ScopedPointer<TextEditor> teVarName;
        ScopedPointer<TextEditor> teVarValue;
        ScopedPointer<TextButton> btOk;
        ScopedPointer<TextButton> btCancel;
        int exporterIndex;
    };

};


class ExporterTable : public Component,
                      public TableListBoxModel
{
public:
    ExporterTable()
    : font(14.0f)
    {
        data = &StoredSettings::getInstance()->getExporters().getAllProperties();

		addAndMakeVisible(table = new TableListBox("Exporter table", this));
        table->setColour (ListBox::outlineColourId, Colours::grey);
	    table->setOutlineThickness (1);

        table->getHeader().addColumn(TRANS("Exporter"), 1, 100);

        table->getHeader().addColumn(TRANS("Command"), 2, 500);
        table->autoSizeAllColumns();
	    table->setMultipleSelectionEnabled(false);
    }

    void resized()
	{
		table->setBounds(0, 0, getWidth(), getHeight());
	}
	int getNumRows()
	{
		return data->size();
	}
    void paintRowBackground(Graphics& g, int /*rowNumber*/, int /*width*/, int /*height*/,
			bool rowIsSelected)
	{
		if(rowIsSelected)
			g.fillAll(Colours::lightseagreen);
	}

	void paintCell(Graphics& g, int rowNumber, int columnId, int width,
			int height, bool /*rowIsSelected*/)
	{
		g.setColour(Colours::black);
        g.setFont(font);
        StringArray keys = data->getAllKeys();
		String varName = keys[rowNumber];
		String varValue = data->getValue(varName, "");
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
        ExporterInputPanel::show(-1);
        table->updateContent();
        table->autoSizeAllColumns();
	}

	void editRow()
	{
		int rowIndex = table->getSelectedRow();
		if(rowIndex >= 0)
		{
            ExporterInputPanel::show(rowIndex);
			table->updateContent();
			table->repaintRow(rowIndex);
            table->autoSizeAllColumns();
		}
	}

	void removeSelectedRow()
	{
		int rowIndex = table->getLastRowSelected();
		if(rowIndex >= 0)
		{
            StringPairArray& spa = StoredSettings::getInstance()->getExporters().getAllProperties();
            String currentExporter = StoredSettings::getInstance()->getCurrentExporter();
            if(currentExporter.compare(spa.getAllKeys()[rowIndex]) == 0)
                StoredSettings::getInstance()->setCurrentExporter(String::empty);
            StoredSettings::getInstance()->getExporters().removeValue(spa.getAllKeys()[rowIndex]);

			table->updateContent();
            table->autoSizeAllColumns();
		}
	}
    void cellDoubleClicked (int rowNumber, int /*columnId*/, const MouseEvent& /*e*/)
    {
        ExporterInputPanel::show(rowNumber);
        table->updateContent();
        table->repaintRow(rowNumber);
        table->autoSizeAllColumns();
    }

    int getColumnAutoSizeWidth(int columnid)
    {
        if(columnid == 2 || columnid == 1)
        {
            int colWidth = 0;
            for (int i = 0; i < data->size(); ++i)
            {
                int dl = 0;
                if(columnid == 1)
                {
                    dl = font.getStringWidth(data->getAllKeys()[i]);
                }
                else if(columnid == 2)
                {
                    dl = font.getStringWidth(data->getAllValues()[i]);
                }
                if (colWidth < dl)
                    colWidth = dl;
            }
            return colWidth+10;
        }
        return 0;
    }
private:
    ScopedPointer<TableListBox> table;
    Font font;
    StringPairArray* data;
};
class ExporterPage : public Component,
                     public Button::Listener
{
public:
    ExporterPage()
    {
        addAndMakeVisible(btAdd = new TextButton("+"));
        btAdd->addListener(this);

        addAndMakeVisible(btRemove = new TextButton("-"));
        btRemove->addListener(this);

        addAndMakeVisible(exporterTable = new ExporterTable());
    }
    ~ExporterPage()
    {
        btAdd = nullptr;
        btRemove = nullptr;
        exporterTable = nullptr;
    }

    void resized()
    {
        exporterTable->setBounds(5, 10, getWidth() - 20, getHeight() - 50);
        btAdd->setBounds(5, getHeight() - 30, 22, 22);
		btRemove->setBounds(30, getHeight() - 30, 22, 22);
    }

    void buttonClicked(Button* button)
	{
        if(button == btAdd)
		{
			exporterTable->addRow();
		}
		else if( button == btRemove)
		{
			exporterTable->removeSelectedRow();
		}
    }
private:
    ScopedPointer<TextButton> btAdd;
    ScopedPointer<TextButton> btRemove;
    ScopedPointer<ExporterTable> exporterTable;
};

//==============================================================================
class AboutPage   : public Component
{
public:
    AboutPage()
    : logo(ImageCache::getFromMemory(BinaryData::synthamodeler_icon_png,
                                     BinaryData::synthamodeler_icon_pngSize))
    {
        text1.setJustification (Justification::centredTop);
        String infoTxt(BinaryData::about_txt);
        text1.append (infoTxt, Font (13.0f));

        text2.setJustification (Justification::centred);
        String buildDate = " (Build: " + String(__DATE__) + " " + String(__TIME__) + ")";
        text2.append ("Synth-A-Modeler-Designer v" + JUCEApplication::getInstance()->getApplicationVersion()
                        + buildDate + ", " + SystemStats::getJUCEVersion(), Font (12.0f, Font::bold));

        addAndMakeVisible (link = new HyperlinkButton("https://github.com/ptrv/SaM-Designer",
                                                      URL("https://github.com/ptrv/SaM-Designer")));
        link->setFont (Font (10.0f, Font::bold | Font::underlined), true);
    }

    ~AboutPage()
    {
        link = nullptr;
    }

    void paint (Graphics& g)
    {
        g.fillAll (Colour (0xffebebeb));
        g.drawImageWithin (logo, 0, 4, getWidth(), getHeight()*0.3,
                           RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize,
                           false);

        text1.draw (g, Rectangle<int> (12, getHeight()*0.4, getWidth() - 24, 100).toFloat());
        text2.draw (g, Rectangle<int> (12, getHeight() - 50, getWidth() - 24, 20).toFloat());
    }

    void resized()
    {
        link->setSize (100, 22);
        link->changeWidthToFitText();
        link->setTopLeftPosition ((getWidth() - link->getWidth()) / 2,
                                  getHeight() - link->getHeight() - 10);
    }

private:
    ScopedPointer<HyperlinkButton> link;
    Image logo;
    AttributedString text1, text2;
};


//==============================================================================
static const String miscPage = TRANS("Misc");
static const String exporterPage = TRANS("Exporter");
static const String aboutPage = TRANS("About");

class PrefsTabComp  : public PreferencesPanel
{
public:
    PrefsTabComp()
    {
        addSettingsPage (miscPage,
                         BinaryData::prefs_misc_png,
                         BinaryData::prefs_misc_pngSize);
        addSettingsPage (exporterPage,
                         BinaryData::prefs_export_png,
                         BinaryData::prefs_export_pngSize);
        addSettingsPage (aboutPage,
                         BinaryData::prefs_about_png,
                         BinaryData::prefs_about_pngSize);
    }

    ~PrefsTabComp()
    {
        StoredSettings::getInstance()->flush();
    }

    Component* createComponentForPage (const String& pageName)
    {
    	if (pageName == miscPage)
        {
            return new MiscPage();
        }
        else if (pageName == aboutPage)
        {
            return new AboutPage();
        }
        else if (pageName == exporterPage)
        {
            return new ExporterPage();
        }

        return new Component();
    }
};


//==============================================================================
//static String prefsWindowPos;


PrefsPanel::PrefsPanel()
: DialogWindow(TRANS("Synth-A-Modeler-Designer Preferences"),
                    Colour::greyLevel (0.92f), true)
{
    PrefsTabComp* const p = new PrefsTabComp();
    p->setSize (456, 520);

    setContentOwned (p, true);

    const String prefsWindowPos = StoredSettings::getInstance()->getProps()
                                    .getValue ("lastPrefsPanelWindowPos");
    restoreWindowStateFromString (prefsWindowPos);

    centreWithSize(getWidth(), getHeight());
    setResizable (true, true);
    setResizeLimits (400, 400, 1000, 800);
}

PrefsPanel::~PrefsPanel()
{
    StoredSettings::getInstance()->getProps()
            .setValue ("lastPrefsPanelWindowPos", getWindowStateAsString());
}

void PrefsPanel::closeButtonPressed()
{
    setVisible (false);
}

void PrefsPanel::show()
{
    PrefsPanel pp;
    pp.runModalLoop();
}
