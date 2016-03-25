/*
  ==============================================================================

    IdManager.cpp
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

#include "IdManager.h"

#include "Application/CommonHeaders.h"

#include "Utilities/ObjectsHelper.h"


using namespace synthamodeler;

IdManager::IdManager()
: allIdsRegex(String::empty)
{
    for (int i = 0; i < ObjectsHelper::getAllObjectIds().size(); ++i)
    {
        theIds.add(SortedSet<String>());
    }
}

IdManager::~IdManager()
{
    allIds.clear();
    for (SortedSet<String>& s : theIds)
    {
        s.clear();
    }
}

SortedSet<String>* IdManager::getSet(const Identifier& objId)
{
    int objIdx = ObjectsHelper::getAllObjectIds().indexOf(objId);
    if(objIdx == -1)
        return nullptr;

    return &theIds.getReference(objIdx);
}

class AddIdAction : public UndoableAction
{
public:
    AddIdAction(IdManager* idMgr_, const Identifier& objId_, const String& objName_)
    : idMgr(idMgr_), objId(objId_), objName(objName_) {}
    ~AddIdAction(){}

    bool perform()
    {
        return idMgr->addId(objId, objName, nullptr);
    }

    bool undo()
    {
        idMgr->removeId(objId, objName, nullptr);
        return true;
    }
private:
    IdManager* idMgr;
    Identifier objId;
    String objName;
};

bool IdManager::addId(const Identifier& objId, const String& objName,
                      UndoManager* undoManager)
{
    if( undoManager != nullptr)
    {
        AddIdAction* action = new AddIdAction(this, objId, objName);
        return undoManager->perform(action, "Add Id");
    }
    else
    {
        SortedSet<String>* theSet = getSet(objId);
        if (theSet != nullptr && objName != String::empty)
        {
            if (addToAllIds(objName))
            {
                if (theSet->add(objName))
                {
                    return true;
                }
                else
                {
                    // This line should never be reached, because if adding new
                    // name to allId ssucceeds, adding to theSet should also
                    // succeed
                    allIds.removeString(objName);
                }
            }
        }
        return false;
    }
}

class RemoveIdAction : public UndoableAction
{
public:
    RemoveIdAction(IdManager* idMgr_, const Identifier& objId_,
                   const String& objName_)
    : idMgr(idMgr_), objId(objId_), objName(objName_) {}
    ~RemoveIdAction(){}

    bool perform()
    {
        idMgr->removeId(objId, objName, nullptr);
        return true;
    }

    bool undo()
    {
        return idMgr->addId(objId, objName, nullptr);
    }
private:
    IdManager* idMgr;
    Identifier objId;
    String objName;
};

void IdManager::removeId(const Identifier& objId, const String& objName,
                         UndoManager* undoManager)
{
    if(undoManager != nullptr)
    {
        RemoveIdAction* action = new RemoveIdAction(this, objId, objName);
        undoManager->perform(action, "Remove Id");
    }
    else
    {
        SortedSet<String>* theSet = getSet(objId);
        if (theSet != nullptr)
            theSet->removeValue(objName);

        allIds.removeString(objName);
    }
}

bool IdManager::contains(const Identifier& objId, const String& objName)
{
    SortedSet<String>* theSet = getSet(objId);
    if(theSet != nullptr)
        return theSet->contains(objName) && allIds.contains(objName);
    else
        return false;
}

class RenameIdAction : public UndoableAction
{
public:
    RenameIdAction(IdManager* idMgr_, const Identifier& objId_,
                   const String& oldName_, const String& newName_)
    : idMgr(idMgr_), objId(objId_), oldName(oldName_), newName(newName_) {}
    ~RenameIdAction(){}

    bool perform()
    {
        return idMgr->renameId(objId, oldName, newName, nullptr);
    }

    bool undo()
    {
        return idMgr->renameId(objId, newName, oldName, nullptr);
    }
private:
    IdManager* idMgr;
    Identifier objId;
    String oldName;
    String newName;
};

bool IdManager::renameId(const Identifier& objId, const String& oldName,
                         const String& newName, UndoManager* undoManager)
{
    if(undoManager != nullptr)
    {
        RenameIdAction* action = new RenameIdAction(this, objId, oldName, newName);
        return undoManager->perform(action, "Rename Id");
    }
    else
    {
        SortedSet<String>* theSet = getSet(objId);
        if (theSet != nullptr)
        {
            theSet->removeValue(oldName);
            allIds.removeString(oldName);
            if (addToAllIds(newName))
            {
                if (theSet->add(newName))
                {
                    return true;
                }
                else
                {
                    // This line should never be reached, because if adding new
                    // name to allId ssucceeds, adding to theSet should also
                    // succeed
                    allIds.removeString(newName);
                }
            }
        }
        return false;
    }
}

String IdManager::getNextId(const Identifier& objId)
{
    SortedSet<String>* theSet = getSet(objId);
    if(theSet != nullptr)
    {
        String nextId = getObjNamePrefix(objId);
        int numObjs = theSet->size();

        while (theSet->contains(nextId + String(numObjs)))
        {
            ++numObjs;
        }
        return nextId + String(numObjs);
    }
    else
    {
        return String::empty;
    }
}

const std::map<const char*, String> IdManager::objNamePrefixMap =
{
    {Ids::mass.getCharPointer(), "m"},
    {Ids::ground.getCharPointer(), "g"},
    {Ids::port.getCharPointer(), "dev"},
    {Ids::resonators.getCharPointer(), "r"},
    {Ids::link.getCharPointer(), "l"},
    {Ids::pluck.getCharPointer(), "p"},
    {Ids::touch.getCharPointer(), "t"},
    {Ids::pulseTouch.getCharPointer(), "pt"},
    {Ids::detent.getCharPointer(), "d"},
    {Ids::softeninglink.getCharPointer(), "softl"},
    {Ids::stiffeninglink.getCharPointer(), "stifl"},
    {Ids::audioout.getCharPointer(), "a"},
    {Ids::waveguide.getCharPointer(), "wg"},
    {Ids::termination.getCharPointer(), "term"},
    {Ids::junction.getCharPointer(), "junct"},
    {Ids::comment.getCharPointer(), "comment"},
    {Ids::display.getCharPointer(), "dply"}
};

String IdManager::getObjNamePrefix(const Identifier& objId)
{
    auto it = objNamePrefixMap.find(objId.getCharPointer());
    return it != objNamePrefixMap.end() ? it->second : String("n/a");
}

String IdManager::getObjNameForPaste(const Identifier& objId,
                                     const String& objName,
                                     int timesPasted,
                                     bool groupPaste)
{
    if(groupPaste)
    {
        // TODO better solution
        String copySuffix = "cpy";
        if(timesPasted > 1)
            copySuffix << String(timesPasted - 1);

        String tmpName = objName;
        String tmpName2 = tmpName + copySuffix;
        int tmpSuffix = 0;

        while(contains(objId, tmpName2))
        {
            ++tmpSuffix;
            tmpName2 = tmpName2 + String(tmpSuffix);
        }
        tmpName = tmpName2;
        return tmpName;
    }
    else
    {
        return getNextId(objId);
    }
}

const String& IdManager::getAllIdsRegex()
{
    if (allIdsRegex.isEmpty())
    {
        allIdsRegex << "(" << allIds.joinIntoString("|") << ")";
    }
    return allIdsRegex;
}

String IdManager::getAllIdsFilteredRegex(const StringArray& filters) const
{
    StringArray result;

    for (const String& id : allIds)
    {
        if (!filters.contains(id, false))
        {
            result.add(id);
        }
    }

    return "(" + result.joinIntoString("|") + ")";
}

bool IdManager::addToAllIds(const String& newId)
{
    const Array<String>::ScopedLockType lock (allIds.strings.getLock());

    int s = 0;
    int e = allIds.size();

    while (s < e)
    {
        String& elem = allIds.getReference (s);
        if (newId == elem)
        {
            elem = newId; // force an update in case operator== permits differences.
            return false;
        }

        const int halfway = (s + e) / 2;
        const bool isBeforeHalfway = (newId.length() > allIds.getReference (halfway).length());

        if (halfway == s)
        {
            if (! isBeforeHalfway)
                ++s;

            break;
        }

        if (isBeforeHalfway)
            e = halfway;
        else
            s = halfway;
    }

    allIds.insert(s, newId);
    allIdsRegex = String::empty;
    return true;

}
