/*
  ==============================================================================

    RedrawOptionsPanel.h
    Created: 18 Dec 2012 3:55:46pm
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

#ifndef __REDRAWOPTIONSPANEL_H_E7478E88__
#define __REDRAWOPTIONSPANEL_H_E7478E88__

namespace synthamodeler
{
//==============================================================================
/*
*/
class RedrawOptionsPanel    : public DialogWindow
{
public:
    RedrawOptionsPanel();
    ~RedrawOptionsPanel();

    void closeButtonPressed();

    /**
     * Opens the DialogWindow for editing defined variables.
     *
     * @param data 			the data of the editing object
     * @param undoManager   the UndoManager of the MDLFile consisting this object.
     */
    static void show();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RedrawOptionsPanel)
};

}

#endif  // __REDRAWOPTIONSPANEL_H_E7478E88__
