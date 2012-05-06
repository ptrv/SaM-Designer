/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  16 Apr 2012 1:46:30am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_CONTENTCOMP_CONTENTCOMP_67843116__
#define __JUCER_HEADER_CONTENTCOMP_CONTENTCOMP_67843116__

//[Headers]     -- You can add your own extra header files here --
#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Application/MainWindow.h"
#include "ObjectsHolder.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]

    Main component which holds all menu items and the objects component.

                                                                    //[/Comments]
*/
class ContentComp  : public Component,
                     public ApplicationCommandTarget
{
public:
    //==============================================================================
    ContentComp (MainAppWindow& mainWindow_, AppController& appController_);
    ~ContentComp();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	ApplicationCommandTarget* getNextCommandTarget();
	void getAllCommands(Array<CommandID>& commands);
	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result);
	bool perform(const InvocationInfo& info);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    MainAppWindow& mainWindow;
	AppController& appController;
	ObjectsHolder* objectsHolder;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    ContentComp (const ContentComp&);
    const ContentComp& operator= (const ContentComp&);
};


#endif   // __JUCER_HEADER_CONTENTCOMP_CONTENTCOMP_67843116__
