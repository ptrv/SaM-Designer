/*
  ==============================================================================

    GraphUtils.cpp
    Created: 13 Nov 2012 8:30:09pm
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

#include "GraphUtils.h"
#include "Node.h"
#include "DirectedGraph.h"

#include <stack>

using namespace synthamodeler;

float GraphUtils::getDirection(double dx, double dy)
{
    // quadrant offsets
    double d1 = 0.0;
    double d2 = M_PI / 2.0;
    double d3 = M_PI;
    double d4 = 3.0 * M_PI / 2.0;
    // compute angle basd on dx and dy values
    double angle = 0;
    float adx = abs((float) dx);
    float ady = abs((float) dy);
    // Vertical lines are one of two angles
    if (dx == 0)
    {
        angle = (dy >= 0 ? d2 : d4);
    }
        // Horizontal lines are also one of two angles
    if (dy == 0)
    {
        angle = (dx >= 0 ? d1 : d3);
    }
        // The rest requires trigonometry (note: two use dx/dy and two use dy/dx!)
    else if (dx > 0 && dy > 0)
    {
        angle = d1 + atan(ady / adx);
    } // direction: X+, Y+
    else if (dx < 0 && dy > 0)
    {
        angle = d2 + atan(adx / ady);
    } // direction: X-, Y+
    else if (dx < 0 && dy < 0)
    {
        angle = d3 + atan(ady / adx);
    } // direction: X-, Y-
    else if (dx > 0 && dy < 0)
    {
        angle = d4 + atan(adx / ady);
    } // direction: X+, Y-
    // return directionality in positive radians
    return fmod ((float) (angle + 2 * M_PI), (2 * M_PI));
}

Point<int> GraphUtils::rotateCoordinate(float x, float y, float angle)
{
    Point<int> rc(0, 0);
    rc.x = (int) (x * cos(angle) - y * sin(angle));
    rc.y = (int) (x * sin(angle) + y * cos(angle));
    return rc;
}

Point<float> GraphUtils::coulombForce(Point<float> p1, Point<float> p2, float beta)
{
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;

    float ds2 = dx * dx + dy * dy;
    float ds = sqrt(ds2);
//    float dsTmp = ds2 * ds;
    float c = 0.0f;
//    if(dsTmp != 0.0f)
//        c = beta / dsTmp;
//        c = beta / ds2;
        c = beta / ds;

    return Point<float>(-c * dx, -c * dy);
}

Point<float> GraphUtils::hookeForce(Point<float> p1, Point<float> p2, float dij, float k)
{
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;

    float ds2 = dx * dx + dy * dy;
    float ds = sqrt(ds2);
    float dl = ds - dij;
    float c = 0.0f;

    if(ds != 0.0f)
        c = k * dl / ds;

    return Point<float>(c * dx, c * dy);
}

void GraphUtils::calculateConnectedGroups(DirectedGraph& g)
{
    tNodes& nodes = g.nodes;
    tEdgesMatrix& allEdges = g.edges;
    Array<tNodesAndEdges>& connectedGroups = g.connectedGroups;

    Array<int> checkedNodes;

    connectedGroups.clear();

    auto fnIsInConnecteds = [&connectedGroups](Node* const node)
    {
        for(const tNodesAndEdges& group : connectedGroups)
        {
            if (group.nodes.contains(node))
            {
                return true;
            }
        }
        return false;
    };

    auto fnGetNeightbours = [&allEdges, &nodes](int idx)
    {
        const Array<bool>& edges = allEdges.getReference(idx);
        tNodes neighbours;

        for (int i = 0; i < edges.size(); ++i)
        {
            if (edges.getUnchecked(i))
            {
                neighbours.add(nodes[i]);
            }
        }
        return neighbours;
    };

    // depth first search
    auto fnDFS = [&](Node* const n, tNodes& currentGroup)
    {
        std::stack<Node*> S;
        S.push(n);
        while (!S.empty())
        {
            Node* w = S.top();
            S.pop();

            int idxW = nodes.indexOf(w);
            if (!checkedNodes.contains(idxW))
            {
                checkedNodes.add(idxW);

                const tNodes neighbours = fnGetNeightbours(idxW);
                w->setNeighbours(neighbours);

                currentGroup.add(w);

                for (Node* const x : neighbours)
                {
                    S.push(x);
                }
            }
        }
    };

    for (Node* const v : nodes)
    {
        if (!fnIsInConnecteds(v))
        {
            tNodes group;
            tNodesAndEdges nodesAndEdges;

            fnDFS(v, group);
            nodesAndEdges.nodes = group;
            connectedGroups.add(nodesAndEdges);
        }
    }

    // // dbg output
    // {
    //     String outStr;
    //     for (tNodesAndEdges group : connectedNodes)
    //     {
    //         for (Node* const n : group.nodes)
    //         {
    //             outStr << n->getLabel() << " ";
    //         }
    //         outStr << newLine;
    //     }
    //     DBG(outStr);
    // }

    // find edges for group nodes
    for (tNodesAndEdges& ne : g.connectedGroups)
    {
        int numNodes = ne.nodes.size();

        Array<bool> arr;
        arr.insertMultiple(0, false, numNodes);
        ne.edges.insertMultiple(0, arr, numNodes);

        for (int i = 0; i < numNodes; ++i)
        {
            Node* const n = ne.nodes.getUnchecked(i);
            Array<bool>& ed = ne.edges.getReference(i);
            const tNodes& neighbours = n->getNeighbours();
            for (Node* const u : neighbours)
            {
                const int inIdx = ne.nodes.indexOf(u);
                ed.set(inIdx, true);
            }
        }
    }

    // for (tNodesAndEdges& ne : g.connectedGroups)
    // {
    //     String outStr;
    //     for (int i = 0; i < ne.edges.size(); ++i)
    //     {
    //         for (int j = 0; j < ne.edges[i].size(); ++j)
    //         {
    //             outStr << (ne.edges[i][j] ? "1" : "0");
    //         }
    //         outStr << newLine;
    //     }
    //     outStr << newLine;
    //     DBG(outStr);
    // }
}
