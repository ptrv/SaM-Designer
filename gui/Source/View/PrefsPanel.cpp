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
#include "PrefsPanel.h"
#include "../Utilities/StoredSettings.h"


using namespace synthamodeler;
//==============================================================================
class MiscPage  : public Component
{
public:
    MiscPage()
    {
        fcDataDir = new FilenameComponent("Synth-A-Modeler data directory:",
                                          StoredSettings::getInstance()->getDataDir(),
                                          true, true, false,
                                          String::empty, String::empty,
                                          "(select the Synth-A-Modeler data directory)");
        addAndMakeVisible(fcDataDir);
        laDataDir = new Label(String::empty, "Data Dir");
        laDataDir->attachToComponent (fcDataDir, false);

        fcFaustDir = new FilenameComponent("FAUST directory:",
                                           StoredSettings::getInstance()->getFaustDir(),
                                           true, true, false,
                                           String::empty, String::empty,
                                           "(select the directory where the faust executable resides)");
        addAndMakeVisible (fcFaustDir);
        laFaustDir = new Label(String::empty, "FAUST Dir");
        laFaustDir->attachToComponent (fcFaustDir, false);

        tbAutoCorrectValues = new ToggleButton("Autocorrect parameter values");
        addAndMakeVisible(tbAutoCorrectValues);
        tbAutoCorrectValues->setToggleState(StoredSettings::getInstance()->getAutoCorrectValues(),
                                            dontSendNotification);
        
        tbRunSAMBeforeExport = new ToggleButton("Run Synth-A-Modeler before generating code");
        addAndMakeVisible(tbRunSAMBeforeExport);
        tbRunSAMBeforeExport->setToggleState(StoredSettings::getInstance()->getRunSAMBeforeExternal(),
                                             dontSendNotification);
        
        tbExportConfirm = new ToggleButton("Confirm before generating code");
        addAndMakeVisible(tbExportConfirm);
        tbExportConfirm->setToggleState(StoredSettings::getInstance()->getIsExportConfirm(),
                                        dontSendNotification);

        tbOpenFaustExport = new ToggleButton("Open Faust file after export");
        addAndMakeVisible(tbOpenFaustExport);
        tbOpenFaustExport->setToggleState(StoredSettings::getInstance()->getOpenFaustExport(),
                                          dontSendNotification);

        tbUseMDLX = new ToggleButton("Use MDLX file");
        addAndMakeVisible(tbUseMDLX);
        tbUseMDLX->setToggleState(StoredSettings::getInstance()->getIsUsingMDLX(),
                                  dontSendNotification);
#if BUILTIN_SAM_COMPILER
        tbUseBuiltinCompiler = new ToggleButton("Use builtin Synth-A-Modeler compiler");
        addAndMakeVisible(tbUseBuiltinCompiler);
        tbUseBuiltinCompiler->setToggleState(StoredSettings::getInstance()->getIsUsingBuiltinSAMCompiler(),
                                             dontSendNotification);
#endif
#ifdef DEBUG
        tbLoggingOn = new ToggleButton("Logging (After change, restart required)");
        addAndMakeVisible(tbLoggingOn);
        tbLoggingOn->setToggleState(StoredSettings::getInstance()->getIsLoggingOn(),
                                    dontSendNotification);
#endif
        fcExternalEditor = new FilenameComponent("External editor:",
                                                 StoredSettings::getInstance()->getExternalEditor(),
                                                 true, false, false,
                                                 String::empty, String::empty,
                                                 "(select external editor executable)");
        addAndMakeVisible(fcExternalEditor);
        laExternalEditor = new Label(String::empty, "External editor");
        laExternalEditor->attachToComponent(fcExternalEditor, false);

        tbRedrawOnLoad = new ToggleButton("Redraw model when no position data");
        addAndMakeVisible(tbRedrawOnLoad);
        tbRedrawOnLoad->setToggleState(StoredSettings::getInstance()->getShouldRedrawOnLoad(),
                                       dontSendNotification);
    }

    ~MiscPage()
    {
        StoredSettings::getInstance()->setDataDir(fcDataDir->getCurrentFile().getFullPathName());
        fcDataDir = nullptr;
        StoredSettings::getInstance()->setFaustDir(fcFaustDir->getCurrentFile().getFullPathName());
        fcFaustDir = nullptr;
        StoredSettings::getInstance()->setAutoCorrectValues(tbAutoCorrectValues->getToggleState());
        tbAutoCorrectValues = nullptr;
        StoredSettings::getInstance()->setRunSAMBeforeExternal(tbRunSAMBeforeExport->getToggleState());
        tbRunSAMBeforeExport = nullptr;
        StoredSettings::getInstance()->setIsExportConfirm(tbExportConfirm->getToggleState());
        tbExportConfirm = nullptr;
        StoredSettings::getInstance()->setOpenFaustExport(tbOpenFaustExport->getToggleState());
        tbOpenFaustExport = nullptr;
        StoredSettings::getInstance()->setIsUsingMDLX(tbUseMDLX->getToggleState());
        tbUseMDLX = nullptr;
#if BUILTIN_SAM_COMPILER
        StoredSettings::getInstance()->setIsUsingBuiltinSAMCompiler(tbUseBuiltinCompiler->getToggleState());
        tbUseBuiltinCompiler = nullptr;
#endif
#ifdef DEBUG
        StoredSettings::getInstance()->setIsLoggingOn(tbLoggingOn->getToggleState());
        tbLoggingOn = nullptr;
#endif
        StoredSettings::getInstance()->setExternalEditor(fcExternalEditor->getCurrentFile().getFullPathName());
        fcExternalEditor = nullptr;
        StoredSettings::getInstance()->setShouldRedrawOnLoad(tbRedrawOnLoad->getToggleState());
        tbRedrawOnLoad = nullptr;

        laDataDir = nullptr;
        laFaustDir = nullptr;
        laExternalEditor = nullptr;
    }

