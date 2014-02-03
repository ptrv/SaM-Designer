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
#include "../Graph/Node.h"
#include "../View/BaseObjectComponent.h"
#include "../View/ObjectComponent.h"
#include "../View/LinkComponent.h"
#include "ObjectFactory.h"

using namespace synthamodeler;

static ValueTree createResonatorParamsTree(StringArray p);

static ValueTree createMewMassLikeTree(const Identifier& massType,
                                       const String& newName, int x, int y,
                                       const StringPairArray& paramsStringPairs)
{
    if (massType != Ids::mass && massType != Ids::ground
        && massType != Ids::resonators && massType != Ids::termination
        && massType != Ids::junction && massType != Ids::port)
    {
        return ValueTree();
    }

    StoredSettings& settings = *StoredSettings::getInstance();
    ValueTree newTree(massType);

    newTree.setProperty(Ids::posX, x, nullptr);
    newTree.setProperty(Ids::posY, y, nullptr);

    StringArray paramsStrings;
    for (int i = 0; i < paramsStringPairs.size(); ++i)
    {
        const String& paramName = paramsStringPairs.getAllKeys()[i];
        const String& paramValue = paramsStringPairs.getAllValues()[i];

        paramsStrings.add(settings.getDefaultValue(paramName, paramValue));
    }
    if (paramsStrings.size() > 0)
    {
        ValueTree paramsTree;
        if (massType == Ids::resonators)
        {
            paramsTree = createResonatorParamsTree(paramsStrings);
        }
        else
        {
            paramsTree = ObjectFactory::createParamsTree(paramsStrings);
        }
        newTree.addChild(paramsTree, -1, nullptr);
    }
    newTree.setProperty(Ids::identifier, newName, nullptr);

    return newTree;
}

static ValueTree createNewMassTree(const String& newName, int x, int y)
{
    StringPairArray defaultValues;
    defaultValues.set("mass_kg", "0.001");
    defaultValues.set("mass_init_position", "0.0");
    defaultValues.set("mass_init_velocity", "0.0");

    return createMewMassLikeTree(Ids::mass, newName, x, y, defaultValues);
}

static ValueTree createNewPortTree(const String& newName, int x, int y)
{
    return createMewMassLikeTree(Ids::port, newName, x, y, StringPairArray());
}

static ValueTree createNewGroundTree(const String& newName, int x, int y)
{
    StringPairArray defaultValues;
    defaultValues.set("ground_init_position", "0.0");

    return createMewMassLikeTree(Ids::ground, newName, x, y, defaultValues);
}

static ValueTree createNewResonatorsTree(const String& newName, int x, int y)
{
    StringPairArray defaultValues;
    defaultValues.set("resonators_frequency", "200.0");
    defaultValues.set("resonators_decay_time", "1.5");
    defaultValues.set("resonators_eq_mass", "0.01");

    return createMewMassLikeTree(Ids::resonators, newName, x, y, defaultValues);
}

static ValueTree createNewLinkLikeTree(const Identifier& linkType,
                                   const String& newName,
                                   const String& startObject,
                                   const String& endObject,
                                       const StringPairArray& paramsStringPairs)
{
    if (linkType != Ids::link && linkType != Ids::touch
        && linkType != Ids::pluck && linkType != Ids::pulsetouch
        && linkType != Ids::waveguide)
    {
        return ValueTree();
    }

    StoredSettings& settings = *StoredSettings::getInstance();
    ValueTree newTree(linkType);

    StringArray p;
    for (int i = 0; i < paramsStringPairs.size(); ++i)
    {
        const String& valName = paramsStringPairs.getAllKeys()[i];
        const String& value = paramsStringPairs.getAllValues()[i];
        p.add(settings.getDefaultValue(valName, value));
    }

    ValueTree paramsTree = ObjectFactory::createParamsTree(p);
    newTree.addChild(paramsTree, -1, nullptr);

    newTree.setProperty(Ids::identifier, newName, nullptr);
    newTree.setProperty(Ids::startVertex, startObject, nullptr);
    newTree.setProperty(Ids::endVertex, endObject, nullptr);

    return newTree;
}

static ValueTree createNewLinkTree(const String& newName,
                                   const String& startObject,
                                   const String& endObject)
{
    StringPairArray defaultValues;
    defaultValues.set("link_stiffness", "100.0");
    defaultValues.set("link_damping", "0.1");
    defaultValues.set("link_offset", "0.0");

    return createNewLinkLikeTree(Ids::link, newName, startObject, endObject,
                                 defaultValues);
}

