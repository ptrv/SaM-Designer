/*
  ==============================================================================

    ObjectsClipboard.cpp
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

#include "ObjectsClipboard.h"

#include "Application/CommonHeaders.h"

#include "Controller/ObjController.h"

#include "Graph/Node.h"

#include "View/SelectableObject.h"
#include "View/BaseObjectComponent.h"
#include "View/ObjectComponent.h"
#include "View/LinkComponent.h"
#include "View/Connector.h"
#include "View/CommentComponent.h"

#include "IdManager.h"
#include "ObjectsHelper.h"


using namespace synthamodeler;

const char* const ObjectsClipboard::clipboardXmlTag = "SAMOBJECTS";

static int timesPasted = 0;
void ObjectsClipboard::copySelected(ObjController& objController)
{
    SelectedItemSet <SelectableObject*>& selectedObjects =
        objController.getSelectedObjects();

    if (selectedObjects.getNumSelected() == 0)
    {
        return;
    }

    timesPasted = 0;
    XmlElement clip(clipboardXmlTag);

    for (int i = 0; i < selectedObjects.getNumSelected(); ++i)
    {
        if (ObjectComponent * const oc = dynamic_cast<ObjectComponent*>(selectedObjects.getSelectedItem(i)))
        {
            XmlElement * const e = oc->getData().createXml();
            clip.addChildElement(e);
        }
        else if (LinkComponent * const lc = dynamic_cast<LinkComponent*>(selectedObjects.getSelectedItem(i)))
        {
            XmlElement * const e = lc->getData().createXml();
            clip.addChildElement(e);
        }
        else if (CommentComponent * const cc = dynamic_cast<CommentComponent*>(selectedObjects.getSelectedItem(i)))
        {
            XmlElement * const e = cc->getData().createXml();
            clip.addChildElement(e);
        }
    }

    SystemClipboard::copyTextToClipboard(clip.createDocument(String::empty, false, false));
}

void ObjectsClipboard::paste(ObjController& objController, ObjectsHolder& holder)
{
    XmlDocument clip(SystemClipboard::getTextFromClipboard());
    ScopedPointer<XmlElement> doc(clip.getDocumentElement());

    if (doc != 0 && doc->hasTagName(clipboardXmlTag))
    {
        SelectedItemSet <SelectableObject*>& selectedObjects =
            objController.getSelectedObjects();

        IdManager& idMgr = objController.getIdManager();

        ++timesPasted;

        int numObjs = doc->getNumChildElements();
        bool groupPaste = false;
        if (numObjs > 1)
            groupPaste = true;

        selectedObjects.deselectAll();
        HashMap<String, String> objectNamesOldNew;
        HashMap<String, String> objectNamesOldNewForAudio;

        forEachXmlChildElement(*doc, e)
        {
            ValueTree valTree = ValueTree::fromXml(*e);
            const Identifier& groupId = ObjectsHelper::getObjectGroup(valTree.getType());
            if (groupId == Objects::masses ||
                //groupId == Objects::audioobjects ||
                groupId == Objects::terminations ||
                groupId == Objects::junctions)
            {
                int posX = int(valTree.getProperty(Ids::posX));
                int posY = int(valTree.getProperty(Ids::posY));
                valTree.setProperty(Ids::posX, posX + 10, nullptr);
                valTree.setProperty(Ids::posY, posY + 10, nullptr);

                String objName = valTree.getProperty(Ids::identifier).toString();
                if (idMgr.contains(valTree.getType(), objName))
                {
                    String newName = idMgr.getObjNameForPaste(
                        valTree.getType(), objName, timesPasted, groupPaste);
                    objectNamesOldNew.set(objName, newName);
                    objectNamesOldNewForAudio.set(objName, newName);
                    valTree.setProperty(Ids::identifier, newName, nullptr);
                }
                else
                {
                    objectNamesOldNew.set(objName, objName);
                }
                ObjectComponent* newObjectComp =
                    objController.addObject(&holder, valTree, -1, true);

                if (newObjectComp != 0)
                    selectedObjects.addToSelection(newObjectComp);
            }
            else if (valTree.getType() == Ids::comment)
            {
                String objName = valTree.getProperty(Ids::identifier).toString();
                if (idMgr.contains(valTree.getType(), objName))
                {
                    String newName = idMgr.getObjNameForPaste(
                        valTree.getType(), objName, timesPasted, groupPaste);
                    valTree.setProperty(Ids::identifier, newName, nullptr);
                }
                CommentComponent* newCommentComp =
                    objController.addComment(&holder, valTree, -1, true);

                if (newCommentComp != 0)
                    selectedObjects.addToSelection(newCommentComp);
            }
        }

        forEachXmlChildElement(*doc, e)
        {
            ValueTree valTree = ValueTree::fromXml(*e);
            const Identifier& groupId = ObjectsHelper::getObjectGroup(valTree.getType());
            if (groupId == Objects::links || groupId == Objects::waveguides)
            {
                String objName = valTree.getProperty(Ids::identifier).toString();
                if (idMgr.contains(valTree.getType(), objName))
                {
                    String newName = idMgr.getObjNameForPaste(
                        valTree.getType(), objName, timesPasted, groupPaste);
                    objectNamesOldNewForAudio.set(objName, newName);
                    valTree.setProperty(Ids::identifier, newName, nullptr);
                }
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

                LinkComponent* newLinkComp =
                    objController.addLink(&holder, valTree, -1, true);

                if (newLinkComp != 0)
                    selectedObjects.addToSelection(newLinkComp);
            }
        }

        forEachXmlChildElementWithTagName(*doc, e, "audioout")
        {
            ValueTree valTree = ValueTree::fromXml(*e);

            int posX = int(valTree.getProperty(Ids::posX));
            int posY = int(valTree.getProperty(Ids::posY));
            valTree.setProperty(Ids::posX, posX + 10, nullptr);
            valTree.setProperty(Ids::posY, posY + 10, nullptr);

            String objName = valTree.getProperty(Ids::identifier).toString();
            if (idMgr.contains(valTree.getType(), objName))
            {
                String newName = idMgr.getObjNameForPaste(
                    valTree.getType(), objName, timesPasted, groupPaste);
                objectNamesOldNew.set(objName, newName);
                objectNamesOldNewForAudio.set(objName, newName);
                valTree.setProperty(Ids::identifier, newName, nullptr);
            }
            else
            {
                objectNamesOldNew.set(objName, objName);
            }

            ValueTree sources = valTree.getChildWithName(Ids::sources);
            for (int j = 0; j < sources.getNumChildren(); ++j)
            {
                ValueTree source = sources.getChild(j);
                BaseObjectComponent* sourceComp =
                    ObjectsHelper::getBaseObjectFromSource(objController, source);
                
                String oldSrcName = sourceComp->getData()[Ids::identifier];
                if (objectNamesOldNewForAudio.contains(oldSrcName))
                {
                    String sVal = source.getProperty(Ids::value);
                    StringArray sVals;
                    sVals.addTokens(sVal, "*", "\"");
                    String newVal;
                    int srcIdx = -1;
                    for (int k = 0; k < sVals.size(); ++k)
                    {
                        if (sVals[k].compare(oldSrcName) == 0)
                        {
                            srcIdx = k;
                            break;
                        }
                    }
                    if (srcIdx != -1)
                    {
                        int gainVals = 0;
                        for (int k = 0; k < sVals.size(); ++k)
                        {
                            if (k != srcIdx)
                            {
                                if (gainVals >= 1)
                                    newVal << "*";
                                newVal << sVals[k];
                                ++gainVals;
                            }
                            else
                            {
                                newVal << objectNamesOldNewForAudio[oldSrcName];
                                ++gainVals;
                            }
                        }
                    }
                    source.setProperty(Ids::value, newVal, nullptr);
                }
            }

            //            DBG(valTree.toXmlString());
            ObjectComponent* newObjectComp =
                objController.addObject(&holder, valTree, -1, true);

            if (newObjectComp != 0)
                selectedObjects.addToSelection(newObjectComp);

            for (int j = 0; j < sources.getNumChildren(); ++j)
            {
                ValueTree source = sources.getChild(j);
                BaseObjectComponent* sourceComp =
                    ObjectsHelper::getBaseObjectFromSource(objController, source);
                sourceComp = ObjectsHelper::getBaseObjectFromSource(objController, source);

                if (sourceComp != nullptr)
                {
                    sources.removeChild(source, nullptr);
                    Connector* conn =
                        objController.addAudioConnection(&holder, sourceComp,
                                                         newObjectComp, source,
                                                         -1, true);
                    if (conn != nullptr)
                        selectedObjects.addToSelection(conn);
                }
            }
        }
    }
}

void ObjectsClipboard::cut(ObjController& objController, ObjectsHolder& holder)
{
    ObjectsClipboard::copySelected(objController);
    objController.removeSelectedObjects(holder);
}

bool ObjectsClipboard::canPaste()
{
    XmlDocument clip(SystemClipboard::getTextFromClipboard());
    ScopedPointer<XmlElement> doc(clip.getDocumentElement());

    return doc != 0 && doc->hasTagName(clipboardXmlTag);
}
