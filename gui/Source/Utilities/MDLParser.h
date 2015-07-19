/*
  ==============================================================================

    MDLParser.h
    Created: 16 Apr 2012 1:04:09am
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

#ifndef __MDLPARSER_H_DC928BC0__
#define __MDLPARSER_H_DC928BC0__

#include "JuceHeader.h"

namespace synthamodeler
{
class MDLFile;
class RegularExpression;
/**
 * MDL parser class.
 */
class MDLParser
{
public:
    MDLParser(MDLFile& mdlFile_);

    /**
     * Parses the MDLFile
     *
     * @return		true if parsing succeeded
     */
    bool parseMDL(const File& f);
    bool parseMDLX(const File& f, bool onlyExtras=false);

private:

    void addTree(ValueTree& rootTree, const ValueTree& newTree);

    ValueTree getMassTree(const String& line, RegularExpression& re);
    ValueTree getLinkTree(const String& line, RegularExpression& re);
    ValueTree getFaustCodeTree(const String& line, RegularExpression& re);
    ValueTree getAudioOutTree(const String& line, RegularExpression& re);
    ValueTree getWaveguideTree(const String& line, RegularExpression& re);
    ValueTree getTerminationTree(const String& line, RegularExpression& re);
    ValueTree getJunctionTree(const String& line, RegularExpression& re);
    ValueTree getCommentTree(const String& line, RegularExpression& re);
    ValueTree getDisplayTree(const String& line, RegularExpression& re);

    MDLFile& mdlFile;
};
}


#endif  // __MDLPARSER_H_DC928BC0__
