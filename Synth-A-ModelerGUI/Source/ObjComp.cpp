/*
  ==============================================================================

    ObjComp.cpp
    Created: 11 Apr 2012 5:10:20pm
    Author:  Peter Vasil

  ==============================================================================
*/

#include "ObjComp.h"

ObjComp::ObjComp(ObjController& objController_)
: Component(), objController(objController_)
{
	setSize(100,100);
}

ObjComp::~ObjComp()
{

}

void ObjComp::paint(Graphics& g)
{
	g.fillAll (Colours::white);
	g.drawText("This will be the object component", 10, 10 ,
			getWidth()-20, getHeight()-20, Justification::centred, false);

}

//void ObjComp::resized()
//{
//
//}
