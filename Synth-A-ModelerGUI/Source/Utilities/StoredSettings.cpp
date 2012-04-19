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

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Application/CommonHeaders.h"

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
	return props->getValue("cmdperl", "/usr/bin/perl");
}
void StoredSettings::setCmdPerl(const String& cmdPerl)
{
	props->setValue("cmdperl", cmdPerl);
}

String StoredSettings::getCmdFaust() const
{
	return props->getValue("cmdfaust", "/usr/local/bin/faust");
}
void StoredSettings::setCmdFaust(const String& cmdFaust)
{
	props->setValue("cmdfaust", cmdFaust);
}

String StoredSettings::getCmdFaust2supercollider() const
{
	return props->getValue("cmdfaust2supercollider", "/usr/local/bin/faust2supercollider");
}
void StoredSettings::setCmdFaust2supercollider(const String& cmdFaust2supercollider)
{
	props->setValue("cmdfaust2supercollider", cmdFaust2supercollider);
}

String StoredSettings::getCmdFaust2puredata() const
{
	return props->getValue("cmdfaust2puredata", "/usr/local/bin/faust2puredata");
}
void StoredSettings::setCmdFaust2puredata(const String& cmdFaust2puredata)
{
	props->setValue("cmdfaust2puredata", cmdFaust2puredata);
}

String StoredSettings::getCmdSAM() const
{
	String samPath = "../../../../Synth-A-ModelerCmd/Synth-A-Modeler.plx";
#if JUCE_MAC
	samPath = "../"+samPath;
#endif
	return props->getValue("cmdsam", samPath);
}
void StoredSettings::setCmdSAM(const String& cmdSAM)
{
	props->setValue("cmdsam", cmdSAM);
}

bool StoredSettings::getShowCompilerWindow() const
{
	return props->getBoolValue("showcompilerwindow", false);
}
void StoredSettings::setShowCompilerWindow(bool shouldShow)
{
	props->setValue("showcompilerwindow", shouldShow);
}
