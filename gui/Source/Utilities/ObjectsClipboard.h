/*
  ==============================================================================

    ObjectsClipboard.h
    Created: 3 Feb 2014 1:05:41am
    Author:  peter

  ==============================================================================
*/

#ifndef OBJECTSCLIPBOARD_H_INCLUDED
#define OBJECTSCLIPBOARD_H_INCLUDED

namespace synthamodeler
{

class ObjController;
class ObjectsHolder;

class ObjectsClipboard
{
public:
    static const char* const clipboardXmlTag;
    static void copySelected(ObjController& objController);
    static void paste(ObjController& objController, ObjectsHolder& holder);
    static void cut(ObjController& objController, ObjectsHolder& holder);
};

}



#endif  // OBJECTSCLIPBOARD_H_INCLUDED
