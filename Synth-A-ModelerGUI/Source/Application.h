/*
  ==============================================================================

    Application.h
    Created: 11 Apr 2012 1:28:10pm
    Author:  Peter Vasil

  ==============================================================================
*/

#ifndef __APPLICATION_H_BBAD6F5C__
#define __APPLICATION_H_BBAD6F5C__


//#include "../JuceLibraryCode/JuceHeader.h"
#include "MainWindow.h"


//==============================================================================
class SynthAModelerApplication  : public JUCEApplication
{
public:
    //==============================================================================
    SynthAModelerApplication();
    ~SynthAModelerApplication();

    //==============================================================================
    void initialise (const String& commandLine);
    void shutdown();
    //==============================================================================
    void systemRequestedQuit();
    //==============================================================================
    const String getApplicationName();
    const String getApplicationVersion();
    bool moreThanOneInstanceAllowed();
    void anotherInstanceStarted (const String& commandLine);
private:
    ScopedPointer <MainAppWindow> mainWindow;
};



#endif  // __APPLICATION_H_BBAD6F5C__
