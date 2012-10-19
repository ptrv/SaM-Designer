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
#include "../../JuceLibraryCode/JuceHeader.h"
#include "StoredSettings.h"
#include "../Models/ObjectIDs.h"
#include "../View/BaseObjectComponent.h"
#include "../View/ObjectComponent.h"
#include "../View/LinkComponent.h"
#include "../Controller/ObjController.h"

#include "MiscUtilities.h"
#include "RegularExpression.h"

namespace Utils
{

bool cancelAnyModalComponents()
{
    const int numModal = ModalComponentManager::getInstance()->getNumModalComponents();

    for (int i = numModal; --i >= 0;)
        if (ModalComponentManager::getInstance()->getModalComponent(i) != nullptr)
            ModalComponentManager::getInstance()->getModalComponent(i)->exitModalState (0);

    return numModal > 0;
}

void SAMLogger(const String& message)
{
String timeString;
//	timeString << "[" << Time::getCurrentTime().formatted("%H:%M:%S")
	timeString << "[" << Time::getCurrentTime().formatted("%c") << "]: ";
	String tmp;
	tmp << timeString << message;
	Logger::writeToLog(tmp);
}

FileLogger* getLogger()
{
#if JUCE_LINUX
	String subDir = ".Synth-A-Modeler";
#else
	String subDir = "";
#endif

	return FileLogger::createDefaultAppLogger(subDir,
			"Synth-A-Modeler.log", "Synth-A-Modeler logfile");
}

void openHelpUrl()
{
//	URL helpUrl("https://github.com/ptrv/Synth-A-Modeler/wiki");
	URL helpUrl("http://www.user.tu-berlin.de/peter.vasil/synthamodelergui-help/");
	helpUrl.launchInDefaultBrowser();
}

void openHelpUrl(const String& helpId)
{
//	URL helpUrl("https://github.com/ptrv/Synth-A-Modeler/wiki/" + helpId);
	URL helpUrl("http://www.user.tu-berlin.de/peter.vasil/synthamodelergui-help/" + helpId + ".html");
	helpUrl.launchInDefaultBrowser();
}

void openDataDir()
{
#if JUCE_MAC
	Process::openDocument("/usr/bin/open", StoredSettings::getInstance()->getDataDir());
#else
	Process::openDocument("file:"+StoredSettings::getInstance()->getDataDir(), "");
#endif

}

void openFileNative(const String& path)
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

String getObjectTypeAsString(const Identifier& objType)
{
	if(objType == Ids::mass)
		return "Mass";
	else if(objType == Ids::port)
		return "Port";
	else if(objType == Ids::ground)
		return "Ground";
	else if(objType == Ids::resonator)
		return "Resonator";
	else if(objType == Ids::link)
		return "Link";
	else if(objType == Ids::touch)
		return "Touch";
	else if(objType == Ids::pluck)
		return "Pluck";
	else if(objType == Ids::audioout)
		return "AudioOut";
	else if(objType == Ids::waveguide)
		return "Waveguide";
	else if(objType == Ids::termination)
		return "Termination";
	else if(objType == Ids::junction)
		return "Junction";
	else
		return String::empty;
}

String fixParameterValueIfNeeded(const String& paramVal)
{
	if( paramVal == String::empty)
		return "0.0";

	String tmpVal;
    StringArray operators;
    StringArray params;
    if(paramVal.containsAnyOf("*+-/"))
    {
        String tmp = "";
        for (int i = 0; i < paramVal.length(); ++i)
        {
            if(paramVal[i] == '*' || paramVal[i] == '+'
                || paramVal[i] == '-' || paramVal[i] == '/')
            {
                String op = "";
                op << paramVal[i];
                operators.add(op);
                params.add(tmp);
                tmp = "";
            }
            else
            {
                tmp << paramVal[i];
            }
        }
        if(tmp.compare("") != 0)
            params.add(tmp);

        for (int i = 0; i < params.size(); ++i)
        {
            tmpVal << params[i];
            if(params[i].containsOnly("0123456789") && params[i].indexOf(".") == -1)
                tmpVal << ".0";
            if(i < operators.size())
                tmpVal << operators[i];
        }
    }
    else
    {
        tmpVal = paramVal;
        if (tmpVal.containsOnly("0123456789") && tmpVal.indexOf(".") == -1)
            tmpVal << ".0";
    }

	return tmpVal;
}

String getGainForSourceId(ValueTree& sources, const String& sourceId)
{
    String gainStr;
    for (int i = 0; i < sources.getNumChildren(); ++i)
    {
        ValueTree source = sources.getChild(i);
        StringArray vals;
        vals.addTokens(source[Ids::value].toString(), "*", "\"");
        int srcIdx = -1;
        for (int j = 0; j < vals.size(); ++j)
        {
            if(vals[j].compare(sourceId) == 0)
            {
                srcIdx = j;
                break;
            }
        }
        if(srcIdx != -1)
        {
            int gainVals = 0;
            for (int j = 0; j < vals.size(); ++j)
            {
                if(j != srcIdx)
                {
                    if (gainVals >= 1)
                        gainStr << "*";
                    gainStr << vals[j];
                    ++ gainVals;
                }
            }
            break;
        }
    }
    return gainStr;
}

void setGainForSourceId(ValueTree& sources, const String& sourceId,
                        const String& gainVal, UndoManager* undoManager)
{
    String gainStr;
    for (int i = 0; i < sources.getNumChildren(); ++i)
    {
        ValueTree source = sources.getChild(i);
        StringArray vals;
        vals.addTokens(source[Ids::value].toString(), "*", "\"");
        int srcIdx = -1;
        for (int j = 0; j < vals.size(); ++j)
        {
            if(vals[j].compare(sourceId) == 0)
            {
                srcIdx = j;
                break;
            }
        }
        if(srcIdx != -1)
        {
            gainStr << sourceId << "*";
            gainStr << gainVal;
            source.setProperty(Ids::value, gainStr, undoManager);
            break;
        }
    }
}

BaseObjectComponent* getBaseObjectFromSource(ObjController* objController, ValueTree& source)
{
    String srcVal = source[Ids::value].toString();
    StringArray srcArray;
    srcArray.addTokens(srcVal, "*", "\"");
    for (int i = 0; i < srcArray.size(); ++i)
    {
        if (ObjectComponent* oc = objController->getObjectForId(srcArray[i]))
        {
            return oc;
        }
        else if(LinkComponent* lc = objController->getLinkForId(srcArray[i]))
        {
            return lc;
        }
    }
    return nullptr;
}

const Identifier& getObjectGroup(const Identifier& ident)
{
    if(ident == Ids::mass || ident == Ids::port
            || ident == Ids::ground	|| ident == Ids::resonator)
        return Objects::masses;
    else if(ident == Ids::link || ident == Ids::touch
            || ident == Ids::pluck)
        return Objects::links;
    else if(ident == Ids::audioout)
        return Objects::audioobjects;
    else if(ident == Ids::waveguide)
        return Objects::waveguides;
    else if(ident == Ids::termination)
        return Objects::terminations;
    else if(ident == Ids::junction)
        return Objects::junctions;
    else if(ident == Ids::variable)
        return Objects::variables;

    else
        return Objects::invalid;
}

// escapes particular characters
String fixPath(const String& path)
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

}
