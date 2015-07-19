/*
  ==============================================================================

    GraphReflowController.cpp
    Created: 24 Jun 2014 12:01:16am
    Author:  peter

  ==============================================================================
*/
#include "GraphReflowController.h"

#include "Application/CommonHeaders.h"

#include "ObjController.h"

#include "Graph/Node.h"
#include "Graph/DirectedGraph.h"
#include "Graph/GraphUtils.h"
#include "Graph/FlowAlgorithm.h"
#include "Graph/CircleFlowAlgorithm.h"
#include "Graph/ForceDirectedFlowAlgorithm.h"

#include "View/SelectableObject.h"
#include "View/BaseObjectComponent.h"
#include "View/ContentComp.h"
#include "View/ObjectComponent.h"
#include "View/LinkComponent.h"
#include "View/ObjectsHolder.h"
#include "View/Connector.h"

#include "Utilities/StoredSettings.h"
#include "Utilities/ObjectsHelper.h"




using namespace synthamodeler;

GraphReflowController::GraphReflowController(ObjController& objCtrl)
    : objController(objCtrl), objHolder(nullptr)
{
}

void GraphReflowController::makeGraph(const bool includeMiscObjects)
{
    if (!graph)
    {
        return;
    }

    // add nodes
    for (const LinkComponent* const l : objController.getLinks())
    {
        ObjectComponent* const ocStart =
            objController.getObjectForId(l->getData()[Ids::startVertex].toString());
        ObjectComponent* const ocEnd =
            objController.getObjectForId(l->getData()[Ids::endVertex].toString());

        graph->addNode(ocStart);
        graph->addNode(ocEnd);
    }

    if (includeMiscObjects)
    {
        for (Connector* const conn : objController.getConnectors())
        {
            graph->addNode(conn->getSourceObject());
            graph->addNode(conn->getTargetObject());
        }
    }

    tNodes& nodes = graph->getNodes();

    graph->init(nodes.size());

    // link nodes
    for (const LinkComponent* const l : objController.getLinks())
    {
        ObjectComponent* const ocStart =
            objController.getObjectForId(l->getData()[Ids::startVertex].toString());
        ObjectComponent* const ocEnd =
            objController.getObjectForId(l->getData()[Ids::endVertex].toString());

        graph->linkNodes(ocStart, ocEnd);
    }

    if (includeMiscObjects)
    {
        for (Connector* const conn : objController.getConnectors())
        {
            graph->linkNodes(conn->getSourceObject(), conn->getTargetObject());
        }
    }

    // init node data
    std::for_each(nodes.begin(), nodes.end(),
                  [](Node* const n) { n->initNodeData(); });

    // find combined groups in graph
    GraphUtils::calculateConnectedGroups(*graph);

    // String outStr;
    // for (int i = 0; i < graph.edges.size(); ++i)
    // {
    //     for (int j = 0; j < graph.edges[i].size(); ++j)
    //     {
    //         outStr << (graph.edges[i][j] ? "1" : "0");
    //     }
    //     outStr << newLine;
    // }
    // DBG(outStr);
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
    if (!reflow())
    {
        stopReflow();
        DBG("stop timer");
    }
}

int timeStep = 60;

bool GraphReflowController::reflow()
{
    if (graph != nullptr && objHolder != nullptr)
    {
        bool needsIteration = graph->reflow(bounds, objController, timeStep);
        objHolder->repaint();
        return needsIteration;
    }
    return false;
}

void GraphReflowController::startReflow(ObjectsHolder& objectsHolder, const int cmdId)
{
    objHolder = &objectsHolder;

    if (isTimerRunning())
    {
        stopTimer();
    }

    graph = nullptr;

    timeStep = StoredSettings::getInstance()->getProps()
        .getDoubleValue("redrawparam_timestep", 60);

    graph = new DirectedGraph();
    makeGraph(true);

    if(cmdId == CommandIDs::redrawForceDirected)
    {
        graph->setFlowAlgorithm(new ForceDirectedFlowAlgorithm());

        const bool randomizeNodes =
            StoredSettings::getInstance()->getProps()
                .getBoolValue("redrawparam_randomize", true);

        const ContentComp* const cc = objHolder->getContentComp();
        bounds = Rectangle<int>(cc->getViewPosition().x, cc->getViewPosition().y,
                                cc->getViewWidth(), cc->getViewHeight());
        if (randomizeNodes)
        {
            GraphUtils::randomizeNodes(*graph, bounds);
        }
    }

    objController.getUndoManager().beginNewTransaction();

    startTimer(timeStep);
}

void GraphReflowController::stopReflow()
{
    stopTimer();
    graph->setPositions();
    graph = nullptr;
    objHolder = nullptr;
}
