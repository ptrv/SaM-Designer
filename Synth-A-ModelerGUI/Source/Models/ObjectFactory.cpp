/*
  ==============================================================================

    ObjectFactory.cpp
    Created: 6 May 2012 11:40:40pm
    Author:  peter

  ==============================================================================
*/

#include "ObjectFactory.h"

#include "ObjectIDs.h"
namespace ObjectFactory
{

static ValueTree createNewMassTree(int x, int y)
{
	ValueTree newTree(Ids::mass);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
	ValueTree paramsTree(Ids::parameters);
	paramsTree.setProperty(Ids::idx[0], 1.0f, nullptr);
	paramsTree.setProperty(Ids::idx[1], 0.0f, nullptr);
	paramsTree.setProperty(Ids::idx[2], 0.0f, nullptr);
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

static ValueTree createNewLinkTree(int x, int y)
{
	ValueTree newTree(Ids::link);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
	ValueTree paramsTree(Ids::parameters);
	paramsTree.setProperty(Ids::idx[0], "", nullptr);
	newTree.addChild(paramsTree, -1, nullptr);
	int rnd = Random::getSystemRandom().nextInt(100000);
	newTree.setProperty(Ids::identifier,"l_"+String(rnd), nullptr);
	newTree.setProperty(Ids::startVertex, "", nullptr);
	newTree.setProperty(Ids::endVertex, "", nullptr);
	ValueTree labelsTree(Ids::labels);
	labelsTree.setProperty(Ids::idx[0], "laebl_"+String(rnd), nullptr);
	newTree.addChild(labelsTree, -1, nullptr);

	return newTree;
}

static ValueTree createNewTouchTree(int x, int y)
{
	ValueTree newTree(Ids::touch);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
	ValueTree paramsTree(Ids::parameters);
	paramsTree.setProperty(Ids::idx[0], "", nullptr);
	newTree.addChild(paramsTree, -1, nullptr);
	int rnd = Random::getSystemRandom().nextInt(100000);
	newTree.setProperty(Ids::identifier, "t_"+String(rnd), nullptr);
	newTree.setProperty(Ids::startVertex, "", nullptr);
	newTree.setProperty(Ids::endVertex, "", nullptr);
	ValueTree labelsTree(Ids::labels);
	labelsTree.setProperty(Ids::idx[0], "label_"+String(rnd), nullptr);
	newTree.addChild(labelsTree, -1, nullptr);

	return newTree;
}
static ValueTree createNewPluckTree(int x, int y)
{
	ValueTree newTree(Ids::pluck);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
	ValueTree paramsTree(Ids::parameters);
	paramsTree.setProperty(Ids::idx[0], "", nullptr);
	newTree.addChild(paramsTree, -1, nullptr);
	int rnd = Random::getSystemRandom().nextInt(100000);
	newTree.setProperty(Ids::identifier,"p_"+String(rnd), nullptr);
	newTree.setProperty(Ids::startVertex, "", nullptr);
	newTree.setProperty(Ids::endVertex, "", nullptr);
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
	else if(objType == Ids::link)
		return createNewLinkTree(x, y);
	else if(objType == Ids::touch)
		return createNewTouchTree(x, y);
	else if(objType == Ids::pluck)
		return createNewPluckTree(x, y);
	else if(objType == Ids::audioout)
		return createNewAudioOutTree(x, y);
	else
		return ValueTree::invalid;
}

}

