/*
  ==============================================================================

    MDLWriter.h
    Created: 22 Apr 2012 1:07:57am
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

#ifndef __MDLWRITER_H_CB9D9CEF__
#define __MDLWRITER_H_CB9D9CEF__

namespace synthamodeler
{
class MDLFile;
/**
 * MDL writer class. Writes current oatch to a file.
 */
class MDLWriter
{
public:
    MDLWriter(const MDLFile& mdlFile_);

	/**
	 * Writes MDLFile to specified file.
	 *
	 * @param saveFile		File to write the MDLFile to
	 * @return				true if writing succeeded
	 */
	bool writeMDL(const File& saveFile);
    
    bool writeMDLX(const File& saveFile);

private:
    void getMDLString(String& mdlContent);
    const MDLFile& mdlFile;
};
}


#endif  // __MDLWRITER_H_CB9D9CEF__
