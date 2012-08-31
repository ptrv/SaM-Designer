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
    ValueTree l1(Ids::label);
	l1.setProperty(Ids::value, "label_"+newName, nullptr);
    labelsTree.addChild(l1, -1, nullptr);
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
    ValueTree l1(Ids::label);
	l1.setProperty(Ids::value, "label_"+newName, nullptr);
    labelsTree.addChild(l1, -1, nullptr);
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
    ValueTree l1(Ids::label);
	l1.setProperty(Ids::value, "label_"+newName, nullptr);
    labelsTree.addChild(l1, -1, nullptr);
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
    ValueTree l1(Ids::label);
	l1.setProperty(Ids::value, "label_"+newName, nullptr);
    labelsTree.addChild(l1, -1, nullptr);
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
    ValueTree l1(Ids::label);
	l1.setProperty(Ids::value, "label_"+newName, nullptr);
    labelsTree.addChild(l1, -1, nullptr);
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
    ValueTree l1(Ids::label);
	l1.setProperty(Ids::value, "label_"+newName, nullptr);
    labelsTree.addChild(l1, -1, nullptr);
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
    ValueTree l1(Ids::label);
	l1.setProperty(Ids::value, "label_"+newName, nullptr);
    labelsTree.addChild(l1, -1, nullptr);
	newTree.addChild(labelsTree, -1, nullptr);

	return newTree;
}
static ValueTree createNewAudioOutTree(const String& newName, int x, int y)
{
	ValueTree newTree(Ids::audioout);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
	int rnd = Random::getSystemRandom().nextInt(100000);
	newTree.setProperty(Ids::identifier,"a_"+String(rnd), nullptr);
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

ObjectComponent* createNewObjectComponentFromTree(ObjController& owner, ValueTree objTree, int index)
{
    ObjectComponent* const objComp = new ObjectComponent(owner, objTree);
    
    owner.addComponent(objComp, index);
    return objComp;
}

LinkComponent* createNewLinkComponentFromTree(ObjController& owner, ValueTree linkTree, int index)
{
    LinkComponent* const linkComp = new LinkComponent(owner, linkTree);
    
    owner.addLinkComponent(linkComp, index);
    return linkComp;
}

}

