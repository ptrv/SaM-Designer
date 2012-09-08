/*
  ==============================================================================

    ObjectFactory.cpp
    Created: 6 May 2012 11:40:40pm
    Author:  Peter Vasil

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

#include "../Application/CommonHeaders.h"
#include "../View/ObjectComponent.h"
#include "../View/LinkComponent.h"
#include "ObjectFactory.h"

namespace ObjectFactory
{

static ValueTree createNewMassTree(const String& newName, int x, int y)
{
	ValueTree newTree(Ids::mass);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
	ValueTree paramsTree(Ids::parameters);
    ValueTree pa1(Ids::parameter);
    ValueTree pa2(Ids::parameter);
    ValueTree pa3(Ids::parameter);
	pa1.setProperty(Ids::value, "0.001", nullptr);
	pa2.setProperty(Ids::value, "0.0", nullptr);
	pa3.setProperty(Ids::value, "0.0", nullptr);
    paramsTree.addChild(pa1, -1, nullptr);
    paramsTree.addChild(pa2, -1, nullptr);
    paramsTree.addChild(pa3, -1, nullptr);
	newTree.addChild(paramsTree, -1, nullptr);
	newTree.setProperty(Ids::identifier, newName, nullptr);
	ValueTree labelsTree(Ids::labels);
//    ValueTree l1(Ids::label);
//    l1.setProperty(Ids::value, "", nullptr);
//    labelsTree.addChild(l1, -1, nullptr);
	newTree.addChild(labelsTree, -1, nullptr);

	return newTree;
}

static ValueTree createNewPortTree(const String& newName, int x, int y)
{
	ValueTree newTree(Ids::port);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
    newTree.setProperty(Ids::identifier, newName, nullptr);
	ValueTree labelsTree(Ids::labels);
//    ValueTree l1(Ids::label);
//    l1.setProperty(Ids::value, "", nullptr);
//    labelsTree.addChild(l1, -1, nullptr);
	newTree.addChild(labelsTree, -1, nullptr);

	return newTree;
}

static ValueTree createNewGroundTree(const String& newName, int x, int y)
{
	ValueTree newTree(Ids::ground);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
	ValueTree paramsTree(Ids::parameters);
    ValueTree pa1(Ids::parameter);
	pa1.setProperty(Ids::value, "0.0", nullptr);
    paramsTree.addChild(pa1, -1, nullptr);
	newTree.addChild(paramsTree, -1, nullptr);
    newTree.setProperty(Ids::identifier, newName, nullptr);
	ValueTree labelsTree(Ids::labels);
//    ValueTree l1(Ids::label);
//    l1.setProperty(Ids::value, "", nullptr);
//    labelsTree.addChild(l1, -1, nullptr);
	newTree.addChild(labelsTree, -1, nullptr);

	return newTree;
}

static ValueTree createNewResonatorTree(const String& newName, int x, int y)
{
	ValueTree newTree(Ids::resonator);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
	ValueTree paramsTree(Ids::parameters);
    ValueTree pa1(Ids::parameter);
    ValueTree pa2(Ids::parameter);
    ValueTree pa3(Ids::parameter);
	pa1.setProperty(Ids::value, "200.0", nullptr);
    pa2.setProperty(Ids::value, "1.5", nullptr);
    pa3.setProperty(Ids::value, "0.01", nullptr);
    paramsTree.addChild(pa1, -1, nullptr);
    paramsTree.addChild(pa2, -1, nullptr);
    paramsTree.addChild(pa3, -1, nullptr);
    newTree.addChild(paramsTree, -1, nullptr);
    newTree.setProperty(Ids::identifier, newName, nullptr);
	ValueTree labelsTree(Ids::labels);
//    ValueTree l1(Ids::label);
//	l1.setProperty(Ids::value, "", nullptr);
//    labelsTree.addChild(l1, -1, nullptr);
	newTree.addChild(labelsTree, -1, nullptr);

	return newTree;
}

static ValueTree createNewLinkTree(const String& newName,
                                   const String& startObject,
                                   const String& endObject)
{
	ValueTree newTree(Ids::link);

	ValueTree paramsTree(Ids::parameters);
    ValueTree pa1(Ids::parameter);
    ValueTree pa2(Ids::parameter);
    ValueTree pa3(Ids::parameter);
    pa1.setProperty(Ids::value, "100.0", nullptr);
    pa2.setProperty(Ids::value, "0.1", nullptr);
    pa3.setProperty(Ids::value, "0.0", nullptr);
    paramsTree.addChild(pa1, -1, nullptr);
    paramsTree.addChild(pa2, -1, nullptr);
    paramsTree.addChild(pa3, -1, nullptr);

	newTree.addChild(paramsTree, -1, nullptr);
	newTree.setProperty(Ids::identifier, newName, nullptr);
	newTree.setProperty(Ids::startVertex, startObject, nullptr);
	newTree.setProperty(Ids::endVertex, endObject, nullptr);
	ValueTree labelsTree(Ids::labels);
//    ValueTree l1(Ids::label);
//	l1.setProperty(Ids::value, "", nullptr);
//    labelsTree.addChild(l1, -1, nullptr);
	newTree.addChild(labelsTree, -1, nullptr);

	return newTree;
}

static ValueTree createNewTouchTree(const String& newName,
                                    const String& startObject, 
                                    const String& endObject)
{
	ValueTree newTree(Ids::touch);

    ValueTree paramsTree(Ids::parameters);
        ValueTree pa1(Ids::parameter);
    ValueTree pa2(Ids::parameter);
    ValueTree pa3(Ids::parameter);
    pa1.setProperty(Ids::value, "100.0", nullptr);
    pa2.setProperty(Ids::value, "0.1", nullptr);
    pa3.setProperty(Ids::value, "0.0", nullptr);
    paramsTree.addChild(pa1, -1, nullptr);
    paramsTree.addChild(pa2, -1, nullptr);
    paramsTree.addChild(pa3, -1, nullptr);

    newTree.addChild(paramsTree, -1, nullptr);
    newTree.setProperty(Ids::identifier, newName, nullptr);
    newTree.setProperty(Ids::startVertex, startObject, nullptr);
	newTree.setProperty(Ids::endVertex, endObject, nullptr);
	ValueTree labelsTree(Ids::labels);
//    ValueTree l1(Ids::label);
//	l1.setProperty(Ids::value, "", nullptr);
//    labelsTree.addChild(l1, -1, nullptr);
	newTree.addChild(labelsTree, -1, nullptr);

	return newTree;
}
static ValueTree createNewPluckTree(const String& newName,
                                    const String& startObject, 
                                    const String& endObject)
{
	ValueTree newTree(Ids::pluck);

	ValueTree paramsTree(Ids::parameters);
    ValueTree pa1(Ids::parameter);
    ValueTree pa2(Ids::parameter);
    ValueTree pa3(Ids::parameter);
    ValueTree pa4(Ids::parameter);
    pa1.setProperty(Ids::value, "300.0", nullptr);
    pa2.setProperty(Ids::value, "0.1", nullptr);
    pa3.setProperty(Ids::value, "0.003", nullptr);
    pa4.setProperty(Ids::value, "0.005", nullptr);
    paramsTree.addChild(pa1, -1, nullptr);
    paramsTree.addChild(pa2, -1, nullptr);
    paramsTree.addChild(pa3, -1, nullptr);
    paramsTree.addChild(pa4, -1, nullptr);

	newTree.addChild(paramsTree, -1, nullptr);
	newTree.setProperty(Ids::identifier, newName, nullptr);
	newTree.setProperty(Ids::startVertex, startObject, nullptr);
	newTree.setProperty(Ids::endVertex, endObject, nullptr);
	ValueTree labelsTree(Ids::labels);
//    ValueTree l1(Ids::label);
//	l1.setProperty(Ids::value, "", nullptr);
//    labelsTree.addChild(l1, -1, nullptr);
	newTree.addChild(labelsTree, -1, nullptr);

	return newTree;
}

static ValueTree createNewWaveguideTree(const String& newName, int x, int y)
{
    ValueTree newTree(Ids::waveguide);

    ValueTree paramsTree(Ids::parameters);
    ValueTree pa1(Ids::parameter);
    pa1.setProperty(Ids::value, "1.0", nullptr);
    ValueTree stringType(Ids::string);
    stringType.setProperty(Ids::stringType, "simpleString", nullptr);
    ValueTree sp1(Ids::parameter);
    ValueTree sp2(Ids::parameter);
    sp1.setProperty(Ids::value, "0.033", nullptr);
    sp2.setProperty(Ids::value, "0.017", nullptr);
    stringType.addChild(sp1, -1, nullptr);
    stringType.addChild(sp2, -1, nullptr);

    paramsTree.addChild(pa1, -1, nullptr);
    paramsTree.addChild(stringType, -1, nullptr);

	newTree.addChild(paramsTree, -1, nullptr);

    newTree.setProperty(Ids::identifier, newName, nullptr);
    newTree.setProperty(Ids::objLeft, "", nullptr);
    newTree.setProperty(Ids::objRight, "", nullptr);
    ValueTree labelsTree(Ids::labels);
    newTree.addChild(labelsTree, -1, nullptr);

    return newTree;
}

static ValueTree createNewTerminationTree(const String& newName, int x, int y)
{
    ValueTree newTree(Ids::termination);

    ValueTree paramsTree(Ids::parameters);
    ValueTree termType(Ids::term);
    termType.setProperty(Ids::termType, "simpleStringTerm", nullptr);
//    ValueTree termParams(Ids::termParams);
    ValueTree tp1(Ids::parameter);
    ValueTree tp2(Ids::parameter);
    tp1.setProperty(Ids::value, "-0.996", nullptr);
    tp2.setProperty(Ids::value, "20", nullptr);
    termType.addChild(tp1, -1, nullptr);
    termType.addChild(tp2, -1, nullptr);
//    termType.addChild(termParams, -1, nullptr);
    paramsTree.addChild(termType, -1, nullptr);

    newTree.addChild(paramsTree, -1, nullptr);
    newTree.setProperty(Ids::identifier, newName, nullptr);
    ValueTree labelsTree(Ids::labels);
    newTree.addChild(labelsTree, -1, nullptr);

    return newTree;
}

static ValueTree createNewAudioOutTree(const String& newName, int x, int y)
{
	ValueTree newTree(Ids::audioout);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
	newTree.setProperty(Ids::identifier,newName, nullptr);
    ValueTree sources(Ids::sources);
    newTree.addChild(sources, -1, nullptr);

	return newTree;
}


ValueTree createNewObjectTree(const Identifier& objType,
                              const String& newName,
                              int x, int y)
{
	if(objType == Ids::mass)
		return createNewMassTree(newName, x, y);
	else if(objType == Ids::port)
		return createNewPortTree(newName, x, y);
	else if(objType == Ids::ground)
		return createNewGroundTree(newName, x, y);
	else if(objType == Ids::resonator)
		return createNewResonatorTree(newName, x, y);
	else if(objType == Ids::audioout)
		return createNewAudioOutTree(newName, x, y);
    else if(objType == Ids::waveguide)
        return createNewWaveguideTree(newName, x, y);
    else if(objType == Ids::termination)
        return createNewTerminationTree(newName, x, y);
	else
		return ValueTree::invalid;
}

ValueTree createNewLinkObjectTree(const Identifier& linkType,
                                  const String& newName,
                                  const String& startObject, 
                                  const String& endObject)
{
    if(linkType == Ids::link)
        return createNewLinkTree(newName, startObject, endObject);
	else if(linkType == Ids::touch)
		return createNewTouchTree(newName, startObject, endObject);
	else if(linkType == Ids::pluck)
		return createNewPluckTree(newName, startObject, endObject);
    else
		return ValueTree::invalid;
}

}

