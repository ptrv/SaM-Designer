/*
  ==============================================================================

    ForceDirectedFlowAlgorithm.h
    Created: 13 Nov 2012 6:48:50pm
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

#ifndef __FORCEDIRECTEDFLOWALGORITHM_H_F765C202__
#define __FORCEDIRECTEDFLOWALGORITHM_H_F765C202__


namespace synthamodeler
{

class FlowAlgorithm;
class DirectedGraph;
class ObjController;
class ForceDirectedFlowAlgorithm : public FlowAlgorithm
{
public:
    ForceDirectedFlowAlgorithm();
    virtual ~ForceDirectedFlowAlgorithm();

    bool reflow(DirectedGraph* g, int width, int height, ObjController& objController);

    void setElasticity(float e) { elasticity = e; }
    void setRepulsion(float r) { repulsion = r; }
private:

    float minSize;
    float elasticity;
    float repulsion;
};


}


#endif  // __FORCEDIRECTEDFLOWALGORITHM_H_F765C202__
