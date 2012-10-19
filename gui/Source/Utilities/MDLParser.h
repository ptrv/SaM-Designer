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

namespace synthamodeler
{
class MDLFile;
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
    bool parseMDLX(const File& f, bool onlyMDLX);
private:
	MDLFile& mdlFile;
};
}


#endif  // __MDLPARSER_H_DC928BC0__
