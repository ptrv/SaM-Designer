/*
  ==============================================================================

    MiscUtilities.cpp
    Created: 12 May 2012 3:38:26pm
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
#include "../Application/CommonHeaders.h"
#include "../Graph/Node.h"
#include "../View/BaseObjectComponent.h"
#include "../View/SelectableObject.h"
#include "../View/ObjectComponent.h"
#include "../View/LinkComponent.h"
#include "../Controller/ObjController.h"
#include "../Controller/MDLController.h"
#include "../Models/SAMCmd.h"
#include "../Models/MDLFile.h"

#include "MiscUtilities.h"
#include "RegularExpression.h"

#include <sys/time.h>

using namespace synthamodeler;

bool Utils::cancelAnyModalComponents()
{
    const int numModal = ModalComponentManager::getInstance()->getNumModalComponents();

    for (int i = numModal; --i >= 0;)
        if (ModalComponentManager::getInstance()->getModalComponent(i) != nullptr)
            ModalComponentManager::getInstance()->getModalComponent(i)->exitModalState (0);

    return numModal > 0;
}

void Utils::SAMLogger(const String& message)
{
    String timeString;
//	timeString << "[" << Time::getCurrentTime().formatted("%H:%M:%S")
	timeString << "[" << Time::getCurrentTime().formatted("%F %T");
    timeString << "." << String::formatted("%03d", getCurrentTimeMillis());
    timeString << "]: ";
	String tmp;
	tmp << timeString << message;
	Logger::writeToLog(tmp);
}

FileLogger* Utils::getLogger()
{
#if JUCE_LINUX
	String subDir = ".Synth-A-Modeler-Designer";
#else
	String subDir = "";
#endif

	return FileLogger::createDefaultAppLogger(subDir,
			"Synth-A-Modeler-Designer.log", "Synth-A-Modeler-Designer logfile");
}

void Utils::openHelpUrl()
{
	URL helpUrl("https://github.com/ptrv/SaM-Designer/wiki");
//	URL helpUrl("http://www.user.tu-berlin.de/peter.vasil/synthamodelergui-help/");
	helpUrl.launchInDefaultBrowser();
}

void Utils::openHelpUrl(const String& helpId)
{
	URL helpUrl("https://github.com/ptrv/SaM-Designer/wiki/" + helpId);
//	URL helpUrl("http://www.user.tu-berlin.de/peter.vasil/synthamodelergui-help/" + helpId + ".html");
	helpUrl.launchInDefaultBrowser();
}

void Utils::openHelpPatch(const String& helpId)
{
    String helpPatch = helpId + "-help.mdl";
    File f = StoredSettings::getInstance()->getDataDir() + "/help-patches/" + helpPatch;

    if(f.existsAsFile())
        SAMApplication::getApp()->openFile(f);
    else
        openHelpUrl(helpId);
}
void Utils::openDataDir()
{
#if JUCE_MAC
	Process::openDocument("/usr/bin/open", StoredSettings::getInstance()->getDataDir());
#else
	Process::openDocument(StoredSettings::getInstance()->getDataDir(), "");
#endif

}

void Utils::openFileExternal(const String& path)
{
    File f(path);
    if(f.existsAsFile())
    {
        StringArray tokens;
        tokens.addTokens(StoredSettings::getInstance()->getExternalEditor(), " ", "\"");
        if(tokens.size() == 0)
            return;
        tokens.add(f.getFullPathName());
        Process::openDocument(tokens[0], tokens.joinIntoString(" ", 1, -1));
    }
    else
    {
        DBG("The file " << f.getFullPathName() << " does not exist");
    }
}
void Utils::openFileNative(const String& path)
{
    File f(path);
    if(f.existsAsFile())
    {
#if JUCE_MAC
        Process::openDocument("/usr/bin/open", f.getFullPathName());
#else
        Process::openDocument("file:"+f.getFullPathName(), "");
#endif
    }
    else
    {
        DBG("The file " << f.getFullPathName() << " does not exist");
    }
}

// escapes particular characters
String Utils::fixPath(const String& path)
{
    if(path.containsAnyOf(" ()*?!"))
    {
        String tmp = path;
        tmp = tmp.replace(" ", "\\ ", true);
        tmp = tmp.replace("(", "\\(", true);
        tmp = tmp.replace(")", "\\)", true);
        tmp = tmp.replace("*", "\\*", true);
        tmp = tmp.replace("?", "\\?", true);
        tmp = tmp.replace("!", "\\!", true);
        return tmp;
    }
    return path;
}

bool Utils::writeStringToFile(const String& s, const File& f)
{
	TemporaryFile temp (f);

	ScopedPointer <FileOutputStream> out (temp.getFile().createOutputStream());

	if (out != nullptr)
	{
		out->write(s.toUTF8(), s.getNumBytesAsUTF8());
		out = nullptr; // (deletes the stream)

		bool succeeded = temp.overwriteTargetFileWithTemporary();
		return succeeded;
	}
	else
	{
		return false;
	}
}


int Utils::getCurrentTimeMillis()
{
    struct timeval tv;
    gettimeofday (&tv, nullptr);
    int millis = tv.tv_usec / 1000;

    return millis;
}


void Utils::uniquifyPaths(HashMap<String,String>& paths)
{
    // Get all file paths that have the same filename (i.e. /foo/bar.mdl and /bar.mdl)
    SortedSet<String> fileSet;
    for (HashMap<String, String>::Iterator i (paths); i.next();)
    {
        for (HashMap<String, String>::Iterator j(paths); j.next();)
        {
            if(i.getKey() != j.getKey())
            {
                File fi = i.getKey();
                File fj = j.getKey();
                if(fi.getFileName().compare(fj.getFileName()) == 0)
                {
                    fileSet.add(fj.getFullPathName());
                }
            }
        }
    }

    //if there are no same filenames just return
    if(fileSet.size() == 0)
        return;

    // Split paths to an Array
    HashMap<String, StringArray> sameFiles;
    for (int i = 0; i < fileSet.size(); ++i)
    {
        StringArray arr;
        arr.addTokens(fileSet[i], File::separatorString, "\"");
        sameFiles.set(fileSet[i], arr);
    }

    // Create unique paths by adding a parent directory to the files until
    // the path is unique
    bool uniquePaths = false;
    int lvl = 0;
    HashMap<String, String> sameFiles2;
    while(! uniquePaths)
    {
        SortedSet<String> tmpSet;
        ++lvl;
        for (HashMap<String, StringArray>::Iterator k(sameFiles); k.next();)
        {
            StringArray arr = k.getValue();
            int arrSize = arr.size();
            String tmpPath2;
            if(arrSize - lvl > 0)
            {
                StringArray tmpPath;
                for (int l = lvl; --l >= 0;)
                {
                    tmpPath.add(arr[arrSize - 1 - l]);
                }
                tmpPath2 = tmpPath.joinIntoString(File::separatorString);
            }
            else
            {
                tmpPath2 = k.getValue().joinIntoString(File::separatorString);
            }
            tmpSet.add(tmpPath2);
            sameFiles2.set(k.getKey(), tmpPath2);
        }
        if(tmpSet.size() == fileSet.size())
        {
            uniquePaths = true;
        }
    }
    for (HashMap<String, String>::Iterator it(sameFiles2); it.next();)
    {
        paths.set(it.getKey(), it.getValue());
    }
}

const String Utils::getSAMLog()
{
    File dataDir = StoredSettings::getInstance()->getDataDir();
    File samLogFile = dataDir.getChildFile("SAM-debug-compilation.txt");
    if(samLogFile.existsAsFile()
        && samLogFile.getLastModificationTime() <= SAMCmd::lastFaustCodeGenTime)
        return samLogFile.loadFileAsString();
    else
        return "No SAM Log!";
}

void Utils::setLocale()
{
    //    String currLocale = std::locale("").name().c_str();
    //    String currCountryCode = currLocale.substring(0, 2);
    //
    //    String localStr;
    //    if (currCountryCode.compare("de") == 0
    //        || currCountryCode.compare("at") == 0
    //        || currLocale.compare("ch") == 0)
    //    {
    //        localStr = String::createStringFromData(BinaryData::trans_german_txt,
    //                                                BinaryData::trans_german_txtSize);
    //    }

    //    if (localStr.isNotEmpty())
    //    {
    //        LocalisedStrings* localisedStrings = new LocalisedStrings(localStr, true);
    //        LocalisedStrings::setCurrentMappings(localisedStrings);
    //    }
}

//static const char* fileTypesToDelete[] = {".dsp", ".mdx", ".dsp.xml", ".cpp"};
void Utils::cleanDataDir(const MDLFile& mdlFile)
{
    File f = mdlFile.getFile();

    String dataDir = StoredSettings::getInstance()->getDataDir();
    String mdlName = f.getFileNameWithoutExtension();

    String outStrOk;
    String outStrError;

    for (int i = 0; i < 4; ++i)
    {
        String filePathToDelete = dataDir + "/" + mdlName
            + MDLController::fileTypesToDelete[i];
        File ftd(filePathToDelete);
        if (!ftd.existsAsFile())
            continue;
        if (ftd.moveToTrash())
            outStrOk << filePathToDelete << "\n";
        else
            outStrError << filePathToDelete << "\n";
    }

    if (outStrOk.isEmpty() && outStrError.isEmpty())
    {
        SAM_CONSOLE("MSG: No files were deleted.", PostLevel::ALL);
        return;
    }
    if (outStrOk.isNotEmpty())
        SAM_CONSOLE("Delete OK: " + outStrOk, PostLevel::ALL);
    if (outStrError.isNotEmpty())
        SAM_CONSOLE("Could not delete: " + outStrError, PostLevel::ERROR);

}

void Utils::cleanDataDirAll()
{
    StringArray filePathsToDelete;
    for (int i = 0; i < 4; ++i)
    {
        DirectoryIterator iter(StoredSettings::getInstance()->getDataDir(),
                               false,
                               "*" + String(MDLController::fileTypesToDelete[i]));
        while (iter.next())
        {
            filePathsToDelete.add(iter.getFile().getFullPathName());
        }
    }
    String outStrOk;
    String outStrError;
    for (int j = 0; j < filePathsToDelete.size(); ++j)
    {
        File f(filePathsToDelete[j]);
        if (!f.existsAsFile())
            continue;
        if (f.moveToTrash())
            outStrOk << filePathsToDelete[j] << "\n";
        else
            outStrError << filePathsToDelete[j] << "\n";
    }
    if (outStrOk.isEmpty() && outStrError.isEmpty())
    {
        SAM_CONSOLE("MSG: No files were deleted.", PostLevel::ALL);
        return;
    }
    if (outStrOk.isNotEmpty())
        SAM_CONSOLE("Delete OK: " + outStrOk, PostLevel::ALL);
    if (outStrError.isNotEmpty())
        SAM_CONSOLE("Could not delete: " + outStrError, PostLevel::ERROR);
}

//==============================================================================
#if UNIT_TESTS

#include "../../Testsuite/MiscUtilities_test.h"

#endif
