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

namespace synthamodeler
{
class SAMRegex
{
private:
    SAMRegex(){}
    ~SAMRegex(){}
public:
    static const char* pos;
    static const char* label;
    static const char* labels;
    static const char* param;
    static const char* params;
    static const char* vertex;
    static const char* link;
    static const char* audioOutDetails;
    static const char* faustName;
    static const char* faustCode;
    static const char* paramsDetail;
    static const char* word;
    
    static String getVertexLine();
    static String getLinkLine();
    static String getAudioOutLine();
    static String getFaustLine();
    static String getTerminationLine();
    static String getJunctionLine();
    static String getWaveguideLine();
    static String getParamsLine(int numParams);
};
}


#endif  // __SAMREGEX_H_36A909E0__
