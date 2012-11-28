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

namespace synthamodeler
{

class Node
{
public:
    Node(ValueTree data);
    virtual ~Node();

    void addIncomingLink(Node* n);
    Array<Node*>& getIncomingLinks() { return inLinks; }
    int getIncomingLinksCount() const { return inLinks.size(); }

    void addOutgoingLink(Node* n);
    Array<Node*>& getOutgoingLinks() { return outLinks; }
    int getOutgoingLinksCount() const { return outLinks.size(); }

    float getShortestLinkLength();

    bool equals(const Node* other);

    void setLabel(const String& s) { label = s; }
    const String& getLabel() { return label; }

    void setNX(float x_);
    void setNY(float y_);
    float getNX();
    float getNY();

    float getMass();
    Point<float> force;
    Point<float> velocity;
private:

    float x, y;
//    ValueTree data;
    String label;
    Array<Node*> inLinks;
    Array<Node*> outLinks;

    float mass;
};

}

#define __NODE_H_FB030B36__





#endif  // __NODE_H_FB030B36__
