/*
  ==============================================================================

    DirectedGraph.cpp
    Created: 13 Nov 2012 6:47:06pm
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

#include "DirectedGraph.h"

#include "Controller/ObjController.h"

#include "FlowAlgorithm.h"
#include "GraphUtils.h"

#include "View/BaseObjectComponent.h"
#include "View/SelectableObject.h"
#include "View/ObjectComponent.h"


using namespace synthamodeler;

DirectedGraph::DirectedGraph()
    : flower(nullptr)
{
}

DirectedGraph::~DirectedGraph()
{
    flower = nullptr;
    nodes.clear();
    connectedGroups.clear();
    GraphUtils::clearEdgesMatrix(edges);
}

void DirectedGraph::setFlowAlgorithm(FlowAlgorithm* f)
{
    flower = nullptr;
    flower = f;
}

void DirectedGraph::init(const int numNodes)
{
    GraphUtils::initEdgesMatrix(edges, numNodes);
}

void DirectedGraph::addNode(Node* n)
{
    if (std::find(nodes.begin(), nodes.end(), n) == nodes.end())
    {
        nodes.push_back(n);
    }
}

bool DirectedGraph::linkNodes(Node* n1, Node* n2)
{
    auto it1 = std::find(nodes.begin(), nodes.end(), n1);
    auto it2 = std::find(nodes.begin(), nodes.end(), n2);
    const bool contains = it1 != nodes.end() && it2 != nodes.end();

    if (contains)
    {
        n1->addNeighbour(n2);
        n2->addNeighbour(n1);
        n1->addOutgoingLink(n2);
        n2->addIncomingLink(n1);
        const int nodeIndex1 = it1 - nodes.begin();
        const int nodeIndex2 = it2 - nodes.begin();
        edges[nodeIndex1][nodeIndex2] = true;
        edges[nodeIndex2][nodeIndex1] = true;

        return true;
    }
    return false;
}

tNodes DirectedGraph::getRoots()
{
    tNodes roots;

    for (Node* const n : nodes)
    {
        if(n->getIncomingLinksCount() == 0)
        {
            roots.push_back(n);
        }
    }

    return roots;
}

tNodes DirectedGraph::getLeaves()
{
    tNodes leaves;

    for (Node* const n : nodes)
    {
        if(n->getOutgoingLinksCount() == 0)
        {
            leaves.push_back(n);
        }
    }

    return leaves;
}

bool DirectedGraph::reflow(const Rectangle<int>& area,
                           ObjController& objController,
                           float deltaTime)
{
    return flower ? flower->reflow(*this, area, objController, deltaTime) : false;
}

String DirectedGraph::toString()
{
    String s;
    for (Node* const n : nodes)
    {
        s << n->getLabel() << "\n";
        for (int j = 0; j < n->getOutgoingLinks().size(); ++j)
        {
            Node* o = n->getOutgoingLinks()[j];

            s << "\t" << o->getLabel() << "\n";
        }
        s << "\n";
    }
    return s;
}

void DirectedGraph::setPositions()
{
    for (Node* const n : nodes)
    {
        if (ObjectComponent* oc = dynamic_cast<ObjectComponent*>(n))
        {
            oc->setPosition(oc->getActualPos(), true);
        }
    }
}
