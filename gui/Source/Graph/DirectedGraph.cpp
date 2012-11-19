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

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Node.h"
#include "FlowAlgorithm.h"
//#include "ForceDirectedFlowAlgorithm.h"
#include "CircleFlowAlgorithm.h"
#include "../Controller/ObjController.h"

#include "DirectedGraph.h"

using namespace synthamodeler;

DirectedGraph::DirectedGraph()
{
    flower = new CircleFlowAlgorithm();
}

DirectedGraph::~DirectedGraph()
{
    flower = nullptr;
}

void DirectedGraph::setFlowAlgorithm(FlowAlgorithm* f)
{
    flower = nullptr;
    flower = f;
}

void DirectedGraph::addNode(Node* n)
{
    nodes.addIfNotAlreadyThere(n);
}

bool DirectedGraph::linkNodes(Node* n1, Node* n2)
{
    if(nodes.contains(n1) && nodes.contains(n2))
    {
        n1->addOutgoingLink(n2);
        n2->addIncomingLink(n1);
        return true;
    }
    return false;
}

Array<Node*> DirectedGraph::getRoots()
{
    Array<Node*> roots;

    for (int i = 0; i < nodes.size(); ++i)
    {
        Node* n = nodes.getUnchecked(i);
        if(n->getIncomingLinksCount() == 0)
            roots.add(n);
    }

    return roots;
}

Array<Node*> DirectedGraph::getLeaves()
{
    Array<Node*> leaves;

    for (int i = 0; i < nodes.size(); ++i)
    {
        Node* n = nodes.getUnchecked(i);
        if(n->getOutgoingLinksCount() == 0)
            leaves.add(n);
    }

    return leaves;
}

bool DirectedGraph::reflow(int width, int height, ObjController& objController, float deltaTime)
{
    return flower->reflow(this, width, height, objController, deltaTime);
}

void DirectedGraph::randomizeNodes(int width, int height)
{
    Random rnd(Time::currentTimeMillis());
    for (int i = 0; i < nodes.size(); ++i)
    {
        Node* n = nodes[i];

        int x = n->getNX();
        int y = n->getNY();
        float f1 = 1.0f;
        float f2 = f1/2.0f;
        int rx = x + rnd.nextInt(width*f1) - width*f2;
        int ry = y + rnd.nextInt(height*f1) - height*f2;
//        while(rx < 0 || ry > width)
//            rx = x + rnd.nextInt(width*f1) - width*f2;
//        while(ry < 0 || ry > height)
//            ry = y + rnd.nextInt(height*f1) - height*f2;

        if(rx < 0)
            rx = abs(rx);
        else if(rx > width)
            rx = width - (rx - width);
        if(ry < 0)
            ry = abs(ry);
        else if(ry > height)
            ry = height - (ry - height);

        n->setNX(rx);
        n->setNY(ry);
    }

}

String DirectedGraph::toString()
{
    String s;
    for (int i = 0; i < nodes.size(); ++i)
    {
        Node* n = nodes[i];
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