/*
  ==============================================================================

    CommandIDs.h

    Created: 22 Jan 2012 8:49:22pm
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

#ifndef __COMMANDIDS_H_5D051BD1__
#define __COMMANDIDS_H_5D051BD1__



/**
    A namespace to hold all the possible command IDs.
*/
namespace synthamodeler
{
namespace CommandIDs
{
    static const int newFile                = 0x200010;
    static const int open                   = 0x200020;
    static const int closeDocument          = 0x200030;
    static const int saveDocument           = 0x200040;
    static const int saveDocumentAs         = 0x200045;
    static const int saveDocumentAsImage    = 0x200046;

    static const int segmentedConnectors    = 0x200051;
    static const int reverseDirection       = 0x200052;
    static const int defineFaustcode        = 0x200053;
    static const int showObjectNames        = 0x200054;

    static const int saveProjectAs          = 0x200070;
    static const int openMdlFileExtern      = 0x200071;
    static const int openDataDir			= 0x200072;
    static const int writeMDLFileAsXml      = 0x200073;
    static const int showProjectSettings    = 0x200074;
    static const int insertAudioConnection  = 0x200075;
    static const int showPostWindow         = 0x200076;
    static const int clearOutputConsole		= 0x200077;
    static const int printMDL               = 0x200078;
    static const int printSAMLog            = 0x200079;
    static const int showNextWindow         = 0x20007a;
    static const int showPreviousWindow     = 0x20007b;

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
    static const int insertPulsetouch       = 0x2030b3;
    static const int insertPluck			= 0x2030b4;
    static const int insertDetent           = 0x2030b5;
    static const int insertSofteningLink    = 0x2030b6;
    static const int insertStiffeningLink   = 0x2030b7;
    static const int insertAudioOutput		= 0x2030c0;
    static const int insertWaveguide		= 0x2030c1;
    static const int insertTermination		= 0x2030c2;
    static const int insertJunction 		= 0x2030c3;
    static const int insertComment          = 0x2030c4;
    static const int insertDisplay          = 0x203100;
    static const int insertDisplayConnection = 0x203101;

    static const int generateFaust			= 0x2030c5;
    static const int generateExternal		= 0x2030c6;
    static const int cleanDataDir           = 0x2030c7;
    static const int cleanDataDirAll        = 0x2030c8;

    static const int showPrefs              = 0x2020c0;
    static const int showHelp				= 0x2020d0;

    static const int showGrid               = 0x2020e0;
    static const int enableSnapToGrid       = 0x2020f0;

    static const int zoomIn                 = 0x202130;
    static const int zoomOut                = 0x202140;
    static const int zoomNormal             = 0x202150;
    static const int spaceBarDrag           = 0x202160;
    static const int tidyObjects            = 0x202170;
    static const int redrawCircle           = 0x202171;
    static const int redrawForceDirected    = 0x202172;
    static const int showRedrawOptions      = 0x202173;
    static const int showMDLProperties      = 0x202174;
    static const int toggleRedrawIncludeMisc = 0x202175;

    static const int moveUp                 = 0x202200;
    static const int moveDown               = 0x202210;
    static const int moveLeft               = 0x202220;
    static const int moveRight              = 0x202230;
    static const int moveUpFine             = 0x202400;
    static const int moveDownFine           = 0x202410;
    static const int moveLeftFine           = 0x202420;
    static const int moveRightFine          = 0x202430;

    static const int selectTextColour       = 0x202710;
    static const int showAudioConnections   = 0x202720;

    static const int showPropertiesWindow   = 0x202810;
    static const int propertiesWindowOnTop  = 0x202820;
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
    static const char* const help          = "help";
}

}

#endif  // __COMMANDIDS_H_5D051BD1__
