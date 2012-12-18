/*
  ==============================================================================

    CircleFlowAlgorithm.h
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

#ifndef __CIRCLEFLOWALGORITHM_H_6B9B5EB1__
#define __CIRCLEFLOWALGORITHM_H_6B9B5EB1__


namespace synthamodeler
{

class FlowAlgorithm;
class DirectedGraph;
class ObjController;
class CircleFlowAlgorithm : public FlowAlgorithm
{
public:
    bool reflow(DirectedGraph* g, int offsetX, int offsetY,
                int width, int height, ObjController& objController,
                float deltaTime);
};


}


#endif  // __CIRCLEFLOWALGORITHM_H_6B9B5EB1__
