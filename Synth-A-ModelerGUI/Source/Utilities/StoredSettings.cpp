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

static const String getDefaultPathFaustDir()
{
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
	return "";
}
static const String getDefaultPathPerl()
{
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
	return "";
}

static const String getDefaultPathDataDir()
{
	File infile = File::getSpecialLocation(File::userDocumentsDirectory)
									.getChildFile("Synth-A-Modeler-Data");
	if(infile.isDirectory())
	{
		return infile.getFullPathName();
	}
#if JUCE_MAC
	String relDataDir = "../../../../../Synth-A-ModelerCmd";
#else
	String relDataDir = "../../../../Synth-A-ModelerCmd";
#endif
	infile = File::getCurrentWorkingDirectory().getChildFile(relDataDir);
	if(infile.isDirectory())
	{
		return infile.getFullPathName();
	}
	return "";
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
    clearSingletonInstance();
}

juce_ImplementSingleton (StoredSettings)


//==============================================================================
PropertiesFile& StoredSettings::getProps()
{
    jassert (props != nullptr);
    return *props;
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
    options.applicationName     = "Synth-A-Modeler";
#if JUCE_LINUX
    options.folderName          = ".Synth-A-Modeler";
#else
    options.folderName          = "Synth-A-Modeler";
#endif
    options.filenameSuffix      = "settings";
    options.osxLibrarySubFolder = "Application Support";

    props = new PropertiesFile (options);

    // recent files...
    recentFiles.restoreFromString (props->getValue ("recentFiles"));
    recentFiles.removeNonExistentFiles();

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
	return props->getValue("cmdperl", getDefaultPathPerl());
}
void StoredSettings::setCmdPerl(const String& cmdPerl)
{
	props->setValue("cmdperl", cmdPerl);
}

String StoredSettings::getFaustDir() const
{
	return props->getValue("faustdir", getDefaultPathFaustDir());
}
void StoredSettings::setFaustDir(const String& faustDir)
{
	props->setValue("faustdir", faustDir);
}

bool StoredSettings::getShowCompilerWindow() const
{
	return props->getBoolValue("showcompilerwindow", false);
}
void StoredSettings::setShowCompilerWindow(bool shouldShow)
{
	props->setValue("showcompilerwindow", shouldShow);
}

const String StoredSettings::getWorkingFolder() const
{
	return props->getValue("workingdir",
			File::getCurrentWorkingDirectory().getFullPathName());
}

void StoredSettings::setWorkingFolder (const String& folder)
{
	props->setValue("workingdir", folder);
}

const String StoredSettings::getLastDocument() const
{
	return props->getValue("lastmdl", String::empty);
}
void StoredSettings::setLastDocument(const String& docName)
{
	props->setValue("lastmdl", docName);
}

const String StoredSettings::getDataDir() const
{
	return props->getValue("datadir", getDefaultPathDataDir());
}
void StoredSettings::setDataDir (const String& folder)
{
	props->setValue("datadir", folder);
}

const String StoredSettings::getCmdExporter() const
{
	return props->getValue("cmdexporter", "make -C $(DATA_DIR) puredata");
}
void StoredSettings::setCmdExporter(const String& exporters)
{
	props->setValue("cmdexporter", exporters);
}

const bool StoredSettings::getIsSegmentedConnectors() const
{
	return props->getBoolValue("segmentedconnectors", false);
}
void StoredSettings::setIsSegmentedConnectors (bool isSegmentedConnectors)
{
	props->setValue("segmentedconnectors", isSegmentedConnectors);
}

const bool StoredSettings::getIsExportConfirm() const
{
	return props->getBoolValue("exportconfirm", false);
}
void StoredSettings::setIsExportConfirm (bool isExportConfirm)
{
	props->setValue("exportconfirm", isExportConfirm);
}