    void resized()
    {
        fcDataDir->setBounds (10, 30, getWidth() - 20, 22);
        fcFaustDir->setBounds (10, 100, getWidth() - 20, 22);
        tbAutoCorrectValues->setBounds(10, 140, getWidth() - 20, 22);
        tbRunSAMBeforeExport->setBounds(10, 170, getWidth() - 20, 22);
        tbExportConfirm->setBounds(10, 200, getWidth() - 20, 22);
        tbOpenFaustExport->setBounds(10, 230, getWidth() - 20, 22);
        tbUseMDLX->setBounds(10, 260, getWidth() - 20, 22);
#if BUILTIN_SAM_COMPILER
        tbUseBuiltinCompiler->setBounds(10, 290, getWidth() - 20, 22);
#endif
#ifdef DEBUG
        tbLoggingOn->setBounds(10, 320, getWidth() - 20, 22);
#endif
        fcExternalEditor->setBounds(10, 380, getWidth() - 20, 22);
        tbRedrawOnLoad->setBounds(10, 410, getWidth() - 20, 22);
    }

private:
    ScopedPointer<FilenameComponent> fcDataDir;
    ScopedPointer<FilenameComponent> fcFaustDir;
    ScopedPointer<ToggleButton> tbAutoCorrectValues;
    ScopedPointer<ToggleButton> tbRunSAMBeforeExport;
    ScopedPointer<ToggleButton> tbExportConfirm;
    ScopedPointer<ToggleButton> tbOpenFaustExport;
    ScopedPointer<ToggleButton> tbUseMDLX;
#if BUILTIN_SAM_COMPILER
    ScopedPointer<ToggleButton> tbUseBuiltinCompiler;
#endif
#ifdef DEBUG
    ScopedPointer<ToggleButton> tbLoggingOn;
#endif
    ScopedPointer<Label> laDataDir;
    ScopedPointer<Label> laFaustDir;
    ScopedPointer<FilenameComponent> fcExternalEditor;
    ScopedPointer<Label> laExternalEditor;
    ScopedPointer<ToggleButton> tbRedrawOnLoad;
};

//==============================================================================
class ExporterInputPanel : public DialogWindow
{
public:
	ExporterInputPanel(int exporterIndex)
	: DialogWindow("Exporter", Colours::lightgrey, true)
	{
		ExporterInputComponent* const eic = new ExporterInputComponent(*this, exporterIndex);
		eic->setSize(350, 100);

		setContentOwned (eic, true);

		centreAroundComponent (0, getWidth(), getHeight());

		setResizable (true, false);
	}

	~ExporterInputPanel()
	{

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
            laVarName->setText("Exporter", dontSendNotification);

            addAndMakeVisible(laVarValue = new Label("Command"));
            laVarValue->setText("Command", dontSendNotification);

            addAndMakeVisible(teVarName = new TextEditor("Input Variable"));
            teVarName->addListener(this);

            addAndMakeVisible(teVarValue = new TextEditor("Input Value"));
            teVarValue->addListener(this);

            addAndMakeVisible(btOk = new TextButton("Ok"));
            btOk->addListener(this);

            addAndMakeVisible(btCancel = new TextButton("Cancel"));
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
            teVarValue->setBounds(60, 40, getWidth() - 65, 22);
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

	    table->getHeader().addColumn("Exporter",1,100);

	    table->getHeader().addColumn("Command",2, 500);
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
static const char* miscPage = "Misc";
static const char* exporterPage = "Exporter";
static const char* aboutPage = "About";

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
static String prefsWindowPos;


PrefsPanel::PrefsPanel()
    : DialogWindow ("Synth-A-Modeler-Designer Preferences",
                    Colour::greyLevel (0.92f), true)
{
    PrefsTabComp* const p = new PrefsTabComp();
    p->setSize (456, 520);

    setContentOwned (p, true);

    if (! restoreWindowStateFromString (prefsWindowPos))
        centreWithSize(getWidth(), getHeight());

    setResizable (true, true);
    setResizeLimits (400, 400, 1000, 800);
}

PrefsPanel::~PrefsPanel()
{
    prefsWindowPos = getWindowStateAsString();
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
