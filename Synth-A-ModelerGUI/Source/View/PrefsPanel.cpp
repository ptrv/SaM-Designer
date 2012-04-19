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
        : link ("www.rawmaterialsoftware.com/juce",
                URL ("http://www.rawmaterialsoftware.com/juce"))//,
          //logo (ImageCache::getFromMemory (BinaryData::jules_jpg, BinaryData::jules_jpgSize))
    {
        text1.setJustification (Justification::centredTop);
        text1.append ("Programmer Julian Storer, seen here demonstrating a beard designed to "
                      "gain approval from the Linux programming community. Each hair of the beard "
                      "represents one line of source code from the ", Font (13.0f));
        text1.append ("Jucer", Font (13.0f, Font::bold));
        text1.append (" component design tool.", Font (13.0f));

        text2.setJustification (Justification::centred);
        text2.append ("Jucer v" + JUCEApplication::getInstance()->getApplicationVersion()
                        + ", " + SystemStats::getJUCEVersion(), Font (12.0f, Font::bold));

        addAndMakeVisible (&link);
        link.setFont (Font (10.0f, Font::bold | Font::underlined), true);
    }

    void paint (Graphics& g)
    {
        g.fillAll (Colour (0xffebebeb));
//        g.drawImageWithin (logo, 0, 4, getWidth(), getHeight() - 144,
//                           RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize,
//                           false);

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
//    Image logo;
    AttributedString text1, text2;
};


//==============================================================================
static const char* audioPage = "Audio Device";
static const char* miscPage = "Misc";
static const char* aboutPage = "About";

class PrefsTabComp  : public PreferencesPanel
{
public:
    PrefsTabComp()
    {
        addSettingsPage (audioPage, BinaryData::prefs_keys_png, BinaryData::prefs_keys_pngSize);
//        addSettingsPage (miscPage, BinaryData::prefs_misc_png, BinaryData::prefs_misc_pngSize);
//        addSettingsPage (aboutPage, BinaryData::prefs_about_png, BinaryData::prefs_about_pngSize);
    }

    ~PrefsTabComp()
    {
        StoredSettings::getInstance()->flush();
    }

    Component* createComponentForPage (const String& pageName)
    {
        if (pageName == audioPage)
        {
//            return new MiscPage();
        }
        else if (pageName == miscPage)
        {
//            return new KeyMappingEditorComponent (*commandManager->getKeyMappings(), true);
        }
        else if (pageName == aboutPage)
        {
//            return new AboutPage();
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
