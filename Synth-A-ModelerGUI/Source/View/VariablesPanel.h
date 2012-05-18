/*
  ==============================================================================

    VariablesPanel.h
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

#ifndef __VARIABLESPANEL_H_684205E3__
#define __VARIABLESPANEL_H_684205E3__

/**
 * Dialog window for editing defined variables.
 *
 */
class VariablesPanel  : public DialogWindow
{
public:
	VariablesPanel(ValueTree data, UndoManager* undoManager);
    ~VariablesPanel();

    void closeButtonPressed();

    /**
     * Opens the DialogWindow for editing defined variables.
     *
     * @param data 			the data of the editing object
     * @param undoManager   the UndoManager of the MDLFile consisting this object.
     */
    static void show(ValueTree data, UndoManager* undoManager);
};


#endif  // __VARIABLESPANEL_H_684205E3__
