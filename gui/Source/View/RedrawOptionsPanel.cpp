/*
  ==============================================================================

    RedrawOptionsPanel.cpp
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

#include "../Application/CommonHeaders.h"
#include "RedrawOptionsComponent.h"

#include "RedrawOptionsPanel.h"

using namespace synthamodeler;

static String variablesWindowPos;
//==============================================================================
RedrawOptionsPanel::RedrawOptionsPanel()
: DialogWindow("Redraw Options", Colours::grey, true)
{
    RedrawOptionsComponent * const roc = new RedrawOptionsComponent(this);
    roc->setSize (400, 350);

    setContentOwned (roc, true);

    if (! restoreWindowStateFromString (variablesWindowPos))
        centreWithSize(getWidth(), getHeight());

    setResizable (false, false);
    setVisible(true);

}

RedrawOptionsPanel::~RedrawOptionsPanel()
{
}

void RedrawOptionsPanel::closeButtonPressed()
{
    setVisible (false);
}

void RedrawOptionsPanel::show()
{
    RedrawOptionsPanel rop;
    rop.runModalLoop();
}