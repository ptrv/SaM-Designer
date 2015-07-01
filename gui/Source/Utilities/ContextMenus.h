/* 
  ==============================================================================

    ContextMenus.h
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

#ifndef CONTEXTMENUS_H
#define	CONTEXTMENUS_H

#include "JuceHeader.h"

namespace synthamodeler
{

class ObjectsHolder;

class ContextMenus
{
public:
    static void showObjectsMenu(ObjectsHolder& objectsHolder,
                                const Point<int>& pos);

    static void showLinkPopupMenu(ObjectsHolder& objectsHolder,
                                  String so, String eo);

    static void showAudioConnectionPopupMenu(ObjectsHolder& objectsHolder);

private:

    enum tMassMenuItem
    {
        MENU_ITEM_MASS =  1,
        MENU_ITEM_GROUND,
        MENU_ITEM_RESONATORS,
        MENU_ITEM_PORT,
        MENU_ITEM_AUDIO_OUT,
        MENU_ITEM_JUNCTION,
        MENU_ITEM_TERMINATION,
        MENU_ITEM_COMMENT
    };

    enum tLinkMenuItem
    {
        MENU_ITEM_LINK = 1,
        MENU_ITEM_TOUCH,
        MENU_ITEM_PLUCK,
        MENU_ITEM_PULSETOUCH,
        MENU_ITEM_DETENT,
        MENU_ITEM_SOFTENINGLINK,
        MENU_ITEM_STIFFENINGLINK,
        MENU_ITEM_WAVEGUIDE,
        MENU_ITEM_AUDIOCONNECTION
    };
};

}

#endif	/* CONTEXTMENUS_H */

