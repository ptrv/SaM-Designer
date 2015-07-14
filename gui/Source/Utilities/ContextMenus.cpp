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

#include "ContextMenus.h"

#include "Application/CommonHeaders.h"

#include "View/ObjectsHolder.h"


using namespace synthamodeler;

//------------------------------------------------------------------------------

void ContextMenus::showObjectsMenu(ObjectsHolder& objectsHolder,
                                   const Point<int>& pos)
{
    PopupMenu m;
    m.addSectionHeader(TRANS("Insert") + "...");
    m.addItem(MENU_ITEM_MASS, TRANS("Mass"));
    m.addItem(MENU_ITEM_GROUND, TRANS("Ground"));
    m.addItem(MENU_ITEM_RESONATORS, TRANS("Resonator"));
    m.addItem(MENU_ITEM_PORT, TRANS("Port"));
    m.addSeparator();
    m.addItem(MENU_ITEM_AUDIO_OUT, TRANS("Audio Out"));
    m.addItem(MENU_ITEM_DISPLAY, TRANS("Display"));
    m.addSeparator();
    m.addItem(MENU_ITEM_JUNCTION, TRANS("Junction"));
    m.addItem(MENU_ITEM_TERMINATION, TRANS("Termination"));
    m.addSeparator();
    //    bool commentEnabled = StoredSettings::getInstance()->getIsUsingMDLX();
    //    bool isUsingMDLX = mdlFile != nullptr ? mdlFile->getFile().hasFileExtension(".mdlx") : false;
    //    m.addItem(8, "Comment", (commentEnabled || isUsingMDLX));
    m.addItem(8, TRANS("Comment"));

    switch (m.show())
    {
    case MENU_ITEM_MASS:
        objectsHolder.insertNewObject(Ids::mass, pos);
        break;
    case MENU_ITEM_GROUND:
        objectsHolder.insertNewObject(Ids::ground, pos);
        break;
    case MENU_ITEM_RESONATORS:
        objectsHolder.insertNewObject(Ids::resonators, pos);
        break;
    case MENU_ITEM_PORT:
        objectsHolder.insertNewObject(Ids::port, pos);
        break;
    case MENU_ITEM_AUDIO_OUT:
        objectsHolder.insertNewObject(Ids::audioout, pos);
        break;
    case MENU_ITEM_DISPLAY:
        objectsHolder.insertNewObject(Ids::display, pos);
        break;
    case MENU_ITEM_JUNCTION:
        objectsHolder.insertNewObject(Ids::junction, pos);
        break;
    case MENU_ITEM_TERMINATION:
        objectsHolder.insertNewObject(Ids::termination, pos);
        break;
    case MENU_ITEM_COMMENT:
        objectsHolder.insertNewObject(Ids::comment, pos);
        break;
    }
}

//------------------------------------------------------------------------------

void ContextMenus::showLinkPopupMenu(ObjectsHolder& objectsHolder,
                                     String so, String eo)
{
    PopupMenu m;
    m.addSectionHeader(TRANS("Add") + "...");
    m.addItem(MENU_ITEM_LINK, TRANS("Linear Link"));
    m.addItem(MENU_ITEM_TOUCH, TRANS("Touch Link"));
    m.addItem(MENU_ITEM_PLUCK, TRANS("Pluck Link"));
    m.addItem(MENU_ITEM_PULSETOUCH, TRANS("Pulsetouch Link"));
    m.addItem(MENU_ITEM_DETENT, TRANS("Detent"));
    m.addItem(MENU_ITEM_SOFTENINGLINK, TRANS("Softening Link"));
    m.addItem(MENU_ITEM_STIFFENINGLINK, TRANS("Stiffening Link"));
    m.addSeparator();
    m.addItem(MENU_ITEM_WAVEGUIDE, TRANS("Waveguide"));
    m.addSeparator();
    m.addItem(MENU_ITEM_AUDIOCONNECTION, TRANS("Audio Connection"));
    m.addItem(MENU_ITEM_DISPLAYCONNECTION, TRANS("Display Connection"));

    switch (m.show())
    {
    case MENU_ITEM_LINK:
        DBG("Add link");
        objectsHolder.insertNewLink(Ids::link, so, eo);
        break;
    case MENU_ITEM_TOUCH:
        DBG("Add touch");
        objectsHolder.insertNewLink(Ids::touch, so, eo);
        break;
    case MENU_ITEM_PLUCK:
        DBG("Add pluck");
        objectsHolder.insertNewLink(Ids::pluck, so, eo);
        break;
    case MENU_ITEM_PULSETOUCH:
        DBG("Add pulsetouch");
        objectsHolder.insertNewLink(Ids::pulsetouch, so, eo);
        break;
    case MENU_ITEM_DETENT:
        DBG("Add detent");
        objectsHolder.insertNewLink(Ids::detent, so, eo);
        break;
    case MENU_ITEM_SOFTENINGLINK:
        DBG("Add softening link");
        objectsHolder.insertNewLink(Ids::softeninglink, so, eo);
        break;
    case MENU_ITEM_STIFFENINGLINK:
        DBG("Add stiffening link");
        objectsHolder.insertNewLink(Ids::stiffeninglink, so, eo);
        break;
    case MENU_ITEM_WAVEGUIDE:
        DBG("Add waveguide");
        objectsHolder.getStartEndObjectsLeftRight(so, eo);
        objectsHolder.insertNewLink(Ids::waveguide, so, eo);
        break;
    case MENU_ITEM_AUDIOCONNECTION:
        DBG("Add audio connection");
        objectsHolder.insertNewConnection(Ids::audioout);
        break;
    case MENU_ITEM_DISPLAYCONNECTION:
        DBG("Add display connection");
        objectsHolder.insertNewConnection(Ids::display);
        break;
    }
}

//------------------------------------------------------------------------------

void ContextMenus::showAudioConnectionPopupMenu(ObjectsHolder& objectsHolder)
{
    PopupMenu m;
    m.addSectionHeader(TRANS("Add") + "...");
    m.addItem(1, TRANS("Audio Connection"));
    if (m.show() == 1)
    {
        objectsHolder.insertNewConnection(Ids::audioout);
    }
}

//------------------------------------------------------------------------------
