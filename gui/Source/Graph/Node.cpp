/*
  ==============================================================================

    Node.cpp
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

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Models/ObjectIDs.h"

#include "Node.h"

using namespace synthamodeler;

Node::Node(ValueTree data_)
: force(0,0), velocity(0, 0), data(data_)
{
    label = data[Ids::identifier].toString();
}

Node::~Node()
{
}

void Node::addIncomingLink(Node* n)
{
    inLinks.addIfNotAlreadyThere(n);
}

void Node::addOutgoingLink(Node* n)
{
    outLinks.addIfNotAlreadyThere(n);
}

float Node::getShortestLinkLength()
{
    if(inLinks.size() == 0 && outLinks.size() == 0)
    {
        return -1.0f;
    }

    float l = 10000.0f;
    for (int i = 0; i < inLinks.size(); ++i)
    {
        int dx = inLinks.getUnchecked(i)->getNX() - this->getNX();
        int dy = inLinks.getUnchecked(i)->getNY() - this->getNY();

        float il = sqrt(dx*dx + dy*dy);
        if(il < l)
            l = il;
    }

    for (int j = 0; j < outLinks.size(); ++j)
    {
        int dx = outLinks.getUnchecked(j)->getNX() - this->getNX();
        int dy = outLinks.getUnchecked(j)->getNY() - this->getNY();

        float ol = sqrt(dx*dx + dy*dy);
        if(ol < l)
            l = ol;
    }
    return l;
}

bool Node::equals(const Node* other)
{
    if(this == other)
        return true;

    return label.equalsIgnoreCase(other->label);
}

void Node::setNX(int x)
{
    data.setProperty(Ids::posX, x, nullptr);
}
void Node::setNY(int y)
{
    data.setProperty(Ids::posY, y, nullptr);
}
int Node::getNX()
{
    return int(data[Ids::posX]);
}
int Node::getNY()
{
    return int(data[Ids::posY]);
}