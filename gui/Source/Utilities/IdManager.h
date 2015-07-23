/*
  ==============================================================================

    IdManager.h
    Created: 31 Aug 2012 1:21:14am
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

#ifndef __IDMANAGER_H_C25241F9__
#define __IDMANAGER_H_C25241F9__

#include "JuceHeader.h"

#include <map>

namespace synthamodeler
{
class IdManager
{
public:
    IdManager();
    virtual ~IdManager();
    bool addId(const Identifier& objId, const String& objName,
               UndoManager* undoManager);
    void removeId(const Identifier& objId, const String& objName,
                  UndoManager* undoManager);
    bool contains(const Identifier& objId, const String& objName);
    bool renameId(const Identifier& objId, const String& oldName,
                  const String& newName, UndoManager* undoManager);
    String getNextId(const Identifier& objId);
    String getObjNameForPaste(const Identifier& objId,
                              const String& objName,
                              int timesPasted,
                              bool groupPaste);

    String getAllIdsRegex() const;
    String getAllIdsFilteredRegex(const StringArray& filter) const;

private:
    SortedSet<String>* getSet(const Identifier& objId);
    String getObjNamePrefix(const Identifier& objId);

    bool addToAllIds(const String& objId);
    void removeFromAllIds(const String& objId);
    SortedSet<String> allIds;
    StringArray allIdsSorted;
    Array<SortedSet<String> > theIds;

    static const std::map<const char*, String> objNamePrefixMap;

};
}


#endif  // __IDMANAGER_H_C25241F9__
