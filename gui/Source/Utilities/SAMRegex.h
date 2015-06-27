/*
  ==============================================================================

    SAMRegex.h
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

#ifndef __SAMREGEX_H_36A909E0__
#define __SAMREGEX_H_36A909E0__

#include "JuceHeader.h"

namespace synthamodeler
{

class SAMRegex
{
private:
    SAMRegex(){}
    ~SAMRegex(){}
public:

    static const char* myLabel;
    static const char* myLabels;
    static const char* myParam;
    static const char* myParams;
    static const char* myVertex;
    static const String getVertexLine();
    static const char* myLink;
    static const String getLinkLine();
    static const char* myAudioOutDetails;
    static const String getAudioOutLine();
    static const char* myFaustCode;
    static const String getFaustCodeLine();
    static const String getInternalFaustCodeLine();
    static const char* myAdjustIndex;
    static const String getAdjustLine();
    static const String getTerminationLine();
    static const String getWaveguideLine();
    static const char* myJunctionInteraction;
    static const String getJunctionInteractionLine();
    static const String getJunctionLine();
    static const char* myDisplayAttach;
    static const String getDisplayAttachLine();
    static const String getCommentObjectLine();
};
}


#endif  // __SAMREGEX_H_36A909E0__
