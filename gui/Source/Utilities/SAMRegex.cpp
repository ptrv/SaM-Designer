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

#include "../../JuceLibraryCode/JuceHeader.h"

#include "SAMRegex.h"

using namespace synthamodeler;

const char* SAMRegex::pos = "(\\s*#\\s*pos\\s*\\d+,\\s*\\d+\\s*)?";
const char* SAMRegex::label = "([a-zA-Z\\d]*)";
const char* SAMRegex::labels = "([a-zA-Z,\\d\\s]*)";
const char* SAMRegex::param = "(\\s*[^\\n\\r\\a\\033\\f,]*\\s*)";
const char* SAMRegex::params = "(\\s*[^\\n\\r\\a\\033\\f]*\\s*)";
const char* SAMRegex::vertex = "(mass|port|ground|resonators)";
const char* SAMRegex::link = "(link|pluck|touch|pulsetouch)";
const char* SAMRegex::audioOutDetails = "(.+)";
const char* SAMRegex::faustName = "\\A\\s*(faustcode):.*";
//const char* SAMRegex::faustCode = "(.+)\\s*=\\s*(.+)";
const char* SAMRegex::faustCode = "(.+)";
//const char* SAMRegex::paramsDetail = "([-\\+\\.a-zA-Z\\d\\*]+|[a-zA-Z]*\\([a-zA-Z\\.\\d,\\s*]*\\))"";
const char* SAMRegex::paramsDetail = "([\\d\\.]*|[\\d\\.\\*a-zA-Z]*|[\\(\\)\\*\\+\\-a-zA-Z\\.\\d,/:\\s*]*|[\\w\\\\\"\\s[:graph:]]*)";
// resonators paramsDetail
const char* SAMRegex::paramsDetailRes = "([\\d\\.\\*a-zA-Z]*)";
const char* SAMRegex::word = "(\\w+)";
const char* SAMRegex::commentObject = "\\A\\s*#\\s*(comment)";

String SAMRegex::getVertexLine()
{
    static const String vertexLine =
        String::formatted(
            "\\A\\s*%s\\(\\s*%s\\s*\\)\\s*,\\s*%s\\s*;(.*)$",
            vertex, params, label);
    return vertexLine;
}

String SAMRegex::getLinkLine()
{
    static const String linkLine =
        String::formatted(
            "\\A\\s*%s\\(\\s*%s\\s*\\)\\s*,\\s*%s\\s*,\\s*%s\\s*,\\s*%s\\s*;.*$",
            link, params, label, label, label);
    return linkLine;
}

String SAMRegex::getAudioOutLine()
{
    static const String aoLine =
        String::formatted("\\A\\s*(audioout)\\s*,\\s*%s\\s*,%s;(.*)$",
                          label, audioOutDetails);
    return aoLine;
}

String SAMRegex::getFaustLine()
{
    static const String faustLine =
        String::formatted("\\A\\s*(faustcode):\\s*%s\\s*$", faustCode);
    return faustLine;
}

String SAMRegex::getTerminationLine()
{
    static const String termLine =
        String::formatted(
            "\\A\\s*(termination)\\(\\s*%s\\s*\\)\\s*,\\s*%s\\s*;(.*)$",
            params, label);
    return termLine;
}

String SAMRegex::getJunctionLine()
{
    static const String junctLine =
        String::formatted(
            "\\A\\s*(junction)\\(\\s*%s\\s*\\)\\s*,\\s*%s\\s*;(.*)$",
            params, label);
    return junctLine;
}

String SAMRegex::getWaveguideLine()
{
    static const String waveguideLine =
        String::formatted(
            "\\A\\s*(waveguide)\\(\\s*%s\\s*\\)\\s*,\\s*%s\\s*,\\s*%s\\s*,\\s*%s\\s*;.*$",
            params, label, label, label);
    return waveguideLine;
}

String SAMRegex::getParamsLine(int numParams)
{
    String paramsLine;
    for (int i = 0; i < numParams; ++i)
    {
        paramsLine << "\\s*" << paramsDetail << "\\s*";
        if(i != numParams - 1)
            paramsLine << ",";
    }
    return paramsLine;
}

String SAMRegex::getCommentObjectLine()
{
    static const String commentObjectLine =
        String::formatted(
            "\\A\\s*##\\s*(comment)\\(\\s*([!-~\\s]*)\\s*\\)\\s*,\\s*%s\\s*;\\s*(.*)$",
            label);
    return commentObjectLine;
}
