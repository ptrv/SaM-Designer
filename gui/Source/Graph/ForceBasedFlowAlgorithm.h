/*
  ==============================================================================

    ForceBasedFlowAlgorithm.h
    Created: 15 Nov 2012 1:04:00am
    Author:  peter

  ==============================================================================
*/

#ifndef __FORCEBASEDFLOWALGORITHM_H_87FF6DF9__
#define __FORCEBASEDFLOWALGORITHM_H_87FF6DF9__


namespace synthamodeler
{

class FlowAlgorithm;
class DirectedGraph;
class ObjController;
class ForceBasedFlowAlgorithm : public FlowAlgorithm
{
public:
    ForceBasedFlowAlgorithm();
    virtual ~ForceBasedFlowAlgorithm();

    bool reflow(DirectedGraph* g, int offsetX, int offsetY,
                int width, int height, ObjController& objController,
                float deltaT);

private:

};

}

#endif  // __FORCEBASEDFLOWALGORITHM_H_87FF6DF9__
