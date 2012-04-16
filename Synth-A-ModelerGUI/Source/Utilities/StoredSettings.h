/*
  ==============================================================================

    StoredSettings.h
    Created: 22 Jan 2012 10:14:22pm
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
