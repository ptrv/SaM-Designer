/*
  ==============================================================================

    StoredSettings.cpp
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

#include "../../JuceLibraryCode/JuceHeader.h"

#include "StoredSettings.h"
#include "MiscUtilities.h"

using namespace synthamodeler;

static const String getDefaultPathFaustDir()
{
#ifdef JUCE_WINDOWS
	File infile("C:/Program Files/Faust/Faust.exe");
	if(infile.exists())
	{
		return infile.getParentDirectory().getFullPathName();
	}

#else
	File infile("/usr/local/bin/faust");
	if(infile.existsAsFile())
	{
		return infile.getParentDirectory().getFullPathName();
	}
	infile = "/usr/bin/faust";
	if(infile.existsAsFile())
	{
		return infile.getParentDirectory().getFullPathName();
	}
#endif
	return "";
}
static const String getDefaultPathPerl()
{
#ifdef JUCE_WINDOWS
	File infile("C:/strawberry/perl/bin/perl.exe");
	if(infile.exists())
	{
		return infile.getFullPathName();
	}
#else
	File infile("/usr/bin/perl");
	if(infile.existsAsFile())
	{
		return infile.getFullPathName();
	}
	infile = "/usr/local/bin/perl";
	if(infile.existsAsFile())
	{
		return infile.getFullPathName();
	}
#endif
	return "";
}

//static const String getDefaultPathDataDir()
//{
//	File infile = File::getSpecialLocation(File::userDocumentsDirectory)
//									.getChildFile("Synth-A-Modeler-Data");
//	if(infile.isDirectory())
//	{
//		return infile.getFullPathName();
//	}
//#if JUCE_MAC
//	String relDataDir = "../../../../../SaM";
//#else
//	String relDataDir = "../../../../SaM";
//#endif
//	infile = File::getCurrentWorkingDirectory().getChildFile(relDataDir);
//	if(infile.isDirectory())
//	{
//		return infile.getFullPathName();
//	}
//	return "";
//}

static const String getDefaultExternalEditor()
{
#ifdef JUCE_WINDOWS
    return "notepad.exe";
#elif JUCE_MAC
    return "/usr/bin/open -a TextEdit";
#elif JUCE_LINUX
    return "/usr/bin/gedit";
#else
    return String::empty;
#endif
}

//==============================================================================
StoredSettings::StoredSettings()
{
    flush();
}

StoredSettings::~StoredSettings()
{
    flush();
    props = nullptr;
    exporters = nullptr;
    defaultValues = nullptr;
    clearSingletonInstance();
}

juce_ImplementSingleton (StoredSettings)


//==============================================================================
PropertiesFile& StoredSettings::getProps()
{
    jassert (props != nullptr);
    return *props;
}

PropertiesFile& StoredSettings::getProps() const
{
    jassert (props != nullptr);
    return *props;
}

PropertiesFile& StoredSettings::getExporters()
{
    jassert (exporters != nullptr);
    return *exporters;
}

PropertiesFile& StoredSettings::getDefaultValues()
{
    jassert (defaultValues != nullptr);
    return *defaultValues;
}

void StoredSettings::flush()
{
    if (props != nullptr)
    {
        props->setValue ("recentFiles", recentFiles.toString());
    }

    props = nullptr;

    // These settings are used in defining the properties file's location.
    PropertiesFile::Options options;
    options.applicationName     = "Synth-A-Modeler-Designer";
#if JUCE_LINUX
    options.folderName          = ".Synth-A-Modeler-Designer";
#else
    options.folderName          = "Synth-A-Modeler-Designer";
#endif
    options.filenameSuffix      = "settings";
    options.osxLibrarySubFolder = "Application Support";

    props = new PropertiesFile (options);

    // recent files...
    recentFiles.restoreFromString (props->getValue ("recentFiles"));
    recentFiles.removeNonExistentFiles();

    exporters = nullptr;

    PropertiesFile::Options exporterOptions = options;
    exporterOptions.filenameSuffix      = "exporters";

    exporters = new PropertiesFile(exporterOptions);

    defaultValues = nullptr;
    PropertiesFile::Options defaultValuesOptions = options;
    defaultValuesOptions.filenameSuffix = "default_values";

    defaultValues = new PropertiesFile(defaultValuesOptions);
}

const String StoredSettings::getDefaultValue(const String& keyName,
                                             const String& defaultVal)
{
    if(! defaultValues->containsKey(keyName))
    {
        defaultValues->setValue(keyName, defaultVal);
    }
    return defaultValues->getValue(keyName, defaultVal);
}
Array<File> StoredSettings::getLastFiles() const
{
    StringArray s;
    s.addTokens (props->getValue ("lastFiles"), "|", "");

    Array<File> f;
    for (int i = 0; i < s.size(); ++i)
        f.add (File (s[i]));

    return f;
}

void StoredSettings::setLastFiles(const Array<File>& files)
{
    StringArray s;
    for (int i = 0; i < files.size(); ++i)
        s.add (files.getReference(i).getFullPathName());

    props->setValue ("lastFiles", s.joinIntoString ("|"));
}

const StringArray& StoredSettings::getFontNames()
{
    if (fontNames.size() == 0)
        fontNames = Font::findAllTypefaceNames();

    return fontNames;
}

String StoredSettings::getCmdPerl() const
{
	return props->getValue("cmdPerl", getDefaultPathPerl());
}
void StoredSettings::setCmdPerl(const String& cmdPerl)
{
	props->setValue("cmdPerl", cmdPerl);
}

String StoredSettings::getFaustDir() const
{
	return props->getValue("faustDir", getDefaultPathFaustDir());
}
void StoredSettings::setFaustDir(const String& faustDir)
{
	props->setValue("faustDir", faustDir);
}

bool StoredSettings::getShowCompilerWindow() const
{
	return props->getBoolValue("showPostWindow", false);
}
void StoredSettings::setShowCompilerWindow(bool shouldShow)
{
	props->setValue("showPostWindow", shouldShow);
}

const String StoredSettings::getWorkingFolder() const
{
	return props->getValue("workingDir",
			File::getCurrentWorkingDirectory().getFullPathName());
}

void StoredSettings::setWorkingFolder (const String& folder)
{
	props->setValue("workingDir", folder);
}

const String StoredSettings::getLastDocument() const
{
	return props->getValue("lastMDL", String::empty);
}
void StoredSettings::setLastDocument(const String& docName)
{
	props->setValue("lastMDL", docName);
}

const String StoredSettings::getDataDir() const
{
    String p = File::getSpecialLocation(File::currentApplicationFile)
                                    .getParentDirectory().getFullPathName();
	return props->getValue("dataDir", p);
}
void StoredSettings::setDataDir (const String& folder)
{
	props->setValue("dataDir", folder);
}

bool StoredSettings::getIsSegmentedConnectors() const
{
	return props->getBoolValue("segmentedConnectors", false);
}
void StoredSettings::setIsSegmentedConnectors (bool isSegmentedConnectors)
{
	props->setValue("segmentedConnectors", isSegmentedConnectors);
}

bool StoredSettings::getIsExportConfirm() const
{
	return props->getBoolValue("exportConfirm", false);
}
void StoredSettings::setIsExportConfirm (bool isExportConfirm)
{
	props->setValue("exportConfirm", isExportConfirm);
}

bool StoredSettings::getIsLoggingOn() const
{
    return props->getBoolValue("loggingOn", false);
}
void StoredSettings::setIsLoggingOn (bool isLoggingOn)
{
    props->setValue("loggingOn", isLoggingOn);
}

const String StoredSettings::getCurrentExporter() const
{
    return props->getValue("currentExporter", "");
}
void StoredSettings::setCurrentExporter(const String& currentExporter)
{
    props->setValue("currentExporter", currentExporter);
}

bool StoredSettings::getIsSnapGridEnabled() const
{
    return props->getBoolValue("snapGridEnabled", true);
}
void StoredSettings::setSnapGridEnabled (bool snapGridActive)
{
    props->setValue("snapGridEnabled", snapGridActive);
}
bool StoredSettings::getIsSnapGridShow() const
{
    return props->getBoolValue("snapGridShow", true);
}
void StoredSettings::setSnapGridShow (bool snapGridShow)
{
    props->setValue("snapGridShow", snapGridShow);
}
int StoredSettings::getSnapGridPixels() const
{
    return props->getIntValue("snapGridPixels", 8);
}
void StoredSettings::setSnapGridPixels (int snapGridPixels)
{
    props->setValue("snapGridPixels", snapGridPixels);
}

bool StoredSettings::getRunSAMBeforeExternal() const
{
    return props->getBoolValue("runSAMBeforeExternal", true);
}
void StoredSettings::setRunSAMBeforeExternal (bool runSAMBeforeExternal)
{
    props->setValue("runSAMBeforeExternal", runSAMBeforeExternal);
}

bool StoredSettings::getAutoCorrectValues() const
{
    return props->getBoolValue("autoCorrectParameterValues", true);
}
void StoredSettings::setAutoCorrectValues (bool shouldAutoCorrect)
{
    props->setValue("autoCorrectParameterValues", shouldAutoCorrect);
}

bool StoredSettings::getIsUsingMDLX() const
{
    return props->getBoolValue("useMDLX", false);
}
void StoredSettings::setIsUsingMDLX(bool isUsingMDLX)
{
    props->setValue("useMDLX", isUsingMDLX);
}
bool StoredSettings::getShowAudioConnections() const
{
    return props->getBoolValue("showAudioConnections", true);
}
void StoredSettings::setShowAudioConnections(bool showAudioConnections)
{
    props->setValue("showAudioConnections", showAudioConnections);
}

String StoredSettings::getExternalEditor() const
{
    return props->getValue("externalEditor", getDefaultExternalEditor());
}

void StoredSettings::setExternalEditor(const String& externalEditor)
{
    props->setValue("externalEditor", externalEditor);
}

bool StoredSettings::getShouldRedrawOnLoad() const
{
    return props->getBoolValue("shouldRedrawOnLoad", true);
}
void StoredSettings::setShouldRedrawOnLoad(bool shouldRedrawOnLoad)
{
    props->setValue("shouldRedrawOnLoad", shouldRedrawOnLoad);
}

bool StoredSettings::getIsPropertiesWindowAlwaysOnTop() const
{
    return props->getBoolValue("propertiesWindowAlwaysOnTop", false);
}
void StoredSettings::setIsPropertiesWindowAlwaysOnTop (bool alwaysOnTop)
{
    props->setValue("propertiesWindowAlwaysOnTop", alwaysOnTop);
}
