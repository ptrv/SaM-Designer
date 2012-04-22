/*
  ==============================================================================

    MDLController.h
    Created: 12 Apr 2012 11:49:49pm
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

#ifndef __MDLCONTROLLER_H_8A72BC7D__
#define __MDLCONTROLLER_H_8A72BC7D__

class MDLFile;
class OutputCmd;

class MDLController
{
public:
	MDLController();
	~MDLController();

	void newFile();
	void open();
	void openFromFile(const File& mdlFile);
	void save();
	void saveAs();
	void close();

	const String generateFaust();
	const String generateExternal();
	const String& getMDLName();


private:
	FileBasedDocument::SaveResult saveIfNeededAndUserAgrees (MDLFile* mdl);
	ScopedPointer<MDLFile> currentMdl;
	ScopedPointer<OutputCmd> outCmd;
};


#endif  // __MDLCONTROLLER_H_8A72BC7D__
