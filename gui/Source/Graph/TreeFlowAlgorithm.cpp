/*
  ==============================================================================

    TreeFlowAlgorithm.cpp
    Created: 13 Nov 2012 10:43:28pm
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
#include "DirectedGraph.h"
#include "Tree.h"
#include "Node.h"
#include "FlowAlgorithm.h"

#include "TreeFlowAlgorithm.h"

using namespace synthamodeler;

bool TreeFlowAlgorithm::reflow(DirectedGraph* g, int offsetX, int offsetY,
                               int width, int height, ObjController& objController,
                               float deltaTime)
{
    if (Tree * t = dynamic_cast<Tree*> (g))
    {
        //            Tree t = (Tree) g;
        int depth = t->getDepth();
        int vstep = (height - 20) / depth;
        int vpos = 30;

        Node* first = t->root;
//        first->setNX(width / 2);
//        first->setNY(vpos);

        // breadth-first iteration
        Array<Node*>& children = t->root->getOutgoingLinks();
        while (children.size() > 0)
        {
            vpos += vstep;
            int cnum = children.size();
            int hstep = (width - 20) / cnum;
            int hpos = 10 + (hstep / 2);
            Array<Node*> newnodes;
            for (int i = 0; i < children.size(); ++i)
            {
                Node* child = children.getUnchecked(i);
//                child->setNX(hpos);
//                child->setNY(vpos);
                newnodes.addArray(child->getOutgoingLinks());
                //                    addAll(newnodes, child.getOutgoingLinks());
                hpos += hstep;
            }
            children = newnodes;
        }
    }
    return true;
}
