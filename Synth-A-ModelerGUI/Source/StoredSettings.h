/*
  ==============================================================================

    StoredSettings.h
    Created: 22 Jan 2012 10:14:22pm
    Author:  Peter Vasil

  ==============================================================================
*/

#ifndef __STOREDSETTINGS_H_7CF20172__
#define __STOREDSETTINGS_H_7CF20172__

#include "../JuceLibraryCode/JuceHeader.h"

class StoredSettings
{
public:
    //==============================================================================
    StoredSettings();
    ~StoredSettings();

    juce_DeclareSingleton (StoredSettings, false)

    PropertiesFile& getProps();
    void flush();

    //==============================================================================
    RecentlyOpenedFilesList recentFiles;

    Array<File> getLastFiles() const;
    void setLastFiles (const Array<File>& files);

    String getCmdPerl() const;
    void setCmdPerl(const String& cmdPerl);
    String getCmdFaust() const;
    void setCmdFaust(const String& cmdFaust);
    String getCmdFaust2supercollider() const;
    void setCmdFaust2supercollider(const String& cmdFaust2supercollider);
    String getCmdFaust2puredata() const;
    void setCmdFaust2puredata(const String& cmdFaust2puredata);
    String getCmdSAM() const;
    void setCmdSAM(const String& cmdSAM);

    const StringArray& getFontNames();


    //==============================================================================
//    Image getFallbackImage();
//    const Drawable* getImageFileIcon();


private:
    ScopedPointer<PropertiesFile> props;
    StringArray fontNames;

//    ScopedPointer<Drawable> imageFileIcon;
//    Image fallbackImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StoredSettings);
};




#endif  // __STOREDSETTINGS_H_7CF20172__
