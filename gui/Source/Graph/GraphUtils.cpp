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

void GraphUtils::depthFirstSearch(DirectedGraph& g)
{
    tNodes& nodes = g.nodes;
    Array<tNodes>& connectedNodes = g.connectedNodes;
    Array<int> checkedNodes;

    connectedNodes.clear();

    auto fnIsInConnecteds = [&](Node* const n)
    {
        for(tNodes ns : connectedNodes)
        {
            if (ns.contains(n))
            {
                return true;
            }
        }
        return false;
    };

    auto fnGetNeightbours = [&](int idx)
    {
        Array<bool>& ed = g.edges.getReference(idx);
        tNodes no;

        for (int i = 0; i < ed.size(); ++i)
        {
            if (ed.getUnchecked(i))
            {
                no.add(nodes[i]);
            }
        }
        return no;
    };
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
                currentGroup.add(w);

                Array<Node*> neighbours = fnGetNeightbours(idxW);
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
            tNodes na;
            // na.add(v);
            fnDFS(v, na);
            connectedNodes.add(na);
        }
    }

    // String outStr;
    // for (tNodes group : connectedNodes)
    // {
    //     for (Node* const n : group)
    //     {
    //         outStr << n->getLabel() << " ";
    //     }
    //     outStr << newLine;
    // }
    // DBG(outStr);
}
