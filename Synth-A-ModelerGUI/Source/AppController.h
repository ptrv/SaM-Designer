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

#include "../JuceLibraryCode/JuceHeader.h"
#include "OutputCmd.h"
class DebugWindow;
class MainAppWindow;

class AppController {
public:
	AppController(MainAppWindow& maw_, DebugWindow& dw_);
	~AppController();

	void generateFaust();
	void generateExternal();
	void debugWindowToggle();
	void debugWindowClear();
private:
	MainAppWindow& maw;
	DebugWindow& dw;
	ScopedPointer<OutputCmd> outCmd;
};


#endif  // __APPCONTROLLER_H_B0B552E9__
