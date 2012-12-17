/*
  ==============================================================================

    ForceBasedFlowAlgorithm.cpp
    Created: 15 Nov 2012 1:04:00am
    Author:  peter

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "FlowAlgorithm.h"
#include "DirectedGraph.h"
#include "Node.h"
#include "../Controller/ObjController.h"
#include "../View/BaseObjectComponent.h"
#include "../View/SelectableObject.h"
#include "../View/ObjectComponent.h"
#include "GraphUtils.h"
#include "../Models/ObjectIDs.h"

#include "ForceBasedFlowAlgorithm.h"

using namespace synthamodeler;

ForceBasedFlowAlgorithm::ForceBasedFlowAlgorithm()
{

}

ForceBasedFlowAlgorithm::~ForceBasedFlowAlgorithm()
{

}

//bool ForceDirectedFlowAlgorithm::reflow(DirectedGraph* g, int /*width*/, int /*height*/, ObjController& objController)
bool ForceBasedFlowAlgorithm::reflow(DirectedGraph* g, int width, int height, ObjController& objController, float deltaT)
{
    Array<Node*>& nodes = g->getNodes();

    Point<float> totalEnergy(0, 0);
//    float dis = 0.0f;
    
    float alpha = 2.0f;
    float beta = 1.01f;
    float kk = 0.10f;
    float eta = 0.75f;
//    float deltaT = 0.1f;

    for (int i = 0; i < nodes.size(); ++i)
    {
        Node* v = nodes.getUnchecked(i);
        v->force.x = v->force.y = 0.0f;

//        alpha = v->getMass();
        float fx = 0.0f;
        float fy = 0.0f;

        Point<float> vp(v->getNX(), v->getNY());
//        int vx = v->getNX();
//        int vy = v->getNY();
    
        Array<Node*>& incoming = v->getIncomingLinks();
        Array<Node*>& outgoing = v->getOutgoingLinks();
        
//        for (int j = 0; j < nodes.size(); ++j)
//        {
//            if(i == j)
//                continue;
//
//            Node* u = nodes.getUnchecked(j);
//
//            Point<float> up(u->getNX(), u->getNY());
//
//            Point<float> res(0.0f, 0.0f);
//            if(outgoing.contains(u) || incoming.contains(u))
//            {
//                res = GraphUtils::hookeForce(vp, up, 200.0f, kk);
//            }
//            else
//            {
//                res = GraphUtils::coulombForce(vp, up, beta);
//            }
//            fx += res.getX();
//            fy += res.getY();
//        }

        for (int j = 0; j < nodes.size(); ++j)
        {
            if(i == j)
                continue;

            Node* u = nodes.getUnchecked(j);

            Point<float> up(u->getNX(), u->getNY());

            Point<float> res(0.0f, 0.0f);
//            if(outgoing.contains(u) || incoming.contains(u))
//            {
//                res = GraphUtils::hookeForce(vp, up, 200.0f, kk);
//            }
//            else
//            {
//            beta = v->getMass() * u->getMass();
            res = GraphUtils::coulombForce(vp, up, beta);
//            }
            fx += res.getX();// /v->getMass();
            fy += res.getY();// /v->getMass();
        }
        for (int j = 0; j < nodes.size(); ++j)
        {
            if(i == j)
                continue;

            Node* u = nodes.getUnchecked(j);

            Point<float> up(u->getNX(), u->getNY());

            Point<float> res(0.0f, 0.0f);
            if(outgoing.contains(u) || incoming.contains(u))
            {
                res = GraphUtils::hookeForce(vp, up, 200.0f, kk);
            }
            fx += res.getX();
            fy += res.getY();
        }


        v->velocity.x = (v->velocity.x + alpha * fx * deltaT) * eta;
        v->velocity.y = (v->velocity.y + alpha * fy * deltaT) * eta;

        totalEnergy.x = totalEnergy.x + alpha * (v->velocity.x * v->velocity.x);
        totalEnergy.y = totalEnergy.y + alpha * (v->velocity.y * v->velocity.y);

//        dis += abs(fx) + abs(fy);
    }

    objController.getSelectedObjects().deselectAll();
    for (int i = 0; i < nodes.size(); ++i)
    {
        Node* v = nodes.getUnchecked(i);
        float vx = v->getNX();
        float vy = v->getNY();

        vx += v->velocity.x * deltaT;
        vy += v->velocity.y * deltaT;

        if(ObjectComponent* oc = objController.getObjectForId(v->getLabel()))
        {
            oc->setPosition(Point<int>(vx, vy), false);
            v->setNX(vx);
            v->setNY(vy);
        }
        else if(LinkComponent* lc = objController.getLinkForId(v->getLabel()))
        {
            DBG("link");
//            ObjectComponent* oc1 = objController.getObject(lc->getData()[Ids::startVertex].toString());
//            ObjectComponent* oc2 = objController.getObject(lc->getData()[Ids::endVertex].toString());
//            oc->setPosition(Point<int>(vx, vy), false);
//            oc->setPosition(Point<int>(vx, vy), false);
        }
    }

    float lengthTotalEnergy = sqrt(totalEnergy.x * totalEnergy.x
                                   + totalEnergy.y * totalEnergy.y);
//    DBG(lengthTotalEnergy);
    if(lengthTotalEnergy < 0.5f)
        return true;

    return false;
}