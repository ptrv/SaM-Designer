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

static ValueTree createNewMassTree(int x, int y)
{
	ValueTree newTree(Ids::mass);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
	ValueTree paramsTree(Ids::parameters);
	paramsTree.setProperty(Ids::idx[0], "0.001", nullptr);
	paramsTree.setProperty(Ids::idx[1], "0.0", nullptr);
	paramsTree.setProperty(Ids::idx[2], "0.0", nullptr);
	newTree.addChild(paramsTree, -1, nullptr);
	int rnd = Random::getSystemRandom().nextInt(100000);

	newTree.setProperty(Ids::identifier,"m_"+String(rnd), nullptr);
	ValueTree labelsTree(Ids::labels);
	labelsTree.setProperty(Ids::idx[0], "label_"+String(rnd), nullptr);
	newTree.addChild(labelsTree, -1, nullptr);

	return newTree;
}

static ValueTree createNewPortTree(int x, int y)
{
	ValueTree newTree(Ids::port);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
	int rnd = Random::getSystemRandom().nextInt(100000);
	newTree.setProperty(Ids::identifier, "dev_"+String(rnd), nullptr);
	ValueTree labelsTree(Ids::labels);
	labelsTree.setProperty(Ids::idx[0], "label_"+String(rnd), nullptr);
	newTree.addChild(labelsTree, -1, nullptr);

	return newTree;
}

static ValueTree createNewGroundTree(int x, int y)
{
	ValueTree newTree(Ids::ground);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
	ValueTree paramsTree(Ids::parameters);
	paramsTree.setProperty(Ids::idx[0], 0.0f, nullptr);
	newTree.addChild(paramsTree, -1, nullptr);
	int rnd = Random::getSystemRandom().nextInt(100000);
	newTree.setProperty(Ids::identifier, "g_"+String(rnd), nullptr);
	ValueTree labelsTree(Ids::labels);
	labelsTree.setProperty(Ids::idx[0], "label_"+String(rnd), nullptr);
	newTree.addChild(labelsTree, -1, nullptr);

	return newTree;
}

static ValueTree createNewResonatorTree(int x, int y)
{
	ValueTree newTree(Ids::resonator);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
	ValueTree paramsTree(Ids::parameters);
	paramsTree.setProperty(Ids::idx[0], 0.0f, nullptr);
	newTree.addChild(paramsTree, -1, nullptr);
	int rnd = Random::getSystemRandom().nextInt(100000);
	newTree.setProperty(Ids::identifier, "r_"+String(rnd), nullptr);
	ValueTree labelsTree(Ids::labels);
	labelsTree.setProperty(Ids::idx[0], "label_"+String(rnd), nullptr);
	newTree.addChild(labelsTree, -1, nullptr);

	return newTree;
}

static ValueTree createNewLinkTree(String startObject, String endObject)
{
	ValueTree newTree(Ids::link);

//	newTree.setProperty(Ids::posX, x, nullptr);
//	newTree.setProperty(Ids::posY, y, nullptr);
	ValueTree paramsTree(Ids::parameters);
	paramsTree.setProperty(Ids::idx[0], "", nullptr);
	newTree.addChild(paramsTree, -1, nullptr);
	int rnd = Random::getSystemRandom().nextInt(100000);
	newTree.setProperty(Ids::identifier,"l_"+String(rnd), nullptr);
	newTree.setProperty(Ids::startVertex, startObject, nullptr);
	newTree.setProperty(Ids::endVertex, endObject, nullptr);
	ValueTree labelsTree(Ids::labels);
	labelsTree.setProperty(Ids::idx[0], "laebl_"+String(rnd), nullptr);
	newTree.addChild(labelsTree, -1, nullptr);

	return newTree;
}

static ValueTree createNewTouchTree(String startObject, String endObject)
{
	ValueTree newTree(Ids::touch);

//	newTree.setProperty(Ids::posX, x, nullptr);
//	newTree.setProperty(Ids::posY, y, nullptr);
	ValueTree paramsTree(Ids::parameters);
	paramsTree.setProperty(Ids::idx[0], "", nullptr);
	newTree.addChild(paramsTree, -1, nullptr);
	int rnd = Random::getSystemRandom().nextInt(100000);
	newTree.setProperty(Ids::identifier, "t_"+String(rnd), nullptr);
	newTree.setProperty(Ids::startVertex, startObject, nullptr);
	newTree.setProperty(Ids::endVertex, endObject, nullptr);
	ValueTree labelsTree(Ids::labels);
	labelsTree.setProperty(Ids::idx[0], "label_"+String(rnd), nullptr);
	newTree.addChild(labelsTree, -1, nullptr);

	return newTree;
}
static ValueTree createNewPluckTree(String startObject, String endObject)
{
	ValueTree newTree(Ids::pluck);

//	newTree.setProperty(Ids::posX, x, nullptr);
//	newTree.setProperty(Ids::posY, y, nullptr);
	ValueTree paramsTree(Ids::parameters);
	paramsTree.setProperty(Ids::idx[0], "", nullptr);
	newTree.addChild(paramsTree, -1, nullptr);
	int rnd = Random::getSystemRandom().nextInt(100000);
	newTree.setProperty(Ids::identifier,"p_"+String(rnd), nullptr);
	newTree.setProperty(Ids::startVertex, startObject, nullptr);
	newTree.setProperty(Ids::endVertex, endObject, nullptr);
	ValueTree labelsTree(Ids::labels);
	labelsTree.setProperty(Ids::idx[0], "label_"+String(rnd), nullptr);
	newTree.addChild(labelsTree, -1, nullptr);

	return newTree;
}
static ValueTree createNewAudioOutTree(int x, int y)
{
	ValueTree newTree(Ids::audioout);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
	int rnd = Random::getSystemRandom().nextInt(100000);
	newTree.setProperty(Ids::identifier,"a_"+String(rnd), nullptr);
	newTree.setProperty(Ids::sources, "", nullptr);

	return newTree;
}


ValueTree createNewObjectTree(const Identifier& objType, int x, int y)
{
	if(objType == Ids::mass)
		return createNewMassTree(x, y);
	else if(objType == Ids::port)
		return createNewPortTree(x, y);
	else if(objType == Ids::ground)
		return createNewGroundTree(x, y);
	else if(objType == Ids::resonator)
		return createNewResonatorTree(x, y);
	else if(objType == Ids::audioout)
		return createNewAudioOutTree(x, y);
	else
		return ValueTree::invalid;
}

ValueTree createNewLinkObjectTree(const Identifier& linkType, 
                                  String startObject, 
                                  String endObject)
{
    if(linkType == Ids::link)
        return createNewLinkTree(startObject, endObject);
	else if(linkType == Ids::touch)
		return createNewTouchTree(startObject, endObject);
	else if(linkType == Ids::pluck)
		return createNewPluckTree(startObject, endObject);
    else
		return ValueTree::invalid;
}

ObjectComponent* createNewObjectComponentFromTree(ObjController& owner, ValueTree objTree)
{
    ObjectComponent* const objComp = new ObjectComponent(owner, 
                                                         objTree);
    
    owner.addComponent(objComp);
    return objComp;
}
LinkComponent* createNewLinkComponentFromTree(ObjController& owner, ValueTree linkTree)
{
    LinkComponent* const linkComp = new LinkComponent(owner, linkTree);
    
    owner.addLinkComponent(linkComp);
//    linkComp->setData(linkTree);
    return linkComp;
}
//BaseObjectComponent* createObjectComponent(const Identifier& objType, int x, int y)
//{
//}

}

