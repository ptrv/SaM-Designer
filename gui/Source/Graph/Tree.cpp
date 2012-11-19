/*
  ==============================================================================

    Tree.cpp
    Created: 13 Nov 2012 6:47:29pm
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
#include "TreeFlowAlgorithm.h"
#include "DirectedGraph.h"
#include "Node.h"

#include "Tree.h"

using namespace synthamodeler;

Tree::Tree(Node* r) : root(r)
{
    nodes.add(root);
    flower = new TreeFlowAlgorithm();
}

Tree::~Tree()
{

}

void Tree::addChild(Node* p, Node* c)
{
    nodes.add(c);
    linkNodes(p, c);
}

int Tree::getDepth(Node* r)
{
    if (r->getOutgoingLinksCount() == 0)
        return 1;
    int d = 0;
    Array<Node*>& outgoing = r->getOutgoingLinks();

    for (int i = 0; i < outgoing.size(); ++i)
    {
      int dc = getDepth(outgoing.getUnchecked(i));
      if(dc > d)
          d=dc;
    }
    return 1+d;
}
