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


//==============================================================================
class MiscPage  : public Component
{
public:
    MiscPage()
    : fcDataDir ("Synth-A-Modeler location:",
    		StoredSettings::getInstance()->getDataDir(),
    		true, true, false,
    		"*.plx", String::empty,
    		"(select the Synth-A-Modeler.plx file)"),
    	fcFaustDir ("FAUST directory:",
			StoredSettings::getInstance()->getFaustDir(),
			true, true, false,
			"*.*", String::empty,
			"(select the directory which faust is in)"),
		teCmdExport("CmdExport"),
		tbExportConfirm("Confirm export"),
#ifdef DEBUG
        tbLoggingOn("Logging (After change, restart required)"),
#endif
        labelDataDir (String::empty, "Data Dir"),
		labelFaustDir (String::empty, "FAUST Dir"),
		labelCmdExport(String::empty, "Exporter Command")
    {
        addAndMakeVisible (&fcDataDir);
        labelDataDir.attachToComponent (&fcDataDir, false);
        addAndMakeVisible (&fcFaustDir);
        labelFaustDir.attachToComponent (&fcFaustDir, false);
        addAndMakeVisible (&teCmdExport);
        labelCmdExport.attachToComponent (&teCmdExport, false);
        teCmdExport.setMultiLine(false);
        teCmdExport.setText(StoredSettings::getInstance()->getCmdExporter());
        addAndMakeVisible(&tbExportConfirm);
        tbExportConfirm.setToggleState(StoredSettings::getInstance()->getIsExportConfirm(), false);
#ifdef DEBUG
        addAndMakeVisible(&tbLoggingOn);
        tbLoggingOn.setToggleState(StoredSettings::getInstance()->getIsLoggingOn(), false);
#endif
    }

    ~MiscPage()
    {
        StoredSettings::getInstance()->setDataDir(fcDataDir.getCurrentFile().getFullPathName());
        StoredSettings::getInstance()->setFaustDir(fcFaustDir.getCurrentFile().getFullPathName());
        StoredSettings::getInstance()->setCmdExporter(teCmdExport.getText());
        StoredSettings::getInstance()->setIsExportConfirm(tbExportConfirm.getToggleState());
#ifdef DEBUG
        StoredSettings::getInstance()->setIsLoggingOn(tbLoggingOn.getToggleState());
#endif
    }

    void resized()
    {
        fcDataDir.setBounds (10, 30, getWidth() - 20, 22);
        fcFaustDir.setBounds (10, 100, getWidth() - 20, 22);
        teCmdExport.setBounds(10, 170, getWidth() -20, 22);
        tbExportConfirm.setBounds(10, 220, getWidth() - 20, 22);
#ifdef DEBUG
        tbLoggingOn.setBounds(10, 270, getWidth() - 20, 22);
#endif
    }

private:
    FilenameComponent fcDataDir;
    FilenameComponent fcFaustDir;
    TextEditor teCmdExport;
    ToggleButton tbExportConfirm;
#ifdef DEBUG
    ToggleButton tbLoggingOn;
#endif
    Label labelDataDir;
    Label labelFaustDir;
    Label labelCmdExport;
};

//==============================================================================
class AboutPage   : public Component
{
public:
    AboutPage()
        : link ("https://github.com/ptrv/Synth-A-Modeler",
                URL ("https://github.com/ptrv/Synth-A-Modeler")),
          logo (ImageCache::getFromMemory (BinaryData::synthamodeler_icon_png,
        		  BinaryData::synthamodeler_icon_pngSize))
    {
        text1.setJustification (Justification::centredTop);
        String infoTxt(BinaryData::about_txt);
        text1.append (infoTxt, Font (13.0f));

        text2.setJustification (Justification::centred);
        String buildDate = " (Build: " + String(__DATE__) + " " + String(__TIME__) + ")";
        text2.append ("Synth-A-Modeler v" + JUCEApplication::getInstance()->getApplicationVersion()
                        + buildDate + ", " + SystemStats::getJUCEVersion(), Font (12.0f, Font::bold));

        addAndMakeVisible (&link);
        link.setFont (Font (10.0f, Font::bold | Font::underlined), true);
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
        link.setSize (100, 22);
        link.changeWidthToFitText();
        link.setTopLeftPosition ((getWidth() - link.getWidth()) / 2, getHeight() - link.getHeight() - 10);
    }

private:
    HyperlinkButton link;
    Image logo;
    AttributedString text1, text2;
};


//==============================================================================
static const char* miscPage = "Misc";
static const char* aboutPage = "About";

class PrefsTabComp  : public PreferencesPanel
{
public:
    PrefsTabComp()
    {
        addSettingsPage (miscPage, BinaryData::prefs_misc_png, BinaryData::prefs_misc_pngSize);
        addSettingsPage (aboutPage, BinaryData::prefs_about_png, BinaryData::prefs_about_pngSize);
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

        return new Component();
    }
};


//==============================================================================
static String prefsWindowPos;


PrefsPanel::PrefsPanel()
    : DialogWindow ("Synth-A-Modeler Preferences", Colour::greyLevel (0.92f), true)
{
    PrefsTabComp* const p = new PrefsTabComp();
    p->setSize (456, 510);

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
