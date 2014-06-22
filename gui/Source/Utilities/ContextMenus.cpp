/*
  ==============================================================================

    ContextMenus.cpp
    Created: February 4, 2014, 11:00 PM
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
#include "../View/ObjectsHolder.h"

#include "ContextMenus.h"

using namespace synthamodeler;

//------------------------------------------------------------------------------

void ContextMenus::showObjectsMenu(ObjectsHolder& objectsHolder,
                                   const Point<int>& pos)
{
    PopupMenu m;
    m.addSectionHeader(TRANS("Insert") + "...");
    m.addItem(1, TRANS("Mass"));
    m.addItem(2, TRANS("Ground"));
    m.addItem(3, TRANS("Resonator"));
    m.addItem(4, TRANS("Port"));
    m.addSeparator();
    m.addItem(5, TRANS("Audio Out"));
    m.addSeparator();
    m.addItem(6, TRANS("Junction"));
    m.addItem(7, TRANS("Termination"));
    m.addSeparator();
    //    bool commentEnabled = StoredSettings::getInstance()->getIsUsingMDLX();
    //    bool isUsingMDLX = mdlFile != nullptr ? mdlFile->getFile().hasFileExtension(".mdlx") : false;
    //    m.addItem(8, "Comment", (commentEnabled || isUsingMDLX));
    m.addItem(8, TRANS("Comment"));

    const int r = m.show();

    if (r == 1)
    {
        objectsHolder.insertNewObject(Ids::mass, pos);
    }
    else if (r == 2)
    {
        objectsHolder.insertNewObject(Ids::ground, pos);
    }
    else if (r == 3)
    {
        objectsHolder.insertNewObject(Ids::resonators, pos);
    }
    else if (r == 4)
    {
        objectsHolder.insertNewObject(Ids::port, pos);
    }
    else if (r == 5)
    {
        objectsHolder.insertNewObject(Ids::audioout, pos);
    }
    else if (r == 6)
    {
        objectsHolder.insertNewObject(Ids::junction, pos);
    }
    else if (r == 7)
    {
        objectsHolder.insertNewObject(Ids::termination, pos);
    }
    else if (r == 8)
    {
        objectsHolder.insertNewObject(Ids::comment, pos);
    }
}

//------------------------------------------------------------------------------

void ContextMenus::showLinkPopupMenu(ObjectsHolder& objectsHolder,
                                     String so, String eo)
{
    PopupMenu m;
    m.addSectionHeader(TRANS("Add") + "...");
    m.addItem(1, TRANS("Linear Link"));
    m.addItem(2, TRANS("Touch Link"));
    m.addItem(3, TRANS("Pluck Link"));
    m.addItem(4, TRANS("Pulsetouch Link"));
    m.addSeparator();
    m.addItem(5, TRANS("Waveguide"));
    m.addSeparator();
    m.addItem(6, TRANS("Audio Connection"));
    const int r = m.show();

    if (r == 1)
    {
        DBG("Add link");
        objectsHolder.insertNewLink(Ids::link, so, eo);
    }
    else if (r == 2)
    {
        DBG("Add touch");
        objectsHolder.insertNewLink(Ids::touch, so, eo);
    }
    else if (r == 3)
    {
        DBG("Add pluck");
        objectsHolder.insertNewLink(Ids::pluck, so, eo);
    }
    else if (r == 4)
    {
        DBG("Add pulsetouch");
        objectsHolder.insertNewLink(Ids::pulsetouch, so, eo);
    }
    else if (r == 5)
    {
        DBG("Add waveguide");
        objectsHolder.getStartEndObjectsLeftRight(so, eo);
        objectsHolder.insertNewLink(Ids::waveguide, so, eo);
    }
    else if (r == 6)
    {
        DBG("Add audio connection");
        objectsHolder.insertNewAudioConnection();
    }
}

//------------------------------------------------------------------------------

void ContextMenus::showAudioConnectionPopupMenu(ObjectsHolder& objectsHolder)
{
    PopupMenu m;
    m.addSectionHeader(TRANS("Add") + "...");
    m.addItem(1, TRANS("Audio Connection"));
    const int r = m.show();
    if (r == 1)
    {
        objectsHolder.insertNewAudioConnection();
    }
}

//------------------------------------------------------------------------------
