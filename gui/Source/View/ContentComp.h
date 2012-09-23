/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  12 May 2012 12:35:30am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_CONTENTCOMP_CONTENTCOMP_20A12D4E__
#define __JUCER_HEADER_CONTENTCOMP_CONTENTCOMP_20A12D4E__

//[Headers]     -- You can add your own extra header files here --
class MainAppWindow;
class ObjController;
class ObjectsHolder;
class MDLFile;
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
    ContentComp (MainAppWindow& mainWindow_, ObjController& objController_);
    ~ContentComp();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	ApplicationCommandTarget* getNextCommandTarget();
	void getAllCommands(Array<CommandID>& commands);
	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result);
	bool perform(const InvocationInfo& info);

	void setMDLFile(MDLFile* newMDLFile);
    void updateMainAppWindowTitle(const String& newTitle);

    double getZoom() const;
    void setZoom (double newScale);
    void setZoom (double newScale, int anchorX, int anchorY);

    // convert a pos relative to this component into a pos on the editor
    void xyToTargetXY (int& x, int& y) const;

    void dragKeyHeldDown (bool isKeyDown);

//    void visibilityChanged();

    class MagnifierComponent;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    Viewport* viewport;
    MagnifierComponent* magnifier;

    MainAppWindow& mainWindow;
    ObjController& objController;
	ObjectsHolder* objectsHolder;
//	MDLFile* mdlFile;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    ContentComp (const ContentComp&);
    const ContentComp& operator= (const ContentComp&);
};


#endif   // __JUCER_HEADER_CONTENTCOMP_CONTENTCOMP_20A12D4E__
