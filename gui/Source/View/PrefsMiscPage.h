/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.1

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_CAF4156327F718EE__
#define __JUCE_HEADER_CAF4156327F718EE__

//[Headers]     -- You can add your own extra header files here --
namespace synthamodeler
{
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MiscPage  : public Component,
                  public FilenameComponentListener,
                  public ButtonListener
{
public:
    //==============================================================================
    MiscPage ();
    ~MiscPage();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void filenameComponentChanged(FilenameComponent* fileComponentThatHasChanged);
    void readValues();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> laDatDir;
    ScopedPointer<FilenameComponent> fcDataDir;
    ScopedPointer<Label> laFaustDir;
    ScopedPointer<FilenameComponent> fcFaustDir;
    ScopedPointer<Label> laExternalEditor;
    ScopedPointer<FilenameComponent> fcExternalEditor;
    ScopedPointer<ToggleButton> tbAutoCorrect;
    ScopedPointer<ToggleButton> tbRunSAMBeforeExternal;
    ScopedPointer<ToggleButton> tbConfirmBeforeGeneration;
    ScopedPointer<ToggleButton> tbUseMDLX;
    ScopedPointer<ToggleButton> tbLogging;
    ScopedPointer<ToggleButton> tbRedrawWhenNoPos;
    ScopedPointer<ToggleButton> tbReopenLastModels;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiscPage)
};

//[EndFile] You can add extra defines here...
}
//[/EndFile]

#endif   // __JUCE_HEADER_CAF4156327F718EE__
