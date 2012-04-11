/*
  ==============================================================================

    CommandIDs.h

    Created: 22 Jan 2012 8:49:22pm
    Author:  Peter Vasil

  ==============================================================================
*/

#ifndef __COMMANDIDS_H_5D051BD1__
#define __COMMANDIDS_H_5D051BD1__



/**
    A namespace to hold all the possible command IDs.
*/
namespace CommandIDs
{
    static const int newFile                = 0x200010;
    static const int open                   = 0x200020;
    static const int closeDocument          = 0x200030;
    static const int saveDocument           = 0x200040;
    static const int saveDocumentAs         = 0x200050;

    static const int segmentedConnectors    = 0x200051;
    static const int reverseDirection       = 0x200052;
    static const int showLabels             = 0x200053;

//    static const int insertMass             = 0x200060;
//    static const int insertGround           = 0x200061;
//    static const int insertResonator        = 0x200062;
//    static const int insertPort             = 0x200063;
//    static const int linkLinear             = 0x200064;
//    static const int linkTouch              = 0x200065;
//    static const int linkPluck              = 0x200066;
//    static const int audioOutput            = 0x200067;

    static const int saveProjectAs          = 0x200070;
    static const int openInIDE              = 0x200072;
    static const int saveAndOpenInIDE       = 0x200073;
    static const int showProjectSettings    = 0x200074;
    static const int updateModules          = 0x200075;
    static const int showOutputConsole      = 0x200076;

    static const int saveAll                = 0x200080;
    static const int undo                   = 0x200090;
    static const int redo                   = 0x2000a0;

    static const int closeWindow            = 0x201001;
    static const int closeAllDocuments      = 0x201000;

    static const int insertMass				= 0x202090;
    static const int insertGround			= 0x2020a0;
    static const int insertResonator        = 0x2030a1;
    static const int insertPort				= 0x2030b0;
    static const int insertLink				= 0x2030b1;
    static const int insertTouch			= 0x2030b2;
    static const int insertPluck			= 0x2030b3;
    static const int insertAudioOutput		= 0x2030c0;
    static const int insertWaveguide		= 0x2030c1;
    static const int insertTermination		= 0x2030c2;

    static const int generateFaust			= 0x2030c3;
    static const int generateExternal		= 0x2030c4;
//    static const int pointModeSymmetric     = 0x2030c5;

    static const int group                  = 0x202170;
    static const int ungroup                = 0x202180;

    static const int showPrefs              = 0x2020c0;
    static const int useTabbedWindows       = 0x2020d0;

    static const int showGrid               = 0x2020e0;
    static const int enableSnapToGrid       = 0x2020f0;

    static const int editCompLayout         = 0x202100;
    static const int editCompGraphics       = 0x202110;

    static const int bringBackLostItems     = 0x202120;

    static const int zoomIn                 = 0x202130;
    static const int zoomOut                = 0x202140;
    static const int zoomNormal             = 0x202150;
    static const int spaceBarDrag           = 0x202160;

    static const int compOverlay0           = 0x202200;
    static const int compOverlay33          = 0x202210;
    static const int compOverlay66          = 0x202220;
    static const int compOverlay100         = 0x202230;

    static const int newDocumentBase        = 0x322010;
    static const int newComponentBase       = 0x302010;
    static const int newElementBase         = 0x312010;
}

namespace CommandCategories
{
    static const char* const general       = "General";
    static const char* const editing       = "Editing";
    static const char* const inserting	   = "Inserting";
    static const char* const generation	   = "Generation";
    static const char* const tools		   = "Tools";
    static const char* const view          = "View";
    static const char* const windows       = "Windows";
}

#endif  // __COMMANDIDS_H_5D051BD1__
