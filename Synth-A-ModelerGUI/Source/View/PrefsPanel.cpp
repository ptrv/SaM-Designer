/*
  ==============================================================================

    PrefsPanel.cpp
    Created: 19 Apr 2012 2:25:09pm
    Author:  peter

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "PrefsPanel.h"
#include "../Utilities/StoredSettings.h"


class AudioPage  : public Component
{
public:
	AudioPage(AudioDeviceManager& deviceManager_)
	: deviceManager(deviceManager_),
	  deviceSelector(0)
    {
		addAndMakeVisible (deviceSelector = new AudioDeviceSelectorComponent (deviceManager, 0, 2, 0, 2, true, true, true, false));
    }

    ~AudioPage()
    {
    	deleteAndZero (deviceSelector);
    }

    void resized()
    {
    	deviceSelector->setBounds (8, 8, getWidth() - 16, getHeight() - 16);
    }

private:
    AudioDeviceManager& deviceManager;
    AudioDeviceSelectorComponent* deviceSelector;
};

//==============================================================================
class MiscPage  : public Component
{
public:
    MiscPage()
    : samPath ("Synth-A-Modeler location:",
    		StoredSettings::getInstance()->getCmdSAM(),
    		true, false, false,
    		"*.plx", String::empty,
    		"(select the Synth-A-Modeler.plx file)"),
		faustPath ("faust location:",
				StoredSettings::getInstance()->getCmdFaust(),
				true, false, false,
				"*.*", String::empty,
				"(select the faust executable)"),
		faust2supercolliderPath ("faust2supercollider location:",
				StoredSettings::getInstance()->getCmdFaust2supercollider(),
				true, false, false,
				"*.*", String::empty,
				"(select the faust2supercollider executable)"),
		faust2puredataPath ("faust2puredata location:",
				StoredSettings::getInstance()->getCmdFaust2puredata(),
				true, false, false,
				"*.*", String::empty,
				"(select the faust2puredata executable)"),
		labelSAM (String::empty, samPath.getCurrentFile().getFileNameWithoutExtension()),
		labelFaust(String::empty, faustPath.getCurrentFile().getFileNameWithoutExtension()),
		labelFaust2supercollider(String::empty, faust2supercolliderPath.getCurrentFile().getFileNameWithoutExtension()),
		labelFaust2puredata(String::empty, faust2puredataPath.getCurrentFile().getFileNameWithoutExtension())
    {
        addAndMakeVisible (&samPath);
        labelSAM.attachToComponent (&samPath, true);
        addAndMakeVisible (&faustPath);
        labelFaust.attachToComponent (&faustPath, true);
        addAndMakeVisible (&faust2supercolliderPath);
        labelFaust2supercollider.attachToComponent (&faust2supercolliderPath, true);
        addAndMakeVisible (&faust2puredataPath);
        labelFaust2puredata.attachToComponent (&faust2puredataPath, true);
    }

    ~MiscPage()
    {
        StoredSettings::getInstance()->setCmdSAM(samPath.getCurrentFile().getFullPathName());
        StoredSettings::getInstance()->setCmdFaust(faustPath.getCurrentFile().getFullPathName());
        StoredSettings::getInstance()->setCmdFaust2supercollider(faust2supercolliderPath.getCurrentFile().getFullPathName());
        StoredSettings::getInstance()->setCmdFaust2puredata(faust2puredataPath.getCurrentFile().getFullPathName());
    }

    void resized()
    {
        samPath.setBounds (150, 16, getWidth() - 160, 22);
        faustPath.setBounds(150, 56, getWidth() -160, 22);
        faust2supercolliderPath.setBounds(150, 96, getWidth() -160, 22);
        faust2puredataPath.setBounds(150, 136, getWidth() -160, 22);
    }

private:
    FilenameComponent samPath;
    FilenameComponent faustPath;
    FilenameComponent faust2supercolliderPath;
    FilenameComponent faust2puredataPath;
    Label labelSAM;
    Label labelFaust;
    Label labelFaust2supercollider;
    Label labelFaust2puredata;
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
        String infoTxt(BinaryData::Info_txt);
        text1.append (infoTxt, Font (13.0f));

        text2.setJustification (Justification::centred);
        text2.append ("Synth-A-Modeler v" + JUCEApplication::getInstance()->getApplicationVersion()
                        + ", " + SystemStats::getJUCEVersion(), Font (12.0f, Font::bold));

        addAndMakeVisible (&link);
        link.setFont (Font (10.0f, Font::bold | Font::underlined), true);
    }

    void paint (Graphics& g)
    {
        g.fillAll (Colour (0xffebebeb));
        g.drawImageWithin (logo, 0, 4, getWidth(), getHeight() - 144,
                           RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize,
                           false);

        text1.draw (g, Rectangle<int> (12, getHeight() - 130, getWidth() - 24, 100).toFloat());
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
static const char* audioPage = "Audio";
static const char* miscPage = "Misc";
static const char* aboutPage = "About";

class PrefsTabComp  : public PreferencesPanel
{
public:
    PrefsTabComp()
    {
        addSettingsPage (audioPage, BinaryData::prefs_audio_png, BinaryData::prefs_audio_pngSize);
        addSettingsPage (miscPage, BinaryData::prefs_misc_png, BinaryData::prefs_misc_pngSize);
        addSettingsPage (aboutPage, BinaryData::prefs_about_png, BinaryData::prefs_about_pngSize);
    }

    ~PrefsTabComp()
    {
        StoredSettings::getInstance()->flush();
    }

    Component* createComponentForPage (const String& pageName)
    {
        if (pageName == audioPage)
        {
        	return new AudioPage(deviceManager);
        }
        else if (pageName == miscPage)
        {
            return new MiscPage();
        }
        else if (pageName == aboutPage)
        {
            return new AboutPage();
        }

        return new Component();
    }
private:
    AudioDeviceManager deviceManager;
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
        centreAroundComponent (0, getWidth(), getHeight());

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
