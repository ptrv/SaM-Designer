/*
  ==============================================================================

    AppController.h
    Created: 12 Apr 2012 1:30:25am
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

#ifndef __APPCONTROLLER_H_B0B552E9__
#define __APPCONTROLLER_H_B0B552E9__

#include "../../JuceLibraryCode/JuceHeader.h"
#include "MDLController.h"
#include "ObjController.h"
#include "../View/DebugWindow.h"
class SynthAModelerApplication;
//class DebugWindow;
class MainAppWindow;

class AppController
{
public:
	AppController(SynthAModelerApplication& app_, DebugWindow& dw_);
	~AppController();

	bool menuItemWasClicked(CommandID menuId);
	void openMDL(const File& mdlFile);

	void setMainWindowTitle();

	bool perform (UndoableAction* const action, const String& actionName);

	MDLController* const getMDLController() { return mdlController; }
	ObjController* const getObjController() { return objController; }

	bool mdlCheckAndSave();

	UndoManager* getUndoManager();
//	void addMainWindow(MainAppWindow* maw_);
//	void removeMainWindow(MainAppWindow* maw_);
private:
	friend class MDLController;
	friend class ObjController;

	void debugWindowToggle();
	void debugWindowClear();

//	MainAppWindow& maw;
	SynthAModelerApplication& app;
	DebugWindow& dw;

    ScopedPointer<MDLController> mdlController;
    ScopedPointer<ObjController> objController;

//    ScopedPointer<UndoManager> undoMgr;
//	ScopedPointer<OutputCmd> outCmd;
};


#endif  // __APPCONTROLLER_H_B0B552E9__
