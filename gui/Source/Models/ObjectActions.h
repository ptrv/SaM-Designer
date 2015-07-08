/*
  ==============================================================================

    ObjectActions.h
    Created: 5 May 2012 5:32:12pm
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

#ifndef __OBJECTACTIONS_H_7C20FDA1__
#define __OBJECTACTIONS_H_7C20FDA1__

#include "JuceHeader.h"

#include "Controller/ObjController.h"

#include "Graph/Node.h"

#include "ObjectIDs.h"
#include "ObjectFactory.h"

#include "View/BaseObjectComponent.h"
#include "View/ObjectComponent.h"
#include "View/LinkComponent.h"
#include "View/Connector.h"
#include "View/ObjectsHolder.h"
#include "View/SelectableObject.h"
#include "View/CommentComponent.h"

#include "Utilities/ObjectsHelper.h"

namespace synthamodeler
{
class AddObjectAction : public UndoableAction
{
public:
    AddObjectAction(ObjController* objController_,
                    ValueTree objTree_, ObjectsHolder* holder_)
	: objTree(objTree_.createCopy()), 
        holderComp(holder_), 
        objController(objController_)
	{
	}
	~AddObjectAction()
	{
	}

	bool perform()
    {
        if (objController.wasObjectDeleted() || holderComp.wasObjectDeleted())
        {
            return false;
        }
        ObjectComponent* objComp = objController->addObject(holderComp, objTree, -1,false);
        indexAdded = objController->indexOfObject(objComp);
        String logText = "Add ";
        logText << objTree.getType().toString();// << " number " << mdlSubTree.getNumChildren();
		SAM_LOG(logText);
		return true;
	}

	bool undo()
    {
        if (objController.wasObjectDeleted() || holderComp.wasObjectDeleted())
        {
            return false;
        }

        ObjectComponent* objComp = objController->getObject(indexAdded);
        objController->removeObject(objComp, false, holderComp);

        String logText = "Undo add ";
        logText <<  objTree.getType().toString();// << " number " << mdlSubTree.getNumChildren();
		SAM_LOG(logText);
		return true;
	}
    int indexAdded;
private:
    ValueTree objTree;
    WeakReference<ObjectsHolder> holderComp;
    WeakReference<ObjController> objController;
};

class RemoveObjectAction : public UndoableAction
{
public:
    RemoveObjectAction(ObjectsHolder* objHolderComp_,
                       ObjectComponent* componentToRemove,
                       ObjController* objController_)
    :
    oldIndex(-1),
    holderComp(objHolderComp_),
    objController(objController_)
	{
        oldValue = componentToRemove->getData().createCopy();
        oldIndex = objController->indexOfObject(componentToRemove);
	}
	~RemoveObjectAction()
	{
	}

	bool perform()
    {
        if (objController.wasObjectDeleted() || holderComp.wasObjectDeleted())
        {
            return false;
        }

        SAM_LOG("Remove "+oldValue[Ids::identifier].toString());
        ObjectComponent* oc = objController->getObject(oldIndex);
        if(objController->getSelectedObjects().getNumSelected() == 0)
        {
            objController->getSelectedObjects().selectOnly(oc);
        }
        objController->removeObject(oc, false, holderComp);
		return true;
	}

	bool undo()
    {
        if (objController.wasObjectDeleted() || holderComp.wasObjectDeleted())
        {
            return false;
        }

        SAM_LOG("Undo remove "+oldValue[Ids::identifier].toString());

        ObjectComponent* oc = objController->addObject(holderComp, oldValue, oldIndex, false);
        if(objController->getSelectedObjects().getNumSelected() == 0)
        {
            objController->getSelectedObjects().selectOnly(oc);
        }
        if(oc->getData().getType() == Ids::audioout)
        {
            ValueTree sources = oc->getData().getChildWithName(Ids::sources);
            if(sources.isValid())
            {
                for (int i = 0; i < sources.getNumChildren(); ++i)
                {
                    ValueTree src = sources.getChild(i);
                    BaseObjectComponent* boc =
                        ObjectsHelper::getBaseObjectFromSource(*objController.get(), src);

//                    AudioOutConnector* aoc = objController->addAudioConnection(holderComp, boc, oc, src, -1, false);
                    objController->addConnector(holderComp, boc, oc, src, -1, false);
//                    if(aoc != nullptr)
//                        aoc->repaint();
                }
            }
        }

		return true;
	}
private:
	ValueTree oldValue;
    int oldIndex;
    WeakReference<ObjectsHolder> holderComp;
    WeakReference<ObjController> objController;

};

class MoveObjectAction : public UndoableAction {
public:
	MoveObjectAction(ObjectComponent* componentToMove,
                    Point<int> newPos_,
                    ObjController* objController_)
	: objController(objController_),
	  newPos(newPos_),
      oldPos(componentToMove->getOldPos())
	{
        indexOfObjectToMove = objController->indexOfObject(componentToMove);
	}

	~MoveObjectAction()
	{

	}

	bool perform()
    {
        if (!objController.wasObjectDeleted())
        {
            ObjectComponent* oc = objController->getObject(indexOfObjectToMove);
            oc->setPosition(newPos, false);
            return true;
        }
        return false;
	}

	bool undo()
    {
        if (!objController.wasObjectDeleted())
        {
            ObjectComponent* oc = objController->getObject(indexOfObjectToMove);
            oc->setPosition(oldPos, false);
            return true;
        }
        return false;
	}
private:
    WeakReference<ObjController> objController;
	Point<int> newPos;
    Point<int> oldPos;
    int indexOfObjectToMove;
};

class AddLinkAction : public UndoableAction
{
public:
	AddLinkAction(ObjController* objController_,
                ValueTree linkTree_, ObjectsHolder* holder_)
	: linkTree(linkTree_.createCopy()), 
        holderComp(holder_), 
        objController(objController_)
	{
	}
	~AddLinkAction()
	{
	}

	bool perform()
    {
        if (objController.wasObjectDeleted() || holderComp.wasObjectDeleted())
        {
            return false;
        }
        
        LinkComponent* linkComp = objController->addLink(holderComp, linkTree, -1, false);
        indexAdded = objController->indexOfLink(linkComp);

        String logText = "Add ";
        logText << linkTree.getType().toString();// << " number " << mdlSubTree.getNumChildren();
		SAM_LOG(logText);
		return true;
	}

	bool undo()
    {
        if (objController.wasObjectDeleted() || holderComp.wasObjectDeleted())
        {
            return false;
        }

        objController->removeLink(objController->getLink(indexAdded), false, holderComp);
        String logText = "Undo add ";
        logText <<  linkTree.getType().toString();// << " number " << mdlSubTree.getNumChildren();
		SAM_LOG(logText);
		return true;
	}
    int indexAdded;
private:
    ValueTree linkTree;
    WeakReference<ObjectsHolder> holderComp;
    WeakReference<ObjController> objController;

};

class RemoveLinkAction : public UndoableAction
{
public:
    RemoveLinkAction(ObjectsHolder* objHolderComp_,
                     LinkComponent* linkToRemove,
                     ObjController* objController_)
    :
    oldIndex(-1),
    holderComp(objHolderComp_),
    objController(objController_)
	{
        oldValue = linkToRemove->getData().createCopy();
        oldIndex = objController->indexOfLink(linkToRemove);
	}
	~RemoveLinkAction()
	{
	}

	bool perform()
    {
        if (objController.wasObjectDeleted() || holderComp.wasObjectDeleted())
        {
            return false;
        }

        SAM_LOG("Remove "+oldValue[Ids::identifier].toString());
        LinkComponent* lc = objController->getLink(oldIndex);
        if(objController->getSelectedObjects().getNumSelected() == 0)
        {
            objController->getSelectedObjects().selectOnly(lc);
        }
        objController->removeLink(lc, false, holderComp);
		
        return true;
	}

	bool undo()
    {
        if (objController.wasObjectDeleted() || holderComp.wasObjectDeleted())
        {
            return false;
        }

        SAM_LOG("Undo remove "+oldValue[Ids::identifier].toString());

        LinkComponent* lc = objController->addLink(holderComp, oldValue, oldIndex, false);
        if(objController->getSelectedObjects().getNumSelected() == 0)
        {
            objController->getSelectedObjects().selectOnly(lc);
        }

		return true;
	}
private:
	ValueTree oldValue;
    int oldIndex;
    WeakReference<ObjectsHolder> holderComp;
    WeakReference<ObjController> objController;

};

class ReverseLinkDirectionAction : public UndoableAction
{
public:
    ReverseLinkDirectionAction(LinkComponent* linkToReverse,
                               ObjController* objController_)
    : 
    objController(objController_)
	{
        linkIndex = objController->indexOfLink(linkToReverse);
	}
	~ReverseLinkDirectionAction()
	{
	}

	bool perform()
	{
//        SAM_LOG("Reverse direction"+oldValue[Ids::identifier].toString());
        return reverse();
	}

	bool undo()
	{
//        SAM_LOG("Undo remove "+oldValue[Ids::identifier].toString());
        return reverse();
	}
private:
    bool reverse()
    {
        if (!objController.wasObjectDeleted())
        {
            LinkComponent* lc = objController->getLink(linkIndex);
            if (objController->getSelectedObjects().getNumSelected() == 0)
            {
                objController->getSelectedObjects().selectOnly(lc);
            }
            lc->reverseDirection();
            return true;
        }
        return false;
    }

	ValueTree linkTree;
    WeakReference<ObjController> objController;
    String oldStart;
    String oldEnd;
    int linkIndex;
};

class AddAudioConnectionAction : public UndoableAction
{
public:
	AddAudioConnectionAction(ObjController* objController_,
                             BaseObjectComponent* source,
                             ValueTree sourceTree_,
                             ObjectComponent* audioOut,
                             ObjectsHolder* holder_)
	: 
    holderComp(holder_), 
    objController(objController_),
    sourceTree(sourceTree_.createCopy())
	{
        if(ObjectComponent* const oc = dynamic_cast<ObjectComponent*>(source))
        {
            sourceIsLink = false;
            indexSource = objController->indexOfObject(oc);
        }
        else if(LinkComponent* const lc = dynamic_cast<LinkComponent*>(source))
        {
            sourceIsLink = true;
            indexSource = objController->indexOfLink(lc);
        }
        indexAudioOut = objController->indexOfObject(audioOut);
	}
	~AddAudioConnectionAction()
	{
	}

	bool perform()
    {
        if (objController.wasObjectDeleted() || holderComp.wasObjectDeleted())
        {
            return false;
        }

        BaseObjectComponent* sourceComp;
        if(sourceIsLink)
        {
            sourceComp = objController->getLink(indexSource);
        }
        else
        {
            sourceComp = objController->getObject(indexSource);
        }
        Connector* conn = objController->addConnector(
            holderComp, sourceComp, objController->getObject(indexAudioOut),
            sourceTree, -1, false);
        indexAdded = objController->indexOfAudioConnector(conn);

//        String logText = "Add ";
//        logText << linkTree.getType().toString();// << " number " << mdlSubTree.getNumChildren();
//		SAM_LOG(logText);
		return true;
	}

	bool undo()
    {
        if (objController.wasObjectDeleted() || holderComp.wasObjectDeleted())
        {
            return false;
        }

        objController->removeAudioConnection(objController->getAudioConnector(indexAdded), false, holderComp);
//        String logText = "Undo add ";
//        logText <<  linkTree.getType().toString();// << " number " << mdlSubTree.getNumChildren();
//		SAM_LOG(logText);
		return true;
	}
    int indexAdded;
private:
    WeakReference<ObjectsHolder> holderComp;
    WeakReference<ObjController> objController;
    ValueTree sourceTree;
    int indexSource;
    int indexAudioOut;
    bool sourceIsLink;
};

class RemoveAudioConnectionAction : public UndoableAction
{
public:
    RemoveAudioConnectionAction(ObjectsHolder* objHolderComp_,
                                Connector* connToRemove,
                                ObjController* objController_)
    : holderComp(objHolderComp_),
      objController(objController_),
      oldIndex(-1)
    {
        oldIndex = objController->indexOfAudioConnector(connToRemove);
        String srcName;
        if(ObjectComponent* const oc = dynamic_cast<ObjectComponent*>(connToRemove->getSourceObject()))
        {
            sourceIsLink = false;
            oldIndexSource = objController->indexOfObject(oc);
            srcName = oc->getData()[Ids::identifier];
        }
        else if(LinkComponent* const lc = dynamic_cast<LinkComponent*>(connToRemove->getSourceObject()))
        {
            sourceIsLink = true;
            oldIndexSource = objController->indexOfLink(lc);
            srcName = lc->getData()[Ids::identifier];
        }
        oldIndexAudioOut = objController->indexOfObject(connToRemove->getTargetObject());

        ValueTree sources = connToRemove->getTargetObject()->getData().getOrCreateChildWithName(Ids::sources, nullptr);
        for (int i = 0; i < sources.getNumChildren(); ++i)
        {
            ValueTree src = sources.getChild(i);
            if(src[Ids::value].toString().contains(srcName))
                sourceTree = src.createCopy();
        }
	}
	~RemoveAudioConnectionAction()
	{
	}

	bool perform()
    {
        if (objController.wasObjectDeleted() || holderComp.wasObjectDeleted())
        {
            return false;
        }

        Connector* conn = objController->getAudioConnector(oldIndex);
        if(objController->getSelectedObjects().getNumSelected() == 0)
        {
            objController->getSelectedObjects().selectOnly(conn);
        }

        objController->removeAudioConnection(conn, false, holderComp);
		return true;
	}

	bool undo()
    {
        if (objController.wasObjectDeleted() || holderComp.wasObjectDeleted())
        {
            return false;
        }

//        SAM_LOG("Undo remove "+oldValue[Ids::identifier].toString());

        BaseObjectComponent* sourceComp;
        if(sourceIsLink)
            sourceComp = objController->getLink(oldIndexSource);
        else
            sourceComp = objController->getObject(oldIndexSource);
        Connector* conn = objController->addConnector(
            holderComp, sourceComp, objController->getObject(oldIndexAudioOut),
            sourceTree, oldIndex, false);
        if(objController->getSelectedObjects().getNumSelected() == 0)
        {
            objController->getSelectedObjects().selectOnly(conn);
        }
        
		return true;
	}
private:
    WeakReference<ObjectsHolder> holderComp;
    WeakReference<ObjController> objController;
    ValueTree sourceTree;
    int oldIndex;
    int oldIndexSource;
    int oldIndexAudioOut;
    bool sourceIsLink;
};

class AddCommentAction : public UndoableAction
{
public:
	AddCommentAction(ObjController* objController_,
                    ValueTree objTree_, ObjectsHolder* holder_)
	: objTree(objTree_.createCopy()),
      holderComp(holder_),
      objController(objController_)
	{
	}
	~AddCommentAction()
	{
	}

	bool perform()
    {
        if (objController.wasObjectDeleted() || holderComp.wasObjectDeleted())
        {
            return false;
        }

        CommentComponent* commentComp = objController->addComment(holderComp, objTree, -1,false);
        indexAdded = objController->indexOfComment(commentComp);
        String logText = "Add ";
        logText << objTree.getType().toString();// << " number " << mdlSubTree.getNumChildren();
		SAM_LOG(logText);
		return true;
	}

	bool undo()
    {
        if (objController.wasObjectDeleted() || holderComp.wasObjectDeleted())
        {
            return false;
        }

        objController->removeComment(objController->getComment(indexAdded), false, holderComp);

        String logText = "Undo add ";
        logText <<  objTree.getType().toString();// << " number " << mdlSubTree.getNumChildren();
		SAM_LOG(logText);
		return true;
	}
    int indexAdded;
private:
    ValueTree objTree;
    WeakReference<ObjectsHolder> holderComp;
    WeakReference<ObjController> objController;


};

class RemoveCommentAction : public UndoableAction
{
public:
    RemoveCommentAction(ObjectsHolder* objHolderComp_,
                       CommentComponent* componentToRemove,
                       ObjController* objController_)
    :
    oldIndex(-1),
    holderComp(objHolderComp_),
    objController(objController_)
	{
        oldValue = componentToRemove->getData().createCopy();
        oldIndex = objController->indexOfComment(componentToRemove);
	}
	~RemoveCommentAction()
	{
	}

	bool perform()
    {
        if (objController.wasObjectDeleted() || holderComp.wasObjectDeleted())
        {
            return false;
        }

        SAM_LOG("Remove "+oldValue[Ids::identifier].toString());
        CommentComponent* cc = objController->getComment(oldIndex);
        if(objController->getSelectedObjects().getNumSelected() == 0)
        {
            objController->getSelectedObjects().selectOnly(cc);
        }
        objController->removeComment(cc, false, holderComp);
		return true;
	}

	bool undo()
    {
        if (objController.wasObjectDeleted() || holderComp.wasObjectDeleted())
        {
            return false;
        }

        SAM_LOG("Undo remove "+oldValue[Ids::identifier].toString());

        CommentComponent* oc = objController->addComment(holderComp, oldValue, oldIndex, false);
        if(objController->getSelectedObjects().getNumSelected() == 0)
        {
            objController->getSelectedObjects().selectOnly(oc);
        }

		return true;
	}
private:
	ValueTree oldValue;
    int oldIndex;
    WeakReference<ObjectsHolder> holderComp;
    WeakReference<ObjController> objController;

};
class MoveCommentAction : public UndoableAction {
public:
	MoveCommentAction(CommentComponent* componentToMove,
                    Point<int> newPos_,
                    ObjController* objController_)
	: objController(objController_),
	  newPos(newPos_),
      oldPos(componentToMove->getOldPos())
	{
        indexOfCommentToMove = objController->indexOfComment(componentToMove);
	}

	~MoveCommentAction()
	{

	}

	bool perform()
    {
        if (!objController.wasObjectDeleted())
        {
            CommentComponent* cc = objController->getComment(indexOfCommentToMove);
            cc->setPosition(newPos, false);
            return true;
        }
        return false;
	}

	bool undo()
    {
        if (!objController.wasObjectDeleted())
        {
            CommentComponent* cc = objController->getComment(indexOfCommentToMove);
            cc->setPosition(oldPos, false);
            return true;
        }
        return false;
	}
private:
    WeakReference<ObjController> objController;
	Point<int> newPos;
    Point<int> oldPos;
    int indexOfCommentToMove;
};

}

#endif  // __OBJECTACTIONS_H_7C20FDA1__
