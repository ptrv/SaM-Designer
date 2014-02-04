/*
  ==============================================================================

    ObjectsClipboard.h
    Created: 3 Feb 2014 1:05:41am
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

#ifndef OBJECTSCLIPBOARD_H_INCLUDED
#define OBJECTSCLIPBOARD_H_INCLUDED

namespace synthamodeler
{

class ObjController;
class ObjectsHolder;

class ObjectsClipboard
{
public:
    static const char* const clipboardXmlTag;
    static void copySelected(ObjController& objController);
    static void paste(ObjController& objController, ObjectsHolder& holder);
    static void cut(ObjController& objController, ObjectsHolder& holder);
};

}



#endif  // OBJECTSCLIPBOARD_H_INCLUDED
