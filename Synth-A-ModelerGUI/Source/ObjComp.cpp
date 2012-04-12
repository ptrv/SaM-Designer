/*
  ==============================================================================

    ObjComp.cpp
    Created: 11 Apr 2012 5:10:20pm
    Author:  peter

  ==============================================================================
*/

#include "ObjComp.h"

ObjComp::ObjComp() : Component()
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
