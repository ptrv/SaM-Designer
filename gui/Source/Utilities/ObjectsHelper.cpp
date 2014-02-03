/*
  ==============================================================================

    ObjectsHelper.cpp
    Created: 3 Feb 2014 12:37:51am
    Author:  peter

  ==============================================================================
*/

#include "../Application/CommonHeaders.h"
#include "../View/SelectableObject.h"
#include "../Graph/Node.h"
#include "../Graph/DirectedGraph.h"
#include "../View/BaseObjectComponent.h"
#include "../View/ObjectComponent.h"
#include "../View/LinkComponent.h"
#include "../View/AudioOutConnector.h"
#include "../Controller/ObjController.h"

#include "ObjectsHelper.h"

using namespace synthamodeler;

//------------------------------------------------------------------------------

bool ObjectsHelper::changeObjectNameInLink(const ObjController& objController,
                                           const String& oldName,
                                           const String& newName,
                                           UndoManager* undoManager)
{
    ObjectComponent* oc = objController.getObjectForId(newName);
    if (oc == nullptr)
        oc = objController.getObjectForId(oldName);

    if (oc != nullptr)
    {
        Array<LinkComponent*> linksTmp = oc->getAttachedLinks();
        for (int i = 0; i < linksTmp.size(); ++i)
        {
            LinkComponent* elem = linksTmp[i];
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

//------------------------------------------------------------------------------

void ObjectsHelper::changeObjectNameInAudioSources(const ObjController& objController,
                                                   const String& oldName,
                                                   const String& newName,
                                                   UndoManager* undoManager)
{
    const int numAudioConnections = objController.getNumAudioConnections();
    for (int i = 0; i < numAudioConnections; ++i)
    {
        AudioOutConnector* aoc = objController.getAudioConnector(i);

        ValueTree aoData = aoc->getAudioObject()->getData();
        ValueTree sources = aoData.getChildWithName(Ids::sources);

        for (int j = 0; j < sources.getNumChildren(); ++j)
        {
            ValueTree source = sources.getChild(j);

            String srcStr = source.getProperty(Ids::value);
            if (srcStr.contains(oldName))
            {
                StringArray srcParts;
                srcParts.addTokens(srcStr, "*", "\"");
                int oldNameIdx = srcParts.indexOf(oldName);
                if (oldNameIdx == 0)
                {
                    srcParts.set(oldNameIdx, newName);
                    srcStr = srcParts.joinIntoString("*");
                    source.setProperty(Ids::value, srcStr, undoManager);
                }
                break;
            }
        }

    }
}

//------------------------------------------------------------------------------

void ObjectsHelper::tidyUpObjects(ObjController& objController)
{
    objController.startDragging();
    const int YTOLERANCE = 30;
    const int XTOLERANCE = 100;
    const int NHIST = 350;
    bool all = (objController.getSelectedObjects().getNumSelected() == 0);
    int numObjects = objController.getNumObjects();

    // tidy horizontally

    for (int i = 0; i < numObjects; ++i)
    {
        ObjectComponent* obj1 = objController.getObject(i);
        if (obj1->isSelected() || all)
        {
            for (int j = 0; j < numObjects; ++j)
            {
                ObjectComponent* obj2 = objController.getObject(j);
                if (obj2 != obj1 && (obj2->isSelected() || all))
                {
                    if (obj2->getPinPos().y <= obj1->getPinPos().y + YTOLERANCE &&
                        obj2->getPinPos().y >= obj1->getPinPos().y - YTOLERANCE &&
                        obj2->getPinPos().y != obj1->getPinPos().y)
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

    for (int i = 0; i < numObjects; ++i)
    {
        ObjectComponent* obj1 = objController.getObject(i);
        Rectangle<int> obj1rect = obj1->getBounds();
        if (obj1->isSelected() || all)
        {
            for (int j = 0; j < numObjects; ++j)
            {
                ObjectComponent* obj2 = objController.getObject(j);
                if (obj2 != obj1 && (obj2->isSelected() || all))
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
        int hit = histogram[i - 1] + 2 * histogram[i] + histogram[i + 1];
        if (hit > besthist)
        {
            besthist = hit;
            bestdist = i;
        }
    }
    DBG("best vertical distance " << bestdist);

    for (int i = 0; i < numObjects; ++i)
    {
        bool keep = true;
        ObjectComponent* obj1 = objController.getObject(i);
        int ax = obj1->getPinPos().x;
        int ay = obj1->getPinPos().y;
        if (obj1->isSelected() || all)
        {
            for (int j = 0; j < numObjects; ++j)
            {
                ObjectComponent* obj2 = objController.getObject(j);
                int bx = obj2->getPinPos().x;
                int by = obj2->getPinPos().y;
                if (obj2 != obj1 && (obj2->isSelected() || all))
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

    objController.endDragging();
}

//------------------------------------------------------------------------------

void ObjectsHelper::makeGraph(const ObjController& objController, DirectedGraph& graph)
{
    //    ScopedPointer<DirectedGraph> g;

    for (int i = 0; i < objController.getNumLinks(); ++i)
    {
        LinkComponent* l = objController.getLink(i);

        graph.addNode(objController.getObjectForId(l->getData()[Ids::startVertex].toString()));
        graph.addNode(objController.getObjectForId(l->getData()[Ids::endVertex].toString()));

        graph.linkNodes(objController.getObjectForId(l->getData()[Ids::startVertex].toString()),
                     objController.getObjectForId(l->getData()[Ids::endVertex].toString()));

    }
    //    for (int k = 0; k < audioConnections.size(); ++k)
    //    {
    //        AudioOutConnector* ac = audioConnections.getUnchecked(k);
    //        g->addNode(ac->getSourceObject());
    //        g->addNode(ac->getAudioObject());
    //
    //        g->linkNodes(ac->getSourceObject(), ac->getAudioObject());
    //    }

    graph.shuffleNodes();
}

//------------------------------------------------------------------------------
