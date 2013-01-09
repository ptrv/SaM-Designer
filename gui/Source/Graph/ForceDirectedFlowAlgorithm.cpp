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

#include "ForceDirectedFlowAlgorithm.h"

using namespace synthamodeler;

ForceDirectedFlowAlgorithm::ForceDirectedFlowAlgorithm()
: minSize(80.0f), elasticity(200.0f), repulsion(4.0f)
{

}

ForceDirectedFlowAlgorithm::~ForceDirectedFlowAlgorithm()
{

}

//bool ForceDirectedFlowAlgorithm::reflow(DirectedGraph* g, int /*width*/, int /*height*/, ObjController& objController)
bool ForceDirectedFlowAlgorithm::reflow(DirectedGraph* g, int width, int height, ObjController& objController)
{
    Array<Node*>& nodes = g->getNodes();

    int reset = 0;

    for (int i = 0; i < nodes.size(); ++i)
    {
        DBG("width: " +String(width)+ " height: " +String(height));
        Node* n = nodes.getUnchecked(i);
        Array<Node*>& incoming = n->getIncomingLinks();
        Array<Node*>& outcoming = n->getOutgoingLinks();
        int nx = n->getNX();
        int ny = n->getNY();

        // compute the total push force acting on this node
        int dx = 0;
        int dy = 0;

        for (int j = 0; j < incoming.size(); ++j)
        {
            Node* ni = incoming.getUnchecked(j);
            dx += (ni->getNX() - n->getNX());
            dy += (ni->getNY() - n->getNY());
        }

        float len = sqrt(dx * dx + dy * dy);
        float angle = GraphUtils::getDirection(dx, dy);
        Point<int> motion = GraphUtils::rotateCoordinate(0.9 * repulsion,
                                                         0.0, angle);

        int px = n->getNX();
        int py = n->getNY();
        nx += motion.x;
        ny += motion.y;

        if (n->getNX() < 0)
        {
            nx = 0;
        }
        else if (n->getNX() > width)
        {
            nx = width;
        }
        if (n->getNY() < 0)
        {
            ny = 0;
        }
        else if (n->getNY() > height)
        {
            ny = height;
        }
        // undo repositioning if elasticity is violated
        float shortest = n->getShortestLinkLength();
        DBG(shortest);
        if (shortest < minSize || shortest > elasticity * 2)
        {
            ++reset;
            nx = px;
            ny = py;
        }
        objController.getSelectedObjects().deselectAll();

        if(ObjectComponent* oc = objController.getObjectForId(n->getLabel()))
        {
            oc->setPosition(Point<int>(nx, ny), false);
        }
    }
    DBG("reset: " << reset << ", node size: " << nodes.size() );
    objController.changed();
    return reset == nodes.size();
}