static ValueTree createNewTouchTree(const String& newName,
                                    const String& startObject, 
                                    const String& endObject)
{
    StringPairArray defaultValues;
    defaultValues.set("touch_stiffness", "100.0");
    defaultValues.set("touch_damping", "0.1");
    defaultValues.set("touch_offset", "0.0");

    return createNewLinkLikeTree(Ids::touch, newName, startObject, endObject,
                                 defaultValues);
}
static ValueTree createNewPulsetouchTree(const String& newName,
                                         const String& startObject,
                                         const String& endObject)
{
    StringPairArray defaultValues;
    defaultValues.set("pulsetouch_stiffness", "100.0");
    defaultValues.set("pulsetouch_damping", "0.1");
    defaultValues.set("pulsetouch_offset", "0.0");
    defaultValues.set("pulsetouch_pulsemult", "0.0");
    defaultValues.set("pulsetouch_pulsetau", "0.0");
    defaultValues.set("pulsetouch_pulselen", "0.0");

    return createNewLinkLikeTree(Ids::pulsetouch, newName, startObject, endObject,
                                 defaultValues);
}

static ValueTree createNewPluckTree(const String& newName,
                                    const String& startObject, 
                                    const String& endObject)
{
    StringPairArray defaultValues;
    defaultValues.set("pluck_stiffness", "300.0");
    defaultValues.set("pluck_damping", "0.1");
    defaultValues.set("pluck_displacement", "0.003");
    defaultValues.set("pluck_offset", "0.0");

    return createNewLinkLikeTree(Ids::pluck, newName, startObject, endObject,
                                 defaultValues);
}

static ValueTree createNewWaveguideTree(const String& newName,
                                        const String& startObject,
                                        const String& endObject)
{
    StringPairArray defaultValues;
    defaultValues.set("waveguide_impedance", "1.0");
    defaultValues.set("waveguide_string_type", "simpleString(0.033,0.017)");

    return createNewLinkLikeTree(Ids::waveguide, newName, startObject, endObject,
                                 defaultValues);
}

static ValueTree createNewTerminationTree(const String& newName, int x, int y)
{
    StringPairArray defaultValues;
    defaultValues.set("term_type", "simpleStringTerm(-0.996, 20)");

    return createMewMassLikeTree(Ids::termination, newName, x, y, defaultValues);
}

static ValueTree createNewJunctionTree(const String& newName, int x, int y)
{
    StringPairArray defaultValues;
    defaultValues.set("junct_displacement", "0.0");

    return createMewMassLikeTree(Ids::junction, newName, x, y, defaultValues);
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

static ValueTree createNewCommentTree(const String& newName, int x, int y)
{
    ValueTree newTree(Ids::comment);

	newTree.setProperty(Ids::posX, x, nullptr);
	newTree.setProperty(Ids::posY, y, nullptr);
	newTree.setProperty(Ids::identifier,newName, nullptr);
    newTree.setProperty(Ids::value, "", nullptr);
    newTree.setProperty(Ids::fontSize, 16.0f, nullptr);
    newTree.setProperty(Ids::commentColour, Colours::black.toString(), nullptr);
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
	else if(objType == Ids::resonators)
		return createNewResonatorsTree(newName, x, y);
	else if(objType == Ids::audioout)
		return createNewAudioOutTree(newName, x, y);
    else if(objType == Ids::junction)
        return createNewJunctionTree(newName, x, y);
    else if(objType == Ids::termination)
        return createNewTerminationTree(newName, x, y);
    else if(objType == Ids::comment)
        return createNewCommentTree(newName, x, y);
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
	else if(linkType == Ids::pulsetouch)
		return createNewPulsetouchTree(newName, startObject, endObject);
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

static ValueTree createResonatorParamsTree(StringArray p)
{

    ValueTree paramsTree(Ids::parameters);

    if (p.size() != 3)
    {
        for (int i = 0; i < 3; ++i)
        {
            ValueTree value(Ids::parameter);
            ValueTree subval(Utils::resonatorParamIds[i]);
            subval.setProperty(Ids::value, p[i].trim(), nullptr);
            value.addChild(subval, -1, nullptr);
            paramsTree.addChild(value, -1, nullptr);
        }
    }
    return paramsTree;
}
