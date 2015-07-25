/*
  ==============================================================================

    DirectedGraph.h
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

#ifndef __DIRECTEDGRAPH_H_72CA91E6__
#define __DIRECTEDGRAPH_H_72CA91E6__

#include "Node.h"

namespace synthamodeler
{
class FlowAlgorithm;
class Node;
class ObjController;

typedef std::vector<std::vector<bool>> tEdgesMatrix;
typedef std::pair<tNodes, tEdgesMatrix> tNodesAndEdges;
typedef std::vector<tNodesAndEdges> tNodeGroups;

class DirectedGraph
{
public:


    DirectedGraph();
    virtual ~DirectedGraph();

    void setFlowAlgorithm(FlowAlgorithm* f);

    void init(int numNodes);

    void addNode(Node* n);
    int size() const { return nodes.size(); }
    bool linkNodes(Node* n1, Node* n2);

    Node* getNode(int index) { return nodes[index]; }

    tNodes& getNodes() { return nodes; }
    const tNodes& getNodes() const { return nodes; }
    tNodeGroups& getConnectedGroups() { return connectedGroups; }
    const tNodeGroups& getConnectedGroups() const { return connectedGroups; }
    tEdgesMatrix& getEdges() { return edges; }
    const tEdgesMatrix& getEdges() const { return edges; }

    FlowAlgorithm* getFlowAlgorithm() { return flower; }
    const FlowAlgorithm* getFlowAlgorithm() const { return flower; }

    tNodes getRoots();
    tNodes getLeaves();

    bool reflow(const Rectangle<int>& area,
                ObjController& objControler,
                float deltaTime);

    String toString();

    void setPositions();


private:

    tNodes nodes;
    tNodeGroups connectedGroups;
    ScopedPointer<FlowAlgorithm> flower;
    tEdgesMatrix edges;

};


}



#endif  // __DIRECTEDGRAPH_H_72CA91E6__
