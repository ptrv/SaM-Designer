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

    String getFaustDir() const;
    void setFaustDir(const String& faustDir);

    bool getShowCompilerWindow() const;
    void setShowCompilerWindow(bool shouldShow);

	const String getWorkingFolder() const;
    void setWorkingFolder (const String& folder);

	const String getLastDocument() const;
    void setLastDocument(const String& docName);

	const String getDataDir() const;
    void setDataDir (const String& folder);

    const StringArray& getFontNames();

    const String getCmdExporter() const;
    void setCmdExporter(const String& exporters);

	const bool getIsSegmentedConnectors() const;
    void setIsSegmentedConnectors (bool isSegmentedConnectors);

	const bool getIsExportConfirm() const;
    void setIsExportConfirm (bool isExportConfirm);

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
