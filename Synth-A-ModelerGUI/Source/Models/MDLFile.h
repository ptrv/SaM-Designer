/*
  ==============================================================================

    MDLFile.h
    Created: 11 Apr 2012 3:18:35pm
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

#ifndef __MDLFILE_H_70428F9D__
#define __MDLFILE_H_70428F9D__


#include "../../JuceLibraryCode/JuceHeader.h"

class MDLFile : public FileBasedDocument
{
//	friend class MDLParser;
public:
	MDLFile();
	~MDLFile();

	void newMDL();
	void close();

	const String& getFilePath() const { return mdlPath; }
	const String getName() { return isModified ? mdlName+"*" : mdlName; }

	ValueTree mdlRoot;
protected:
	const String getDocumentTitle();
	const String loadDocument (const File& file);
	const String saveDocument (const File& file);
	const File getLastDocumentOpened();
	void setLastDocumentOpened (const File& file);

private:

	void initMDL();
	void destroyMDL();

	bool isModified;
	bool isInit;

	String mdlPath;
	String mdlName;
	static File lastDocumentOpened;

};


#endif  // __MDLFILE_H_70428F9D__
