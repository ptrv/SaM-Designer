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

#include "MiscUtilities.h"

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
	URL helpUrl("https://github.com/ptrv/Synth-A-Modeler/wiki");
	helpUrl.launchInDefaultBrowser();
}

void openHelpUrl(const Identifier& objId)
{
    String objStr = getObjectTypeAsString(objId).toLowerCase();
    objStr.append("_object", 256);
	URL helpUrl("https://github.com/ptrv/Synth-A-Modeler/wiki/" + objStr);
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

	String tmpVal = paramVal;
	if(tmpVal.indexOf(".") == -1)
		tmpVal << ".0";

	return tmpVal;
}

}