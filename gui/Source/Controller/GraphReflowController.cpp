/*
  ==============================================================================

    GraphReflowController.cpp
    Created: 24 Jun 2014 12:01:16am
    Author:  peter

  ==============================================================================
*/

#include "../Application/CommonHeaders.h"
#include "../Graph/Node.h"
#include "../View/SelectableObject.h"
#include "../View/BaseObjectComponent.h"
#include "../View/ContentComp.h"
#include "../View/ObjectComponent.h"
#include "../View/LinkComponent.h"
#include "../View/ObjectsHolder.h"
#include "../Utilities/StoredSettings.h"
#include "../Utilities/ObjectsHelper.h"
#include "../Graph/DirectedGraph.h"
#include "../Graph/GraphUtils.h"
#include "../Graph/FlowAlgorithm.h"
#include "../Graph/CircleFlowAlgorithm.h"
#include "../Graph/ForceDirectedFlowAlgorithm.h"
#include "../Controller/ObjController.h"
#

#include "GraphReflowController.h"

using namespace synthamodeler;

GraphReflowController::GraphReflowController(ObjController& objCtrl)
    : objController(objCtrl), objHolder(nullptr)
{
}

void GraphReflowController::startTimer(int intervalInMilliseconds)
{
    objController.setIsReflowing(true);
    Timer::startTimer(intervalInMilliseconds);
}

void GraphReflowController::stopTimer()
{
    objController.setIsReflowing(false);
    Timer::stopTimer();
}

void GraphReflowController::timerCallback()
{
    if (reflow())
    {
        graph = nullptr;
        objHolder = nullptr;
        stopTimer();
        DBG("stop timer");
    }
}

double timeStep = 0.6;

bool GraphReflowController::reflow()
{
    if (graph != nullptr && objHolder != nullptr)
    {

        bool done = graph->reflow(objHolder->getContentComp()->getViewPosition().x,
                                  objHolder->getContentComp()->getViewPosition().y,
                                  objHolder->getContentComp()->getViewWidth(),
                                  objHolder->getContentComp()->getViewHeight(),
                                  objController, timeStep);
        objHolder->repaint();
        return done;
    }
    return true;
}

void GraphReflowController::startReflow(ObjectsHolder* const objectsHolder, const int cmdId)
{
    objHolder = objectsHolder;

    if (isTimerRunning())
    {
        stopTimer();
    }

    graph = nullptr;

    timeStep = StoredSettings::getInstance()->getProps()
        .getDoubleValue("redrawparam_timestep", 0.6);

    if(cmdId == CommandIDs::redrawCircle)
    {
        graph = new DirectedGraph();
        ObjectsHelper::makeGraph(objController, *graph.get());
    }
    else if(cmdId == CommandIDs::redrawForceDirected)
    {
        graph = new DirectedGraph();
        ObjectsHelper::makeGraph(objController, *graph.get());
//        DBG(graph->toString());
        graph->setFlowAlgorithm(new ForceDirectedFlowAlgorithm());

        const bool randomizeNodes =
            StoredSettings::getInstance()->getProps()
                .getBoolValue("redrawparam_randomize", true);
        if (randomizeNodes)
        {
            GraphUtils::randomizeNodes(*graph,
                                       objHolder->getContentComp()->getViewPosition().x,
                                       objHolder->getContentComp()->getViewPosition().y,
                                       objHolder->getContentComp()->getViewWidth(),
                                       objHolder->getContentComp()->getViewHeight());
        }
    }

    objController.getUndoManager().beginNewTransaction();

    startTimer(100 * timeStep);
}

void GraphReflowController::stopReflow()
{
    stopTimer();
    graph->setPositions();
    objHolder = nullptr;
    graph = nullptr;
}
