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
#include "../View/SelectableObject.h"
#include "../View/BaseObjectComponent.h"
#include "../View/ObjectComponent.h"
#include "../View/LinkComponent.h"
#include "ObjectFactory.h"

using namespace synthamodeler;

static ValueTree createNewMassTree(const String& newName, int x, int y)
{
	ValueTree newTree(Ids::mass);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);

    StringArray p;
    p.add("0.001");
    p.add("0.0");
    p.add("0.0");
	ValueTree paramsTree = ObjectFactory::createParamsTree(p);
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

    StringArray p;
    p.add("200.0");
    p.add("1.5");
    p.add("0.01");
	ValueTree paramsTree = ObjectFactory::createParamsTree(p);
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
    StringArray p;
    p.add("100.0");
    p.add("0.1");
    p.add("0.0");
	ValueTree paramsTree = ObjectFactory::createParamsTree(p);
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
    StringArray p;
    p.add("100.0");
    p.add("0.1");
    p.add("0.0");
    ValueTree paramsTree = ObjectFactory::createParamsTree(p);
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

    StringArray p;
    p.add("300.0");
    p.add("0.1");
    p.add("0.003");
    p.add("0.0");
    ValueTree paramsTree = ObjectFactory::createParamsTree(p);
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

static ValueTree createNewWaveguideTree(const String& newName,
                                        const String& startObject,
                                        const String& endObject)
{
    ValueTree newTree(Ids::waveguide);

    StringArray p;
    p.add("1.0");
    p.add("simpleString(0.033,0.017)");
    ValueTree paramsTree = ObjectFactory::createParamsTree(p);
	newTree.addChild(paramsTree, -1, nullptr);

    newTree.setProperty(Ids::identifier, newName, nullptr);
    newTree.setProperty(Ids::startVertex, startObject, nullptr);
    newTree.setProperty(Ids::endVertex, endObject, nullptr);
    ValueTree labelsTree(Ids::labels);
    newTree.addChild(labelsTree, -1, nullptr);

    return newTree;
}

static ValueTree createNewTerminationTree(const String& newName, int x, int y)
{
    ValueTree newTree(Ids::termination);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
    ValueTree paramsTree(Ids::parameters);
    ValueTree term(Ids::parameter);
    term.setProperty(Ids::value, "simpleStringTerm(-0.996, 20)", nullptr);
    paramsTree.addChild(term, -1, nullptr);

    newTree.addChild(paramsTree, -1, nullptr);
    newTree.setProperty(Ids::identifier, newName, nullptr);
    ValueTree labelsTree(Ids::labels);
    newTree.addChild(labelsTree, -1, nullptr);

    return newTree;
}

static ValueTree createNewJunctionTree(const String& newName, int x, int y)
{
    ValueTree newTree(Ids::junction);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
    ValueTree paramsTree(Ids::parameters);
    ValueTree pa(Ids::parameter);
    pa.setProperty(Ids::value, "0.0", nullptr);
    paramsTree.addChild(pa, -1, nullptr);

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


ValueTree ObjectFactory::createNewObjectTree(const Identifier& objType,
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
    else if(objType == Ids::junction)
        return createNewJunctionTree(newName, x, y);
    else if(objType == Ids::termination)
        return createNewTerminationTree(newName, x, y);
	else
		return ValueTree::invalid;
}

ValueTree ObjectFactory::createNewLinkObjectTree(const Identifier& linkType,
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
	else if(linkType == Ids::waveguide)
		return createNewWaveguideTree(newName, startObject, endObject);
    else
		return ValueTree::invalid;
}
ValueTree ObjectFactory::createParamsTree(StringArray p)
{
    ValueTree paramsTree(Ids::parameters);
    for (int i = 0; i < p.size(); ++i)
    {
        ValueTree value(Ids::parameter);
        value.setProperty(Ids::value, p[i].trim(), nullptr);
        paramsTree.addChild(value, -1, nullptr);
    }
    return paramsTree;
}

ValueTree ObjectFactory::createLabelsTree(StringArray p)
{
    ValueTree labelsTree(Ids::labels);
    for (int i = 0; i < p.size(); ++i)
    {
        ValueTree label(Ids::label);
        label.setProperty(Ids::value, p[i].trim(), nullptr);
        labelsTree.addChild(label, -1, nullptr);
    }
    return labelsTree;
}
