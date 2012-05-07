/*
  ==============================================================================

    ObjectFactory.h
    Created: 6 May 2012 11:40:40pm
    Author:  peter

  ==============================================================================
*/

#ifndef __OBJECTFACTORY_H_2132166E__
#define __OBJECTFACTORY_H_2132166E__

#include "../Application/CommonHeaders.h"

namespace ObjectFactory
{

	ValueTree createNewObjectTree(const Identifier& objType, int x, int y);

}


#endif  // __OBJECTFACTORY_H_2132166E__
