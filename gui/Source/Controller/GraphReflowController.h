/*
  ==============================================================================

    GraphReflowController.h
    Created: 24 Jun 2014 12:01:16am
    Author:  peter

  ==============================================================================
*/

#ifndef GRAPHREFLOWCONTROLLER_H_INCLUDED
#define GRAPHREFLOWCONTROLLER_H_INCLUDED

#include "JuceHeader.h"

namespace synthamodeler
{

class DirectedGraph;
class ObjController;
class ObjectsHolder;

class GraphReflowController : public Timer
{
public:
    GraphReflowController(ObjController& objCtrl);

    void startReflow(ObjectsHolder& objectsHolder, int cmdId);
    void stopReflow();

private:

    void makeGraph(bool includeMiscObjects);

    void startTimer(int intervalInMilliseconds);
    void stopTimer();
    void timerCallback();
    bool reflow();

    ObjController& objController;
    ObjectsHolder* objHolder;
    ScopedPointer<DirectedGraph> graph;

    Rectangle<int> bounds;
};

}


#endif  // GRAPHREFLOWCONTROLLER_H_INCLUDED
