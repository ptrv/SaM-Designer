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

namespace synthamodeler
{
class MDLFile;
class SAMCmd;
class AppController;
class MainAppWindow;

/**
 * The MDLController controlls all actions for the MDLFile.
 */
class MDLController
{
public:
	MDLController(MainAppWindow& mainAppWindow_);
	~MDLController();

    static const char* fileTypesToDelete[];

	void newFile();
	void open();
	void openFromFile(const File& mdlFile);
	void save();
	void saveAs();
	void saveAsImage();
	void close();

    bool saveAsXml();

	const String generateFaust();
	const String generateExternal();
	const String getMDLName();

    void cleanDataDir();
    void cleanDataDirAll();
    
	UndoManager* getUndoManager();

	bool perform (UndoableAction* const action, const String& actionName);

	ValueTree getMDLTree();
	bool mdlCheckAndSaveIfNeeded();
	MDLFile* getMDLFile() const;
	void setMDLFile(MDLFile* mdlFile);

    void changed();

    ObjectsHolder* getHolderComponent();

    bool checkIfMdlCanchedOutside();
private:
	MainAppWindow& mainAppWindow;
	ScopedPointer<MDLFile> currentMdl;
	ScopedPointer<SAMCmd> samCmd;
};
}

#endif  // __MDLCONTROLLER_H_8A72BC7D__
