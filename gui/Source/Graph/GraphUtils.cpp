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

#include "GraphUtils.h"

#include "Node.h"

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

Point<float> GraphUtils::coulombForce(const Point<float>& p1, const Point<float>& p2, float beta)
{
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;

    float dsq = dx * dx + dy * dy;

    if (dsq == 0.f)
        dsq = 0.001f;

    float coul = beta / dsq;

    return Point<float>(coul * dx, coul * dy);
}

Point<float> GraphUtils::hookeForce(const Point<float>& p1, const Point<float>& p2, float k)
{
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;

    return Point<float>(k * dx, k * dy);
}

void GraphUtils::randomizeNodes(DirectedGraph& g, const Rectangle<int>& area)
{
    tNodes& nodes = g.getNodes();
    Random rnd(Time::currentTimeMillis());

    const int offsetX = area.getX();
    const int offsetY = area.getY();
    const int width = area.getWidth();
    const int height = area.getHeight();

    for (Node* const n : nodes)
    {
//        int x = n->getNX();
//        int y = n->getNY();
//        float f1 = 1.0f;
//        float f2 = f1/2.0f;
////        int rx = x + rnd.nextInt(width*f1) - width*f2;
////        int ry = y + rnd.nextInt(height*f1) - height*f2;
        int rx = rnd.nextInt(width) + offsetX;
        int ry = rnd.nextInt(height) + offsetY;

//        while(rx < 0 || ry > width)
//            rx = x + rnd.nextInt(width*f1) - width*f2;
//        while(ry < 0 || ry > height)
//            ry = y + rnd.nextInt(height*f1) - height*f2;

//        if(rx < 0)
//            rx = abs(rx);
//        else if(rx > width)
//            rx = width - (rx - width);
//        if(ry < 0)
//            ry = abs(ry);
//        else if(ry > height)
//            ry = height - (ry - height);
        if(rx < offsetX)
            rx = offsetX;
        else if(rx > width + offsetX)
            rx = width + offsetX;
        if(ry < offsetY)
            ry = offsetY;
        else if(ry > height + offsetY)
            ry = height + offsetY;

        n->setPosF(rx, ry);
    }
}

// http://stackoverflow.com/questions/922256/c-array-shuffle
void GraphUtils::shuffleNodes(DirectedGraph& g)
{
    tNodes& nodes = g.getNodes();
    const int numNodes = nodes.size();
    int numNodes2 = numNodes;
    while(numNodes2 > 1)
    {
        long int k = lrand48();
        k = k % numNodes;
        numNodes2--;
        nodes.swap(numNodes2, k);
//        Node* temp = nodes[numNodes2];
//        nodes[numNodes2] = nodes[k];
//        nodes[k] = temp;
    }
}

void GraphUtils::initEdgesMatrix(tEdgesMatrix& edgesMatrix, int numNodes)
{
    Array<bool> row;
    row.insertMultiple(0, false, numNodes);
    edgesMatrix.insertMultiple(0, row, numNodes);
}

void GraphUtils::calculateConnectedGroups(DirectedGraph& g)
{
    tNodes& allNodes = g.getNodes();
    tNodeGroups& groups = g.getConnectedGroups();

    groups.clear();

    Array<int> visitedNodes;

    for (Node* const v : allNodes)
    {
        const bool isNodeInAnyGroup =
            std::any_of(groups.begin(), groups.end(),
                        [&v](const tNodesAndEdges& group)
        {
            return group.first.contains(v);
        });

        if (!isNodeInAnyGroup)
        {
            tNodesAndEdges ne;
            tNodes& group = ne.first;

            // depth-first search
            std::stack<Node*> S;
            S.push(v);

            while (!S.empty())
            {
                Node* aNode = S.top();
                S.pop();

                int aNodeIndex = allNodes.indexOf(aNode);
                if (!visitedNodes.contains(aNodeIndex))
                {
                    visitedNodes.add(aNodeIndex);

                    group.add(aNode);

                    for (Node* const x : aNode->getNeighbours())
                    {
                        S.push(x);
                    }
                }
            }

            groups.add(ne);
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
    for (tNodesAndEdges& ne : groups)
    {
        const tNodes& nodes = ne.first;
        tEdgesMatrix& edges = ne.second;

        const int numNodes = nodes.size();

        initEdgesMatrix(edges, numNodes);

        for (int i = 0; i < numNodes; ++i)
        {
            const Node* const n = nodes.getUnchecked(i);
            Array<bool>& ed = edges.getReference(i);
            const tNodes& neighbours = n->getNeighbours();
            for (Node* const u : neighbours)
            {
                const int inIdx = nodes.indexOf(u);
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
