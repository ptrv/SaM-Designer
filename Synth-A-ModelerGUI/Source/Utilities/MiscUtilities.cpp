/*
  ==============================================================================

    MiscUtilities.cpp
    Created: 12 May 2012 3:38:26pm
    Author:  peter

  ==============================================================================
*/
#include "../../JuceLibraryCode/JuceHeader.h"
#include "StoredSettings.h"

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

void openDataDir()
{
#if JUCE_MAC
	Process::openDocument("/usr/bin/open", StoredSettings::getInstance()->getDataDir());
#else
	Process::openDocument("file:"+StoredSettings::getInstance()->getDataDir(), "");
#endif

}


}
