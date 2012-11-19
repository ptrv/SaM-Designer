/*
  ==============================================================================

    Tree.h
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

#ifndef __TREE_H_56AD9D2__
#define __TREE_H_56AD9D2__


namespace synthamodeler
{

class DirectedGraph;
class Tree : public DirectedGraph
{
public:
    Tree(Node* r);
    virtual ~Tree();

    Node* getRoot() { return root; }

    int getDepth() { getDepth(root); }

    void addChild(Node* p, Node* c);
    int getDepth(Node* r);
    Node* root;
private:

};


}


#endif  // __TREE_H_56AD9D2__
