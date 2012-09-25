/*
  ==============================================================================

    ObjController.cpp
    Created: 13 Apr 2012 12:06:25am
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
#include "../Models/ObjectActions.h"
#include "../Models/MDLFile.h"
#include "../Models/ObjectFactory.h"
#include "../View/ObjectComponent.h"
#include "../View/LinkComponent.h"
#include "../View/AudioOutConnector.h"
#include "../View/ObjectPropertiesPanel.h"
#include "MDLController.h"
#include "../Utilities/IdManager.h"

#include "ObjController.h"

ObjController::ObjController(MDLController& owner_)
: owner(owner_),
  timesPasted(0)
{
    idMgr = new IdManager();
}

ObjController::~ObjController()
{
    audioConnections.clear(true);
    links.clear(true);
    objects.clear(true);
    idMgr = nullptr;
}

bool ObjController::perform(UndoableAction * const action, const String& actionName)
{
    return owner.perform(action, actionName);
}

ObjectComponent* ObjController::addObject(ObjectsHolder* holder, ValueTree objValues, int index, bool undoable)
{
    if(undoable)
    {
        AddObjectAction* action = new AddObjectAction(this, objValues, holder);
        owner.getUndoManager()->perform(action, "Add new Object");
        
        return objects[action->indexAdded];
    }
    else
    {
        const Identifier& groupName = Objects::getObjectGroup(objValues.getType().toString());
        ValueTree mdl = owner.getMDLTree();
        ValueTree subTree = mdl.getOrCreateChildWithName(groupName, nullptr);

		subTree.addChild(objValues,-1, nullptr);
        idMgr->addId(objValues.getType(), objValues[Ids::identifier].toString(), nullptr);

        ObjectComponent* objComp = new ObjectComponent(*this, objValues);
        objects.insert(index, objComp);

        holder->addAndMakeVisible(objComp);
        holder->updateComponents();
        changed();
        return objComp;
    }
    return 0;
}

void ObjController::addNewObject(ObjectsHolder* holder, ValueTree objValues)
{
    
    ObjectComponent* oc = addObject(holder, objValues, -1, true);
    sObjects.selectOnly(oc);
    
}

LinkComponent* ObjController::addLink(ObjectsHolder* holder, ValueTree linkValues, int index, bool undoable)
{
    if(undoable)
    {
        AddLinkAction* action = new AddLinkAction(this, linkValues, holder);
        owner.getUndoManager()->perform(action, "Add new Link");
        
        return links[action->indexAdded];
    }
    else
    {
        const Identifier& gruopName = Objects::getObjectGroup(linkValues.getType());
        ValueTree mdl = owner.getMDLTree();
        ValueTree subTree = mdl.getOrCreateChildWithName(gruopName, nullptr);
		subTree.addChild(linkValues,-1, nullptr);
        idMgr->addId(linkValues.getType(), linkValues[Ids::identifier].toString(), nullptr);

        LinkComponent* linkComp = new LinkComponent(*this, linkValues);
        links.insert(index, linkComp);
        
		holder->addAndMakeVisible(linkComp);
        holder->updateComponents();
        changed();
        return linkComp;
    }
    return 0;
}

void ObjController::addNewLink(ObjectsHolder* holder, ValueTree linkValues)
{
    addLink(holder, linkValues, -1, true);
}
bool ObjController::checkIfLinkExitsts(ValueTree linkTree)
{
    for (int i = 0; i < links.size(); i++)
    {
        if(links[i]->sameStartEnd(linkTree))
        {
            return true;
        }
    }
    return false;

}

bool ObjController::checkIfAudioConnectionExitsts(ValueTree source,
                                                  ValueTree audioOut)
{
    for (int i = 0; i < audioConnections.size(); i++)
    {
        ValueTree sourceComp = audioConnections[i]->getSourceObject()->getData();
        ValueTree aoComp = audioConnections[i]->getAudioObject()->getData();
        if(sourceComp[Ids::identifier] == source[Ids::identifier]
            && aoComp[Ids::identifier] == audioOut[Ids::identifier])
        {
            return true;
        }
    }
    return false;
}
void ObjController::addNewLinkIfPossible(ObjectsHolder* holder, ValueTree linkValues)
{
    if(sObjects.getNumSelected() == 2)
    {
        ObjectComponent* oc1 = dynamic_cast<ObjectComponent*>(sObjects.getSelectedItem(0));
        ObjectComponent* oc2 = dynamic_cast<ObjectComponent*>(sObjects.getSelectedItem(1));
        
        if(oc1 != nullptr && oc2 != nullptr 
            && oc1->canBeConnected(linkValues.getType())
            && oc2->canBeConnected(linkValues.getType())
            && (! checkIfLinkExitsts(linkValues)))
        {
            addLink(holder, linkValues, -1, true);
            holder->updateComponents();
        }
        else
        {
            String msg = "Cannot connect ";
            msg << oc1->getData().getType().toString();
            msg << " and ";
            msg << oc2->getData().getType().toString();
            msg << " with ";
            msg << linkValues.getType().toString();
            msg << ".";
            SAM_CONSOLE("Error: ", msg);
        }
    }
    else
    {
        SAM_CONSOLE("Error: ", "Please select 2 Objects");
    }
}

AudioOutConnector* ObjController::addAudioConnection(ObjectsHolder* holder,
                                                     BaseObjectComponent* objComp,
                                                     ObjectComponent* audioOutComp,
                                                     int index,
                                                     bool undoable)
{
    if(undoable)
    {
        AddAudioConnectionAction* action = new AddAudioConnectionAction(this, 
                                                                        objComp, 
                                                                        audioOutComp, 
                                                                        holder);
        owner.getUndoManager()->perform(action, "Add new audio connection");
        
        return audioConnections[action->indexAdded];
    }
    else
    {
        AudioOutConnector* aoc = new AudioOutConnector(*this, objComp, audioOutComp);
        ValueTree sources = aoc->getAudioObject()->getData().getChildWithName(Ids::sources);
        ValueTree source(Ids::audiosource);
        String aSrc;
        aSrc << aoc->getSourceObject()->getData()[Ids::identifier].toString();
        aSrc << "*1.0";
        source.setProperty(Ids::value, aSrc, nullptr);
        sources.addChild(source, -1, nullptr);
        audioConnections.insert(index, aoc);
        holder->addAndMakeVisible(aoc);
        holder->updateComponents();
        return aoc;
    }
    return nullptr;
}

void ObjController::addNewAudioConnection(ObjectsHolder* holder)
{
    if(sObjects.getNumSelected() == 2)
    {
        ObjectComponent* oc1 = dynamic_cast<ObjectComponent*>(sObjects.getSelectedItem(0));
        LinkComponent* lc1 = dynamic_cast<LinkComponent*>(sObjects.getSelectedItem(0));
        ObjectComponent* oc2 = dynamic_cast<ObjectComponent*>(sObjects.getSelectedItem(1));
        LinkComponent* lc2 = dynamic_cast<LinkComponent*>(sObjects.getSelectedItem(1));
        
        if(oc1 != nullptr && oc2 != nullptr)
        {
            if ((oc1->getData().getType() == Ids::audioout
                && oc2->getData().getType() != Ids::audioout))
            {
                if(! checkIfAudioConnectionExitsts(oc2->getData(), oc1->getData()))
                    addAudioConnection(holder, oc2, oc1, -1, true);
            }
            else if (oc1->getData().getType() != Ids::audioout
                && oc2->getData().getType() == Ids::audioout)
            {
                if(! checkIfAudioConnectionExitsts(oc1->getData(), oc2->getData()))
                    addAudioConnection(holder, oc1, oc2, -1, true);
            }
            else
            {
                SAM_CONSOLE("Error: ", "Cannot create audio connection");
            }
        }
        else if(oc1 == nullptr && oc2 != nullptr && lc1 != nullptr)
        {
            if( ! checkIfAudioConnectionExitsts(lc1->getData(), oc2->getData()))
                addAudioConnection(holder, lc1, oc2, -1, true);
        }
        else if(oc2 == nullptr && oc1 != nullptr && lc2 != nullptr)
        {
            if( ! checkIfAudioConnectionExitsts(lc2->getData(), oc1->getData()))
                addAudioConnection(holder, lc2, oc1, -1, true);
        }
    }
}

void ObjController::removeObject(ObjectComponent* objComp, bool undoable, ObjectsHolder* holder)
{
    if (undoable)
    {
        owner.getUndoManager()->perform(new RemoveObjectAction(holder, objComp, this));
    }
    else
    {
        sObjects.deselect(objComp);
        sObjects.changed(true);

        // Get link indices attached to this object and remove them.
        // TODO needs better solution
        Array<int> lIndices = checkIfObjectHasLinks(objComp->getData());
//        if(indices.size() > 0)
//            sObjects.deselectAll();
        for(int i = lIndices.size(); --i >= 0;)
        {
            removeLink(getLink(lIndices[i]), true, holder);
        }
        Array<int> aoIndices = checkIfObjectHasAudioConnections(objComp->getData());
        for(int i = aoIndices.size(); --i >= 0;)
        {
            removeAudioConnection(getAudioConnector(aoIndices[i]), true, holder);
        }
        
        const Identifier& groupName = Objects::getObjectGroup(objComp->getData().getType());
        ValueTree mdl = owner.getMDLTree();
        ValueTree subTree = mdl.getOrCreateChildWithName(groupName, nullptr);
        idMgr->removeId(objComp->getData().getType(),
                        objComp->getData()[Ids::identifier].toString(), nullptr);
        subTree.removeChild(objComp->getData(), nullptr);
        objects.removeObject(objComp);
    }
    
}

void ObjController::removeSelectedObjects(ObjectsHolder* holder)
{
    SelectedItemSet <SelectableObject*> temp(sObjects);

    if (temp.getNumSelected() > 0)
    {
        sObjects.deselectAll();
        sObjects.changed(true);
        // first remove all selected links
        for (int i = temp.getNumSelected(); --i >= 0;)
        {
            if(LinkComponent* lc = dynamic_cast<LinkComponent*>(temp.getSelectedItem(i)))
            {
                temp.deselect(lc);
                removeLink(lc, true, holder);
            }
        }
        // then objects and remaining links connected to the objects
        for (int i = temp.getNumSelected(); --i >= 0;)
        {
            if(AudioOutConnector* aoc = dynamic_cast<AudioOutConnector*>(temp.getSelectedItem(i)))
            {
                temp.deselect(aoc);
                removeAudioConnection(aoc, true, holder);
                continue;
            }
        }
        for (int i = temp.getNumSelected(); --i >= 0;)
        {
            if(ObjectComponent* oc = dynamic_cast<ObjectComponent*>(temp.getSelectedItem(i)))
            {
                removeObject(oc, true, holder);
                continue;
            }
//            LinkComponent* lc = dynamic_cast<LinkComponent*>(temp.getSelectedItem(i));
//            if(lc != nullptr)
//            {
//                removeLink(lc, true, holder);
//                continue;
//            }
            
        }
    }
}

void ObjController::removeAudioConnection(AudioOutConnector* aocComp,
                                          bool undoable, 
                                          ObjectsHolder* holder)
{
    if(undoable)
    {
        owner.getUndoManager()->perform(new RemoveAudioConnectionAction(holder, aocComp, this));
    }
    else
    {
        sObjects.deselect(aocComp);
        sObjects.changed(true);
        ValueTree sources = aocComp->getAudioObject()->getData().getChildWithName(Ids::sources);
        ValueTree source = sources.getChildWithProperty(Ids::value, aocComp->getSourceObject()->getData()[Ids::identifier]);
        sources.removeChild(source, nullptr);
        audioConnections.removeObject(aocComp);
    }
}

void ObjController::removeLink(LinkComponent* linkComp, bool undoable, ObjectsHolder* holder)
{
    if (undoable)
    {
        owner.getUndoManager()->perform(new RemoveLinkAction(holder, linkComp, this));
    }
    else
    {
        sObjects.deselect(linkComp);
        sObjects.changed(true);
        
        Array<int> aoIndices = checkIfObjectHasAudioConnections(linkComp->getData());
        for(int i = aoIndices.size(); --i >= 0;)
        {
            removeAudioConnection(getAudioConnector(aoIndices[i]), true, holder);
        }

        const Identifier& groupName = Objects::getObjectGroup(linkComp->getData().getType());
        ValueTree mdl = owner.getMDLTree();
        ValueTree subTree = mdl.getOrCreateChildWithName(groupName, nullptr);
        idMgr->removeId(linkComp->getData().getType(),
                        linkComp->getData()[Ids::identifier].toString(), nullptr);
        subTree.removeChild(linkComp->getData(), nullptr);
        links.removeObject(linkComp);
    }
}

void ObjController::loadComponents(ObjectsHolder* holder)
{
    MDLFile* mf = owner.getMDLFile();
    ValueTree mdl = mf->mdlRoot;
    
    ValueTree massObjects = mdl.getChildWithName(Objects::masses);
    for (int i = 0; i < massObjects.getNumChildren(); i++)
    {
        ValueTree obj = massObjects.getChild(i);
        if(idMgr->addId(obj.getType(), obj[Ids::identifier].toString(), nullptr))
        {
            ObjectComponent* objComp = new ObjectComponent(*this, obj);
            objects.add(objComp);
            holder->addAndMakeVisible(objComp);
            objComp->update();
            SAM_LOG("Load " + obj.getType().toString() + " " + obj[Ids::identifier].toString());
        }
        else
        {
            SAM_LOG("Couldn't add duplicate Object " + obj[Ids::identifier].toString());
        }
    }
    ValueTree termObjects = mdl.getChildWithName(Objects::terminations);
    for (int i = 0; i < termObjects.getNumChildren(); i++)
    {
        ValueTree obj = termObjects.getChild(i);
        if(idMgr->addId(obj.getType(), obj[Ids::identifier].toString(), nullptr))
        {
            ObjectComponent* objComp = new ObjectComponent(*this, obj);
            objects.add(objComp);
            holder->addAndMakeVisible(objComp);
            objComp->update();
            SAM_LOG("Load " + obj.getType().toString() + " " + obj[Ids::identifier].toString());
        }
        else
        {
            SAM_LOG("Couldn't add duplicate Object " + obj[Ids::identifier].toString());
        }
    }
    ValueTree junctObjects = mdl.getChildWithName(Objects::junctions);
    for (int i = 0; i < junctObjects.getNumChildren(); i++)
    {
        ValueTree obj = junctObjects.getChild(i);
        if(idMgr->addId(obj.getType(), obj[Ids::identifier].toString(), nullptr))
        {
            ObjectComponent* objComp = new ObjectComponent(*this, obj);
            objects.add(objComp);
            holder->addAndMakeVisible(objComp);
            objComp->update();
            SAM_LOG("Load " + obj.getType().toString() + " " + obj[Ids::identifier].toString());
        }
        else
        {
            SAM_LOG("Couldn't add duplicate Object " + obj[Ids::identifier].toString());
        }
    }
    ValueTree linkObjects = mdl.getChildWithName(Objects::links);
    for (int i = 0; i < linkObjects.getNumChildren(); i++)
    {
        ValueTree obj = linkObjects.getChild(i);
        if(idMgr->addId(obj.getType(), obj[Ids::identifier].toString(), nullptr))
        {
            LinkComponent* linkComp = new LinkComponent(*this, obj);
            links.add(linkComp);
            holder->addAndMakeVisible(linkComp);
            linkComp->update();
            SAM_LOG("Load " + obj.getType().toString() + " " + obj[Ids::identifier].toString());
        }
    }
    ValueTree waveguideObjects = mdl.getChildWithName(Objects::waveguides);
    for (int i = 0; i < waveguideObjects.getNumChildren(); i++)
    {
        ValueTree obj = waveguideObjects.getChild(i);
        if(idMgr->addId(obj.getType(), obj[Ids::identifier].toString(), nullptr))
        {
            LinkComponent* linkComp = new LinkComponent(*this, obj);
            links.add(linkComp);
            holder->addAndMakeVisible(linkComp);
            linkComp->update();
            SAM_LOG("Load " + obj.getType().toString() + " " + obj[Ids::identifier].toString());
        }
    }

    ValueTree audioObjects = mdl.getChildWithName(Objects::audioobjects);
    for (int i = 0; i < audioObjects.getNumChildren(); i++)
    {
        ValueTree obj = audioObjects.getChild(i);
        if(idMgr->addId(obj.getType(), obj[Ids::identifier].toString(), nullptr))
        {
            ObjectComponent* audioOutComp = new ObjectComponent(*this, obj);
            objects.add(audioOutComp);
            holder->addAndMakeVisible(audioOutComp);
            audioOutComp->update();
            SAM_LOG("Load " + obj.getType().toString() + " " + obj[Ids::identifier].toString());
            
            ValueTree aoSources = obj.getChildWithName(Ids::sources);
            for (int j = 0; j < aoSources.getNumChildren(); ++j)
            {
                ValueTree source = aoSources.getChild(j);
//                ObjectComponent* oc = getObjectForId(src);
//                LinkComponent* lc = getLinkForId(src);
                BaseObjectComponent* sourceComp = Utils::getSourceIdFromSource(this, source);
//                if(oc != nullptr)
//                    sourceComp = oc;
//                else if(lc != nullptr)
//                    sourceComp = lc;
                
                if( sourceComp != nullptr )
                {
                    AudioOutConnector* aoc = new AudioOutConnector(*this, 
                                                                   sourceComp, 
                                                                   audioOutComp);
                    audioConnections.add(aoc);
                    holder->addAndMakeVisible(aoc);
                    aoc->update();
                }
            }
        }
        else
        {
            SAM_LOG("Couldn't add duplicate Object " + obj[Ids::identifier].toString());
        }
    }

    ValueTree variables = mdl.getChildWithName(Objects::variables);
    for (int i = 0; i < variables.getNumChildren(); i++)
    {
        ValueTree obj = variables.getChild(i);
        if(idMgr->addId(obj.getType(), obj[Ids::identifier].toString(), nullptr))
        {
            SAM_LOG("Load " + obj.getType().toString() + " " + obj[Ids::identifier].toString());
        }
        else
        {
            variables.removeChild(obj, nullptr);
        }
    }
    holder->updateComponents();
}

void ObjController::selectAll(bool shouldBeSelected)
{
    if(shouldBeSelected)
    {
        sObjects.deselectAll();
        for (int i = 0; i < objects.size(); ++i)
        {
            sObjects.addToSelection(objects[i]);
        }
        for (int j = 0; j < links.size(); ++j)
        {
            sObjects.addToSelection(links[j]);
        }
        for (int k = 0; k < audioConnections.size(); ++k)
        {
            sObjects.addToSelection(audioConnections[k]);
        }
    }
    else
    {
        sObjects.deselectAll();
    }
}

void ObjController::editObjectProperties(BaseObjectComponent* oc, UndoManager* undoManager)
{
    ObjectPropertiesPanel::show(this, oc, undoManager);
}

void ObjController::startDragging()
{
    for (int i = 0; i < objects.size(); ++i)
    {
        ObjectComponent * const c = objects[i];

        Point<int> r(c->getPosition());

        c->getProperties().set("xDragStart", r.getX());
        c->getProperties().set("yDragStart", r.getY());
    }

    owner.getUndoManager()->beginNewTransaction();
}

void ObjController::dragSelectedComps(int dx, int dy)
{
    owner.getUndoManager()->undoCurrentTransactionOnly();

    for (int i = 0; i < sObjects.getNumSelected(); ++i)
    {
        if(ObjectComponent * const c = dynamic_cast<ObjectComponent*>(sObjects.getSelectedItem(i)))
        {
            const int startX = c->getProperties() ["xDragStart"];
            const int startY = c->getProperties() ["yDragStart"];

            Point<int> r(c->getPosition());

            r.setXY(startX + dx, startY + dy);

            c->setPosition(Point<int>(r.x + c->getWidth() / 2, r.y + c->getHeight() / 2), true);
        }
    }

    changed();
    owner.getMDLFile()->changed();
}

void ObjController::endDragging()
{
    owner.getUndoManager()->beginNewTransaction();
}

void ObjController::moveSelectedComps (int dxFromMoveStart, int dyFromMoveStart)
{
    startDragging();
    dragSelectedComps(dxFromMoveStart, dyFromMoveStart);
    endDragging();
}

UndoManager* ObjController::getUndoManager()
{
    return owner.getUndoManager();
}

void ObjController::changed()
{
    owner.changed();
}

ObjectComponent* ObjController::getObjectForId(String idString) const throw()
{
    for (int i = 0; i < objects.size(); i++)
    {
        ObjectComponent* elem = objects[i];
        if(idString.compare(elem->getData().getProperty(Ids::identifier).toString()) == 0)
        {
            return elem;
        }
    }
    return nullptr;

}

LinkComponent* ObjController::getLinkForId(String idString) const throw()
{
    for (int i = 0; i < links.size(); i++)
    {
        LinkComponent* elem = links[i];
        if(idString.compare(elem->getData().getProperty(Ids::identifier).toString()) == 0)
        {
            return elem;
        }
    }
    return nullptr;

}

void ObjController::reverseLinkDirection()
{
    owner.getUndoManager()->beginNewTransaction();

    for (int i = 0; i < sObjects.getNumSelected(); ++i)
    {
        if(LinkComponent* lc = dynamic_cast<LinkComponent*>(sObjects.getSelectedItem(i)))
        {
            ReverseLinkDirectionAction* action = new ReverseLinkDirectionAction(lc,this);
            owner.getUndoManager()->perform(action, "reverse link direction");
        }
    }
    changed();
    
    owner.getUndoManager()->beginNewTransaction();
}

Array<int> ObjController::checkIfObjectHasLinks(ValueTree objTree)
{
    Array<int> linkIndices;
//    ValueTree objTree = objComp->getData();
    for (int i = 0; i < links.size(); i++)
    {
        ValueTree linkTree = links[i]->getData();
        
        if(linkTree.getProperty(Ids::startVertex) == objTree.getProperty(Ids::identifier)
            || linkTree.getProperty(Ids::endVertex) == objTree.getProperty(Ids::identifier))
        {
            linkIndices.add(i);
        }
    }
    return linkIndices;
}

Array<int> ObjController::checkIfObjectHasAudioConnections(ValueTree objTree)
{
    Array<int> aoIndices;
    for (int i = 0; i < audioConnections.size(); ++i)
    {
        AudioOutConnector* aoc = audioConnections.getUnchecked(i);

        ValueTree aoDataSource = aoc->getSourceObject()->getData();
        ValueTree aoDataAudioObject = aoc->getAudioObject()->getData();
        if(aoDataSource[Ids::identifier] == objTree[Ids::identifier]
            || aoDataAudioObject[Ids::identifier] == objTree[Ids::identifier])
            aoIndices.add(i);
    }
    return aoIndices;
}

//==============================================================================
const char* const ObjController::clipboardXmlTag = "SAMOBJECTS";

void ObjController::copySelectedToClipboard()
{
    if (sObjects.getNumSelected() == 0)
        return;

    timesPasted = 0;
    XmlElement clip (clipboardXmlTag);

    for (int i = 0; i < sObjects.getNumSelected(); ++i)
    {
        if(ObjectComponent * const oc = dynamic_cast<ObjectComponent*>(sObjects.getSelectedItem(i)))
        {
            XmlElement* const e = oc->getData().createXml();
            clip.addChildElement (e);
        }
        else if(LinkComponent * const lc = dynamic_cast<LinkComponent*>(sObjects.getSelectedItem(i)))
        {
            XmlElement* const e = lc->getData().createXml();
            clip.addChildElement (e);
        }
    }

    SystemClipboard::copyTextToClipboard (clip.createDocument (String::empty, false, false));
}

void ObjController::paste(ObjectsHolder* holder)
{
    XmlDocument clip (SystemClipboard::getTextFromClipboard());
    ScopedPointer<XmlElement> doc (clip.getDocumentElement());

    if (doc != 0 && doc->hasTagName (clipboardXmlTag))
    {
        ++timesPasted;

        int numObjs = doc->getNumChildElements();
        bool groupPaste = false;
        if(numObjs > 1)
            groupPaste = true;

        sObjects.deselectAll();
        HashMap<String, String> objectNamesOldNew;
        HashMap<String, String> objectNamesOldNewForAudio;
        forEachXmlChildElement (*doc, e)
        {
            ValueTree valTree = ValueTree::fromXml(*e);
            if(Objects::getObjectGroup(valTree.getType()) == Objects::masses ||
               Objects::getObjectGroup(valTree.getType()) == Objects::audioobjects ||
               Objects::getObjectGroup(valTree.getType()) == Objects::terminations ||
               Objects::getObjectGroup(valTree.getType()) == Objects::junctions)
            {
                int posX = int(valTree.getProperty(Ids::posX));
                int posY = int(valTree.getProperty(Ids::posY));
                valTree.setProperty(Ids::posX, posX + 10, nullptr);
                valTree.setProperty(Ids::posY, posY + 10, nullptr);
                
                String objName = valTree.getProperty(Ids::identifier).toString();
                if(idMgr->contains(valTree.getType(), objName))
                {
                    String newName = idMgr->getObjNameForPaste(valTree.getType(),
                                                               objName,
                                                               timesPasted,
                                                               groupPaste);
                    objectNamesOldNew.set(objName, newName);
                    objectNamesOldNewForAudio.set(objName, newName);
                    valTree.setProperty(Ids::identifier, newName, nullptr);
                    ValueTree objLabels = valTree.getChildWithName(Ids::labels);
//                    for (int i = 0; i < objLabels.getNumChildren(); ++i)
//                    {
//                        ValueTree label = objLabels.getChild(i);
//                        String la = label.getProperty(Ids::value).toString();
//                        la << copySuffix;
//                        label.setProperty(Ids::value, la, nullptr);
//                    }
                }
                else
                {
                    objectNamesOldNew.set(objName, objName);
                }
                ObjectComponent* newObjectComp = addObject(holder, valTree, -1, true);

                if (newObjectComp != 0)
                    sObjects.addToSelection(newObjectComp);
            }
        }
        forEachXmlChildElement(*doc, e)
        {
            ValueTree valTree = ValueTree::fromXml(*e);
            if(Objects::getObjectGroup(valTree.getType()) == Objects::links ||
               Objects::getObjectGroup(valTree.getType()) == Objects::waveguides)
            {
                String objName = valTree.getProperty(Ids::identifier).toString();
                if(idMgr->contains(valTree.getType(), objName))
                {
                    String newName = idMgr->getObjNameForPaste(valTree.getType(),
                                                               objName,
                                                               timesPasted,
                                                               groupPaste);
                    objectNamesOldNewForAudio.set(objName, newName);
                    valTree.setProperty(Ids::identifier, newName, nullptr);
                }
                ValueTree objLabels = valTree.getChildWithName(Ids::labels);
//                    for (int i = 0; i < objLabels.getNumChildren(); ++i)
//                    {
//                        ValueTree label = objLabels.getChild(i);
//                        String la = label.getProperty(Ids::value).toString();
//                        la << copySuffix;
//                        label.setProperty(Ids::value, la, nullptr);
//                    }
                String oldStartVertex = valTree[Ids::startVertex].toString();
                String oldEndVertex = valTree[Ids::endVertex].toString();

                if (objectNamesOldNew.contains(oldStartVertex))
                {
                    valTree.setProperty(Ids::startVertex, objectNamesOldNew[oldStartVertex], nullptr);
                }
                if (objectNamesOldNew.contains(oldEndVertex))
                {
                    valTree.setProperty(Ids::endVertex, objectNamesOldNew[oldEndVertex], nullptr);
                }

                LinkComponent* newLinkComp = addLink(holder, valTree, -1, true);

                if (newLinkComp != 0)
                    sObjects.addToSelection(newLinkComp);
            }
        }
        for(int i = 0; i < sObjects.getNumSelected(); ++i)
        {
            ObjectComponent* ao = dynamic_cast<ObjectComponent*>(sObjects.getSelectedItem(i));
            if(ao != nullptr && ao->getData().getType() == Ids::audioout)
            {
                ValueTree valTree = ao->getData();
                ValueTree sources = valTree.getChildWithName(Ids::sources);
                for (int j = 0; j < sources.getNumChildren(); ++j) {
                    ValueTree source = sources.getChild(j);
                    BaseObjectComponent* sourceComp = Utils::getSourceIdFromSource(this, source);
                    String oldSrcName = sourceComp->getData()[Ids::identifier];
                    if(objectNamesOldNewForAudio.contains(oldSrcName))
                    {
                        source.setProperty(Ids::value, objectNamesOldNewForAudio[oldSrcName], nullptr);
                    }
                    sourceComp = Utils::getSourceIdFromSource(this, source);
//                    ObjectComponent* oc = getObjectForId(source[Ids::value]);
//                    LinkComponent* lc = getLinkForId(source[Ids::value]);
//                    BaseObjectComponent* sourceComp = nullptr;
//                    if(oc != nullptr)
//                        sourceComp = oc;
//                    else if(lc != nullptr)
//                        sourceComp = lc;
                    
                    if( sourceComp != nullptr )
                    {
                        AudioOutConnector* aoc = new AudioOutConnector(*this,
                                                                       sourceComp,
                                                                       ao);
                        audioConnections.add(aoc);
                        holder->addAndMakeVisible(aoc);
                        aoc->update();
                    }
                }
            }
        }
    }
}

void ObjController::cut(ObjectsHolder* holder)
{
    copySelectedToClipboard();
    removeSelectedObjects(holder);
}

bool ObjController::checkIfIdExists(const Identifier& objId, const String& idStr)
{
    return idMgr->contains(objId, idStr);
}

bool ObjController::renameId(const Identifier& objId, const String& oldId,
                             const String& newId, UndoManager* undoManager_)
{
    return idMgr->renameId(objId, oldId, newId, undoManager_);
}

bool ObjController::changeObjectNameInLink(const String& oldName, 
                                           const String& newName,
                                           UndoManager* undoManager)
{
    ObjectComponent* oc = getObjectForId(newName);
    if(oc == nullptr)
        oc = getObjectForId(oldName);
    
    if (oc != nullptr)
    {
        Array<LinkComponent*> links = oc->getAttachedLinks();
        for (int i = 0; i < links.size(); ++i)
        {
            LinkComponent* elem = links[i];
            if (elem->getData()[Ids::startVertex] == oldName)
            {
                elem->getData().setProperty(Ids::startVertex, newName, undoManager);
            }
            else if (elem->getData()[Ids::endVertex] == oldName)
            {
                elem->getData().setProperty(Ids::endVertex, newName, undoManager);
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

void ObjController::changeObjectNameInAudioSources(const String& oldName,
                                                   const String& newName,
                                                   UndoManager* undoManager)
{
    for (int i = 0; i < audioConnections.size(); ++i)
    {
        AudioOutConnector* aoc = audioConnections.getUnchecked(i);

        ValueTree aoData = aoc->getAudioObject()->getData();
        ValueTree sources = aoData.getChildWithName(Ids::sources);

        ValueTree source = sources.getChildWithProperty(Ids::value, oldName);
        if(source.isValid())
            source.setProperty(Ids::value, newName, undoManager);
    }
}

String ObjController::getNewNameForObject(const Identifier& objId)
{
    return idMgr->getNextId(objId);
}

void ObjController::tidyUp()
{
    startDragging();
    int YTOLERANCE = 30;
    int XTOLERANCE = 100;
    int NHIST = 350;
    bool all = (sObjects.getNumSelected() == 0);

    // tidy horizontally
    for (int i = 0; i < objects.size(); ++i)
    {
        ObjectComponent* obj1 = objects[i];
        if(obj1->isSelected() || all)
        {
            for (int j = 0; j < objects.size(); ++j)
            {
                ObjectComponent* obj2 = objects[j];
                if(obj2 != obj1 && (obj2->isSelected() || all))
                {
                    if (obj2->getPinPos().y <= obj1->getPinPos().y + YTOLERANCE &&
                        obj2->getPinPos().y >= obj1->getPinPos().y - YTOLERANCE &&
                        obj2->getPinPos().y !=  obj1->getPinPos().y)
                    {
                        int dx = 0;
                        int dy = obj1->getPinPos().y - obj2->getPinPos().y;
                        const int startX = obj2->getProperties() ["xDragStart"];
                        const int startY = obj2->getProperties() ["yDragStart"];
                        Point<int> r(obj2->getPosition());
                        r.setXY(startX + dx, startY + dy);
                        obj2->setPosition(Point<int>(r.x + obj2->getWidth() / 2, r.y + obj2->getHeight() / 2), true);
                    }
                }
            }
        }
    }

    // tidy vertically
    int histogram[NHIST], besthist = 0, bestdist = 50;
    for (int i = 0; i < NHIST; ++i)
        histogram[i] = 0;

    for (int i = 0; i < objects.size(); ++i)
    {
        ObjectComponent* obj1 = objects[i];
        Rectangle<int> obj1rect = obj1->getBounds();
        if(obj1->isSelected() || all)
        {
            for (int j = 0; j < objects.size(); ++j)
            {
                ObjectComponent* obj2 = objects[j];
                if(obj2 != obj1 && (obj2->isSelected() || all))
                {

//                    Rectangle<int> obj2rect = obj2->getBounds();
//                    if (obj2rect.getX() <= obj1rect.getRight() + XTOLERANCE &&
//                        obj2rect.getX() >= obj1->getPinPos().x - XTOLERANCE)
//
//                    int px2 = obj2->getPinPos().x;
//                    int px1 = obj1->getPinPos().x;
                    if (obj2->getPinPos().x <= obj1->getPinPos().x + XTOLERANCE &&
                        obj2->getPinPos().x >= obj1->getPinPos().x - XTOLERANCE)
                    {
                        int distance = obj2->getPinPos().y - obj1->getPinPos().y;
                        if (distance >= 0 && distance < NHIST)
                            histogram[distance]++;
                    }
                }
            }
        }
    }
//    for (int i = 2; i < (NHIST - 2); ++i)
//    {
//        int hit = 0.5 * histogram[i-2] + histogram[i-1] + 2 * histogram[i] + histogram[i+1] + 0.5 * histogram[i+2];
//        if (hit > besthist)
//        {
//            besthist = hit;
//            bestdist = i;
//        }
//    }
    for (int i = 1; i < (NHIST - 1); ++i)
    {
        int hit = histogram[i-1] + 2 * histogram[i] + histogram[i+1];
        if (hit > besthist)
        {
            besthist = hit;
            bestdist = i;
        }
    }
    DBG("best vertical distance " << bestdist);
    
    for (int i = 0; i < objects.size(); ++i)
    {
        bool keep = true;
        ObjectComponent* obj1 = objects[i];
        int ax = obj1->getPinPos().x;
        int ay = obj1->getPinPos().y;
        if(obj1->isSelected() || all)
        {
            for (int j = 0; j < objects.size(); ++j)
            {
                ObjectComponent* obj2 = objects[j];
                int bx = obj2->getPinPos().x;
                int by = obj2->getPinPos().y;
                if(obj2 != obj1 && (obj2->isSelected() || all))
                {
                    while (keep)
                    {
                        keep = false;

                        if (bx <= ax + XTOLERANCE && bx >= ax - XTOLERANCE &&
                            by > ay && by < ay + NHIST)
                        {
                            int vmove = ay + bestdist - by;
                            int dx = ax - bx;
                            int dy = vmove;
                            const int startX = obj2->getProperties() ["xDragStart"];
                            const int startY = obj2->getProperties() ["yDragStart"];
                            Point<int> r(obj2->getPosition());
                            r.setXY(startX + dx, startY + dy);
                            obj2->setPosition(Point<int>(r.x + obj2->getWidth() / 2, r.y + obj2->getHeight() / 2), true);

                            ay = by + vmove;

                            keep = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    endDragging();
}

void ObjController::setLinksSegmented(bool isSegmented)
{
    for (int i = 0; i < links.size(); ++i)
    {
        LinkComponent* const lc = links.getUnchecked(i);
        lc->setSegmented(isSegmented);
    }
    for (int i = 0; i < audioConnections.size(); ++i)
    {
        AudioOutConnector* const aoc = audioConnections.getUnchecked(i);
        aoc->setSegmented(isSegmented);
    }
}