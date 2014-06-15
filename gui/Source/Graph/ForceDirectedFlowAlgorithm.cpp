/*
  ==============================================================================

    ForceDirectedFlowAlgorithm.cpp
    Created: 13 Nov 2012 6:48:50pm
    Author:  Peter Vasil
    Source: http://processingjs.nihongoresources.com/graphs/

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

#include "../../JuceLibraryCode/JuceHeader.h"
#include "FlowAlgorithm.h"
#include "DirectedGraph.h"
#include "Node.h"
#include "../Controller/ObjController.h"
#include "../View/BaseObjectComponent.h"
#include "../View/SelectableObject.h"
#include "../View/ObjectComponent.h"
#include "GraphUtils.h"
#include "../Utilities/ObjectsHelper.h"
#include "../Utilities/MiscUtilities.h"
#include "../Utilities/StoredSettings.h"

#include "ForceDirectedFlowAlgorithm.h"

using namespace synthamodeler;

ForceDirectedFlowAlgorithm::ForceDirectedFlowAlgorithm()
{
    initParameters();
}

ForceDirectedFlowAlgorithm::~ForceDirectedFlowAlgorithm()
{

}


void ForceDirectedFlowAlgorithm::initParameters()
{
    mass = StoredSettings::getInstance()->getProps().getDoubleValue("redrawparam_alpha", 1.0);
    charge = StoredSettings::getInstance()->getProps().getDoubleValue("redrawparam_beta", 1000);
    k = StoredSettings::getInstance()->getProps().getDoubleValue("redrawparam_k", 0.02);
    damp = StoredSettings::getInstance()->getProps().getDoubleValue("redrawparam_damp", 0.85);
    stopEnergy = StoredSettings::getInstance()->getProps().getDoubleValue("redrawparam_energy", 0.01);
}


bool ForceDirectedFlowAlgorithm::reflow(DirectedGraph& g,
                                        int /*offsetX*/, int /*offsetY*/,
                                        int width, int height,
                                        ObjController& objController,
                                        float /*deltaTime*/)
{
    // const Array<Node*>& nodes = g.getNodes();
    // const Array<Array<bool>>& edges = g.edges;

    Point<float> totalEnergy(0.0, 0.0);

    for (tNodesAndEdges& group : g.connectedNodes)
    {
        applyForces(totalEnergy, group, width, height, objController);
    }


    float lenTotalEnergy = sqrt(totalEnergy.x * totalEnergy.x + totalEnergy.y * totalEnergy.y);

    if (lenTotalEnergy < stopEnergy)
    {
        g.setPositions();
        return true;
    }

    return false;
}

void ForceDirectedFlowAlgorithm::applyForces(Point<float>& totalEnergy,
                                             tNodesAndEdges& nodesAndEdges,
                                             int width, int height,
                                             ObjController& objController)
{
    const tNodes& group = nodesAndEdges.nodes;
    const tEdgesMatrix& edges = nodesAndEdges.edges;

    const int numNodes = group.size();
    for (int i = 0; i < numNodes; ++i)
    {
        Node* const v = group.getUnchecked(i);

        // const int vi = nodes.indexOf(v);

        const tNodes in = v->getIncomingLinks();
        const tNodes on = v->getOutgoingLinks();

        v->force = Point<float>();

        float vx = v->getNX();
        float vy = v->getNY();

        for (int j = 0; j < numNodes; ++j)
        {
            if (i == j)
                continue;

            Node* const u = group.getUnchecked(j);

            float ux = u->getNX();
            float uy = u->getNY();

            float dsq = ((vx - ux) * (vx - ux) + (vy - uy) * (vy - uy));

            if (dsq == 0.f)
                dsq = 0.001f;

            float coul = (charge / numNodes) / dsq;

            v->force.x += coul * (vx - ux);
            v->force.y += coul * (vy - uy);
        }
        for (int j = 0; j < numNodes; ++j)
        {
            if (!edges[i][j])
                continue;

            Node* const u = group.getUnchecked(j);

            // if (!in.contains(u) && !on.contains(u))
            //     continue;

            float ux = u->getNX();
            float uy = u->getNY();

            v->force.x += k * (ux - vx);
            v->force.y += k * (uy - vy);
        }
    }
    for (Node* const v : group)
    {

        if (ObjectComponent* oc = objController.getObjectForId(v->getLabel()))
        {
            float vx = v->getNX();
            float vy = v->getNY();

            if (oc->isMouseButtonDown())
            {
                vx = oc->getActualPos().x;
                vy = oc->getActualPos().y;
            }
            else
            {
                v->velocity = (v->velocity + v->force) * damp;

                totalEnergy = totalEnergy + (v->velocity * v->velocity);

                vx += v->velocity.x;
                vy += v->velocity.y;
                vx = Utils::constrain<float>(vx, 0, width);
                vy = Utils::constrain<float>(vy, 0, height);
            }
            v->setNX(vx);
            v->setNY(vy);
            oc->setActualPosition(Point<int>(vx, vy));
        }
    }
}
