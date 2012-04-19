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
          label (String::empty, samPath.getCurrentFile().getFileNameWithoutExtension())
    {
        addAndMakeVisible (&samPath);
        label.attachToComponent (&samPath, true);
    }

    ~MiscPage()
    {
        StoredSettings::getInstance()->setCmdSAM(samPath.getCurrentFile().getFullPathName());
    }

    void resized()
    {
        samPath.setBounds (150, 16, getWidth() - 160, 22);
    }

private:
    FilenameComponent samPath;
//    FilenameComponent faustPath;
    Label label;
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
