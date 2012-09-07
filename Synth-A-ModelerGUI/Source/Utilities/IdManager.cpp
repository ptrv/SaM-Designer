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

#include "../Application/CommonHeaders.h"

#include "IdManager.h"

IdManager::IdManager()
{
    
}

IdManager::~IdManager()
{
//    objectIds.clear();
    massIds.clear();
    linkIds.clear();
    touchIds.clear();
    pluckIds.clear();
    audioOutIds.clear();
}

SortedSet<String>* IdManager::getSet(const Identifier& objId)
{
    if(objId == Ids::mass)
        return &massIds;
    else if(objId == Ids::ground)
        return &groundIds;
    else if(objId == Ids::port)
        return &portIds;
    else if(objId == Ids::resonator)
        return &resonatorIds;
    else if(objId == Ids::link)
        return &linkIds;
    else if(objId == Ids::touch)
        return &touchIds;
    else if(objId == Ids::pluck)
        return &pluckIds;
    else if(objId == Ids::audioout)
        return &audioOutIds;
    else if(objId == Ids::variable)
        return &variableIds;
    else
        return nullptr;
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
            return theSet->add(objName) && allIds.add(objName);
        else
            return false;
    }
//    return objectIds.add(objName);
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

        allIds.removeValue(objName);
    }

//    objectIds.removeValue(objName);
}

bool IdManager::contains(const Identifier& objId, const String& objName)
{
    SortedSet<String>* theSet = getSet(objId);
    if(theSet != nullptr)
        return theSet->contains(objName) && allIds.contains(objName);
    else
        return false;

//    return objectIds.contains(objName);
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
            allIds.removeValue(oldName);
            return theSet->add(newName) && allIds.add(newName);
        }
        else
        {
            return false;
        }
    }

//    objectIds.removeValue(oldName);
//    return objectIds.add(newName);

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

String IdManager::getObjNamePrefix(const Identifier& objId)
{
if(objId == Ids::mass)
        return "m";
    else if(objId == Ids::ground)
        return "g";
    else if(objId == Ids::port)
        return "dev";
    else if(objId == Ids::resonator)
        return "r";
    else if(objId == Ids::link)
        return "l";
    else if(objId == Ids::touch)
        return "t";
    else if(objId == Ids::pluck)
        return "p";
    else if(objId == Ids::audioout)
        return "a";
    else
        return String::empty;
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