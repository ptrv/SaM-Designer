/*
  ==============================================================================

    Node.h
    Created: 13 Nov 2012 6:44:30pm
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

#ifndef __NODE_H_FB030B36__

#include "JuceHeader.h"

namespace synthamodeler
{

class Node;
typedef Array<Node*> tNodes;

class Node
{
public:
    Node(ValueTree data);
    virtual ~Node();

    void initNodeData();

    void addIncomingLink(Node* n);
    tNodes& getIncomingLinks() { return inLinks; }
    int getIncomingLinksCount() const { return inLinks.size(); }

    void addOutgoingLink(Node* n);
    tNodes& getOutgoingLinks() { return outLinks; }
    int getOutgoingLinksCount() const { return outLinks.size(); }

    void setNeighbours(const Array<Node*>& n);
    const tNodes& getNeighbours() const;
    void addNeighbour(Node* n);
    float getShortestLinkLength();

    bool equals(const Node* other);

    void setLabel(const String& s) { label = s; }
    const String& getLabel() { return label; }

    void setPosF(float x_, float y_) { posF.setXY(x_, y_); };
    const Point<float>& getPosF() const { return posF; };
    Point<float>& getPosF() { return posF; };

    float getMass();
    Point<float>& getForce() { return force; };
    const Point<float>& getForce() const { return force; };
    Point<float>& getVelocity() { return velocity; };
    const Point<float>& getVelocity() const { return velocity; };

private:

    Point<float> posF;
    Point<float> force;
    Point<float> velocity;

    ValueTree data;
    String label;
    tNodes inLinks;
    tNodes outLinks;
    tNodes neighbours;

    float mass;
};

}

#define __NODE_H_FB030B36__





#endif  // __NODE_H_FB030B36__
