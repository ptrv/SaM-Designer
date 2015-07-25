/*
  ==============================================================================

    GraphUtils.h
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

#ifndef __GRAPHUTILS_H_ECA0AD77__
#define __GRAPHUTILS_H_ECA0AD77__

#include "JuceHeader.h"

#include "DirectedGraph.h"

namespace synthamodeler
{

class GraphUtils
{

public:
    static float getDirection(double dx, double dy);
    static Point<int> rotateCoordinate(float x, float y, float angle);
    static Point<float> coulombForce(const Point<float>& p1, const Point<float>& p2, float beta);
    static Point<float> hookeForce(const Point<float>& p1, const Point<float>& p2, float k);

    static void randomizeNodes(DirectedGraph& g, const Rectangle<int>& area);
    static void shuffleNodes(DirectedGraph& g);

    static void initEdgesMatrix(tEdgesMatrix& edgesMatrix, int numNodes);
    static void clearEdgesMatrix(tEdgesMatrix& edgesMatrix);

    static void calculateConnectedGroups(DirectedGraph& g);

};

}


#endif  // __GRAPHUTILS_H_ECA0AD77__
