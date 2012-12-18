/*
  ==============================================================================

    CircleFlowAlgorithm.cpp
    Created: 13 Nov 2012 10:44:03pm
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
#include "GraphUtils.h"
#include "Node.h"
#include "../Controller/ObjController.h"
#include "../View/BaseObjectComponent.h"
#include "../View/SelectableObject.h"
#include "../View/ObjectComponent.h"

#include "CircleFlowAlgorithm.h"

using namespace synthamodeler;

bool CircleFlowAlgorithm::reflow(DirectedGraph* g, int offsetX, int offsetY,
                                 int width, int height, ObjController& objController,
                                 float deltaTime)
{
    float interval = 2 * M_PI / (float) g->size();
    int cx = width/2;
    int cy = height/2;
//    float vl = cx - (2 * g->getNode(0)->r1) - 10;
    float vl = cx - 100;
    for (int a = 0; a < g->size(); ++a)
    {
        Point<int> nc = GraphUtils::rotateCoordinate(vl, 0, (float) a * interval);
        nc.x += offsetX;
        nc.y += offsetY;
        if(ObjectComponent* oc = objController.getObjectForId(g->getNode(a)->getLabel()))
        {
            Point<int> p(cx + nc.x*0.8, cy + nc.y*0.8);
            oc->setPosition(p, false);
        }
    }
    return true;
}