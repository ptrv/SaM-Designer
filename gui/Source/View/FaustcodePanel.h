/*
  ==============================================================================

    FaustcodePanel.h
    Created: 15 May 2012 7:50:53pm
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

#ifndef __FAUSTCODEPANEL_H_684205E3__
#define __FAUSTCODEPANEL_H_684205E3__

#include "JuceHeader.h"

namespace synthamodeler
{
class ObjController;
/**
 * Dialog window for editing defined variables.
 *
 */
class FaustcodePanel  : public DialogWindow
{
public:
    FaustcodePanel(ObjController* objController,
                   ValueTree data,
                   UndoManager* undoManager);
    ~FaustcodePanel();

    void closeButtonPressed();

    /**
     * Set FaustcodePanel visibele and read values from MDL file.
     *
     */
    void show();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FaustcodePanel)
};
}


#endif  // __FAUSTCODEPANEL_H_684205E3__
