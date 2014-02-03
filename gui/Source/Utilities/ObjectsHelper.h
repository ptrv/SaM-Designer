/*
  ==============================================================================

    ObjectsHelper.h
    Created: 3 Feb 2014 12:37:51am
    Author:  peter

  ==============================================================================
*/

#ifndef OBJECTSHELPER_H_INCLUDED
#define OBJECTSHELPER_H_INCLUDED


namespace synthamodeler
{

class ObjController;
class DirectedGraph;

class ObjectsHelper
{
public:

    static bool changeObjectNameInLink(const ObjController& objController,
                                       const String& oldName,
                                       const String& newName,
                                       UndoManager* undoManager);

    static void changeObjectNameInAudioSources(const ObjController& objController,
                                               const String& oldName,
                                               const String& newName,
                                               UndoManager* undoManager);

    static void tidyUpObjects(ObjController& objController);

    static void makeGraph(const ObjController& objController, DirectedGraph& graph);
};

}


#endif  // OBJECTSHELPER_H_INCLUDED
