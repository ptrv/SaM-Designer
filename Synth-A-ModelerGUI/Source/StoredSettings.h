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

    const StringArray& getFontNames();


    //==============================================================================
    Image getFallbackImage();
    const Drawable* getImageFileIcon();


private:
    ScopedPointer<PropertiesFile> props;
    StringArray fontNames;

    ScopedPointer<Drawable> imageFileIcon;
    Image fallbackImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StoredSettings);
};




#endif  // __STOREDSETTINGS_H_7CF20172__
