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

namespace synthamodeler
{
/**
 * Singleton class which handles a property file for persistent user settings.
 */
class StoredSettings
{
public:
    //==============================================================================
    StoredSettings();
    ~StoredSettings();

    juce_DeclareSingleton (StoredSettings, false)

    PropertiesFile& getProps();
    PropertiesFile& getExporters();
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

	const bool getIsSegmentedConnectors() const;
    void setIsSegmentedConnectors (bool isSegmentedConnectors);

	const bool getIsExportConfirm() const;
    void setIsExportConfirm (bool isExportConfirm);

    const bool getIsLoggingOn() const;
    void setIsLoggingOn (bool isLoggingOn);

    const bool getOpenFaustExport() const;
    void setOpenFaustExport (bool isOpenMdlAfterExport);

    const String getCurrentExporter() const;
    void setCurrentExporter(const String& currentExporter);

//    Array<File> getExporters() const;
//    void setExporters (const Array<File>& files);

    //==============================================================================
//    Image getFallbackImage();
//    const Drawable* getImageFileIcon();

    const bool getIsSnapGridEnabled() const;
    void setSnapGridEnabled (bool snapGridActive);
    const bool getIsSnapGridShow() const;
    void setSnapGridShow (bool snapGridShow);
    const int getSnapGridPixels() const;
    void setSnapGridPixels (int snapGridPixels);

    const bool getRunSAMBeforeExternal() const;
    void setRunSAMBeforeExternal (bool runSAMBeforeExternal);

    const bool getIsUsingMDLX() const;
    void setIsUsingMDLX (bool isUsingMDLX);

    const bool getIsUsingBuiltinSAMCompiler() const;
    void setIsUsingBuiltinSAMCompiler (bool isUsingBuiltinSAMCompiler);

    const bool getShowAudioConnections() const;
    void setShowAudioConnections (bool showAudioConnections);

private:
    ScopedPointer<PropertiesFile> props;
    ScopedPointer<PropertiesFile> exporters;
    StringArray fontNames;

//    ScopedPointer<Drawable> imageFileIcon;
//    Image fallbackImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StoredSettings);
};
}



#endif  // __STOREDSETTINGS_H_7CF20172__
