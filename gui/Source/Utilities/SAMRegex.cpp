/*
  ==============================================================================

    SAMRegex.cpp
    Created: 21 Sep 2012 12:52:17am
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

#include "SAMRegex.h"

using namespace synthamodeler;

const char* SAMRegex::myLabel = "([a-zA-Z\\d]*)";
const char* SAMRegex::myLabels = "([a-zA-Z,\\d\\s]*)";
const char* SAMRegex::myParam = "(\\s*[^\\n\\r\\a\\e\\f,]*\\s*)";
const char* SAMRegex::myParams = "(\\s*[^\\n\\r\\a\\e\\f]*\\s*)";

const char* SAMRegex::myLink = "(link|pluck|touch|linkCA|BUT|pulseTouch|pulseTouchGeneral|pulseTouchTable|detent|stiffeninglink|softeninglink)";

const String SAMRegex::getLinkLine()
{
    static const String myLinkLine =
        String::formatted("\\A\\s*%s\\(\\s*%s\\s*\\)\\s*,\\s*%s\\s*,\\s*%s\\s*,\\s*%s\\s*;\\s*$",
                          myLink, myParams, myLabel, myLabel, myLabel);
    return myLinkLine;
}

const char* SAMRegex::myVertex = "(mass|port|ground|resonators|massCA)";

const String SAMRegex::getVertexLine()
{
    static const String myVertexLine =
        String::formatted("\\A\\s*%s\\(\\s*%s\\s*\\)\\s*,\\s*%s\\s*;\\s*(.*)$",
                          myVertex, myParams, myLabel);
    return myVertexLine;
}

const char* SAMRegex::myAudioOutDetails = "(.+)";

const String SAMRegex::getAudioOutLine()
{
    static const String myAudioOutLine =
        String::formatted("\\A\\s*audioout\\s*,\\s*%s\\s*,%s;\\s*(.*)$", myLabel, myAudioOutDetails);
    return myAudioOutLine;
}

const char* SAMRegex::myFaustCode = "(.+)";

const String SAMRegex::getFaustCodeLine()
{
    static const String myFaustCodeLine =
        String::formatted("\\A\\s*faustcode:\\s*%s$", myFaustCode);
    return myFaustCodeLine;
}

const char* SAMRegex::myAdjustIndex = "([\\d]+)";

const String SAMRegex::getAdjustLine()
{
    static const String myAdjustLine =
        String::formatted("\\A\\s*controladjust\\s*,\\s*%s\\s*,\\s*%s\\s*:\\s*%s\\s*;\\s*$",
                          myLabel, myLabel, myAdjustIndex);
    return myAdjustLine;
}

const String SAMRegex::getTerminationLine()
{
    static const String myTerminationLine =
        String::formatted("\\A\\s*termination\\(\\s*%s\\s*\\)\\s*,\\s*%s\\s*;\\s*(.*)$",
                          myParams, myLabel);
    return myTerminationLine;
}

const String SAMRegex::getWaveguideLine()
{
    static const String myWaveguideLine =
        String::formatted("\\A\\s*waveguide\\(\\s*%s\\s*,\\s*%s\\s*\\)\\s*,\\s*%s\\s*,\\s*%s\\s*,\\s*%s\\s*;\\s*$",
                          myParam, myParams, myLabel, myLabel, myLabel);
    return myWaveguideLine;
}


const char* SAMRegex::myJunctionInteraction = "(junctionlink|junctionlinkUnderneath|junctiontouch|junctiontouchUnderneath|junctionpluck|junctionpluckUnderneath)";

const String SAMRegex::getJunctionInteractionLine()
{
    static const String myJunctionInteractionLine =
        String::formatted("\\A\\s*%s\\(\\s*%s\\s*\\)\\s*,\\s*%s\\s*,\\s*%s\\s*;\\s*$",
                          myJunctionInteraction, myParams, myLabel, myLabel);
    return myJunctionInteractionLine;
}

const String SAMRegex::getJunctionLine()
{
    static const String myJunctionLine =
        String::formatted("\\A\\s*junction\\(\\s*%s\\s*\\)\\s*,\\s*%s\\s*;\\s*(.*)$",
                          myParams, myLabel);
    return myJunctionLine;
}

const char* SAMRegex::myDisplayAttach = "(display)";

const String SAMRegex::getDisplayAttachLine()
{
    static const String myDisplayAttachLine =
        String::formatted("\\A\\s*display\\(\\s*%s\\s*\\)\\s*,\\s*%s\\s*,\\s*%s\\s*;\\s*(.*)$",
                          myParams, myLabel, myLabel);
    return myDisplayAttachLine;
}

const String SAMRegex::getCommentObjectLine()
{
    static const String commentObjectLine =
        String::formatted(
            "\\A\\s*##\\s*comment\\(\\s*([!-~\\s]*)\\s*\\)\\s*,\\s*%s\\s*;\\s*(.*)$",
            myLabel);
    return commentObjectLine;
}
