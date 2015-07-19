/*
  ==============================================================================

    MDLParser.cpp
    Created: 16 Apr 2012 1:04:09am
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

#include "MDLParser.h"

#include "Application/CommonHeaders.h"

#include "Models/MDLFile.h"
#include "Models/ObjectFactory.h"

#include "RegularExpression.h"
#include "SAMRegex.h"
#include "ObjectsHelper.h"

#include "MDLHelper.h"

#include <vector>


using namespace synthamodeler;

MDLParser::MDLParser(MDLFile& mdlFile_)
: mdlFile(mdlFile_)
{

}
static Point<int> getPos(const String& posStr)
{
	Point<int> p(0,0);
	int pp = posStr.lastIndexOf("# pos");
	if(pp != -1)
	{
		String tmp = posStr.substring(pp+5);
		tmp = tmp.trim();
		StringArray posArray;
		posArray.addTokens(tmp, ",", "\"");
		if(posArray.size() == 2)
        {
			p.setXY(posArray[0].getIntValue(), posArray[1].getIntValue());
            if(p.getX() < 0)
                p.setX(0);
            if(p.getY() < 0)
                p.setY(0);
        }
	}
	return p;
}

bool MDLParser::parseMDL(const File& f)
{
    RegularExpression re;
    RegularExpression reComment("\\A\\s*#[^#].*$");
    
    const File& in = f;//mdlFile.getFile();
	String mdlContent = in.loadFileAsString();

	ValueTree mdlTree(Objects::synthamodeler);// = mdlFile.mdlRoot;

	mdlTree.setProperty(Ids::mdlName, in.getFileName(), nullptr);
	mdlTree.setProperty(Ids::mdlPath, in.getFullPathName(), nullptr);

	StringArray lines;
	lines.addTokens(mdlContent, "\n", "\"");
	for (int i = 0; i < lines.size(); ++i)
    {
        String line = lines[i];

        if(reComment.fullMatch(line) || line.isEmpty())
        {
            continue;
        }

        try
        {
            if(re.fullMatch(SAMRegex::getVertexLine(), line))
            {
                addTree(mdlTree, getMassTree(line, re));
            }
            else if(re.fullMatch(SAMRegex::getLinkLine(), line))
            {
                addTree(mdlTree, getLinkTree(line, re));
            }
            else if(re.fullMatch(SAMRegex::getFaustCodeLine(), line))
            {
                addTree(mdlTree, getFaustCodeTree(line, re));
            }
            else if(re.fullMatch(SAMRegex::getAudioOutLine(), line))
            {
                addTree(mdlTree, getAudioOutTree(line, re));
            }
            else if(re.fullMatch(SAMRegex::getWaveguideLine(), line))
            {
                addTree(mdlTree, getWaveguideTree(line, re));
            }
            else if(re.fullMatch(SAMRegex::getTerminationLine(), line))
            {
                addTree(mdlTree, getTerminationTree(line, re));
            }
            else if(re.fullMatch(SAMRegex::getJunctionLine(), line))
            {
                addTree(mdlTree, getJunctionTree(line, re));
            }
            else if(re.fullMatch(SAMRegex::getCommentObjectLine(), line))
            {
                addTree(mdlTree, getCommentTree(line, re));
            }
        }
        catch (const std::exception& e)
        {
            DBG("Something went really wrong! "+String(e.what()));
        }
    }
//    DBG(mdlTree.toXmlString());
    mdlFile.mdlRoot = mdlTree;
	return true;
}

//------------------------------------------------------------------------------

void MDLParser::addTree(ValueTree& rootTree, const ValueTree& newTree)
{
    const Identifier& groupId = ObjectsHelper::getObjectGroup(newTree.getType());
    ValueTree group = rootTree.getOrCreateChildWithName(groupId, nullptr);
    group.addChild(newTree, -1, nullptr);
}

//------------------------------------------------------------------------------
//values: mass-type, parameters, identifier
ValueTree MDLParser::getMassTree(const String& line, RegularExpression& re)
{
    StringArray values;
    re.fullMatchValues(line, values, 3);

    ValueTree newTree = ObjectFactory::getMassTreeFromStringId(values[0]);

    if (!newTree.isValid())
    {
        throw std::runtime_error("Cannot parse mass-like object");
    }

    const Point<int> pos = getPos(line);
    newTree.setProperty(Ids::posX, pos.getX(), nullptr);
    newTree.setProperty(Ids::posY, pos.getY(), nullptr);


    if (newTree.getType() != Ids::port && newTree.getType() != Ids::ground)
    {
        StringArray paramsArray = MDLHelper::getParamsFromString(values[1]);

        if (newTree.getType() == Ids::resonators)
        {
            static const int NUM_RES_PARAMS = 3;
            if(paramsArray.size() % NUM_RES_PARAMS == 0)
            {
                ValueTree paramsTree(Ids::parameters);
                Array<ValueTree> valuesArr;
                for (int np = 0; np < NUM_RES_PARAMS; ++np)
                {
                    valuesArr.add(ValueTree(Ids::parameter));
                }

                for (int n = 0; n < paramsArray.size(); ++n)
                {
                    const int paramIdx = n % NUM_RES_PARAMS;
                    ValueTree subVal(ObjectsHelper::getResonatorParamsIds()[paramIdx]);
                    subVal.setProperty(Ids::value, paramsArray[n].trim(), nullptr);

                    valuesArr[paramIdx].addChild(subVal, -1, nullptr);
                }

                for (int np = 0; np < NUM_RES_PARAMS; ++np)
                {
                    paramsTree.addChild(valuesArr[np], -1, nullptr);
                }

                newTree.addChild(paramsTree, -1, nullptr);
            }
        }
        else
        {
            newTree.addChild(ObjectFactory::createParamsTree(paramsArray),
                             -1, nullptr);
        }
    }
    else if (newTree.getType() == Ids::ground)
    {
        ValueTree paramsTree(Ids::parameters);
        ValueTree param(Ids::parameter);
        param.setProperty(Ids::value, values[1].trim(), nullptr);
        paramsTree.addChild(param, -1, nullptr);
        newTree.addChild(paramsTree, -1, nullptr);
    }

    newTree.setProperty(Ids::identifier, values[2].trim(), nullptr);

    return newTree;
}

//------------------------------------------------------------------------------
// values: link-type, parameters, identifier, startVertex, endVertex
ValueTree MDLParser::getLinkTree(const String& line, RegularExpression& re)
{
    StringArray values;
    re.fullMatchValues(line, values, 5);

    ValueTree linkTree = ObjectFactory::getLinkTreeFromStringId(values[0]);

    if (!linkTree.isValid())
    {
        throw std::runtime_error("Could not parse link-like object");
    }

    StringArray paramsArray = MDLHelper::getParamsFromString(values[1]);

    linkTree.addChild(ObjectFactory::createParamsTree(paramsArray),
                      -1, nullptr);

    linkTree.setProperty(Ids::identifier, values[2].trim(), nullptr);
    linkTree.setProperty(Ids::startVertex, values[3].trim(), nullptr);
    linkTree.setProperty(Ids::endVertex, values[4].trim(), nullptr);

    return linkTree;
}

//------------------------------------------------------------------------------
// values: faustcode
ValueTree MDLParser::getFaustCodeTree(const String& line, RegularExpression& re)
{
    StringArray values;
    re.fullMatchValues(line, values, 1);

    ValueTree faustcodeTree(Ids::faustcode);
    faustcodeTree.setProperty(Ids::value, values[0].trim(), nullptr);

    return faustcodeTree;
}

//------------------------------------------------------------------------------
// values: identifier, sources
ValueTree MDLParser::getAudioOutTree(const String& line, RegularExpression& re)
{
    StringArray values;
    re.fullMatchValues(line, values, 2);

    const Point<int> pos = getPos(line);

    ValueTree audioTree(Ids::audioout);
    audioTree.setProperty(Ids::posX, pos.x, nullptr);
    audioTree.setProperty(Ids::posY, pos.y, nullptr);
    audioTree.setProperty(Ids::identifier, values[0].trim(), nullptr);

    // split everything from line starting with first colon
    int posColon = values[1].indexOf(":");
    String audioLine;
    if(posColon > 0)
    {
        audioLine = values[1].substring(0, posColon);
        audioLine = MDLHelper::removeUnbalancedParentheses(audioLine);
        audioTree.setProperty(Ids::optional,values[1].substring(posColon+1), nullptr);
    }
    else
    {
        audioTree.setProperty(Ids::optional, "", nullptr);
        audioLine = values[1];
    }

    // add outputDSP to optional values if not present
    if(! audioTree[Ids::optional].toString().contains("outputDSP"))
    {
        String aoOpt = audioTree[Ids::optional].toString();
        if(aoOpt.isNotEmpty())
        {
            aoOpt = ":" + aoOpt;
        }
        String aoOpt2 = "outputDSP" + aoOpt;
        audioTree.setProperty(Ids::optional, aoOpt2, nullptr);
    }

    // remove unbalanced parentheses
    audioLine = MDLHelper::removeUnbalancedParentheses(audioLine);

    // remove surrounding paranthese if there are some.
    String audioLineClean = MDLHelper::removeSurroundingParentheses(audioLine);

    StringArray audioOutSourcesList;
    audioOutSourcesList.addTokens(audioLineClean, "+", "\"");

    ValueTree audioSources(Ids::sources);
    for (int l = 0; l < audioOutSourcesList.size(); ++l)
    {
        if(audioOutSourcesList[l].trim().compare("0.0") != 0)
        {
            ValueTree aoSource(Ids::audiosource);
            aoSource.setProperty(Ids::value, audioOutSourcesList[l].trim(), nullptr);
            audioSources.addChild(aoSource, -1, nullptr);
        }
    }
    audioTree.addChild(audioSources, -1, nullptr);

    return audioTree;
}

//------------------------------------------------------------------------------
// values: param1, param2, identifier, startVertex, endVertex
ValueTree MDLParser::getWaveguideTree(const String& line, RegularExpression& re)
{
    StringArray values;
    re.fullMatchValues(line, values, 5);

    ValueTree waveguideTree(Ids::waveguide);

    StringArray paramsArray(values.begin(), 2);

    waveguideTree.addChild(ObjectFactory::createParamsTree(paramsArray),
                           -1, nullptr);

    waveguideTree.setProperty(Ids::identifier, values[2].trim(), nullptr);
    waveguideTree.setProperty(Ids::startVertex, values[3].trim(), nullptr);
    waveguideTree.setProperty(Ids::endVertex, values[4].trim(), nullptr);

    return waveguideTree;
}

//------------------------------------------------------------------------------
// values: param, identifier
ValueTree MDLParser::getTerminationTree(const String& line, RegularExpression& re)
{
    StringArray values;
    re.fullMatchValues(line, values, 2);

    const Point<int> pos = getPos(line);

    ValueTree terminationTree(Ids::termination);
    terminationTree.setProperty(Ids::posX, pos.x, nullptr);
    terminationTree.setProperty(Ids::posY, pos.y, nullptr);

    StringArray paramsArray = MDLHelper::getParamsFromString(values[0]);

    terminationTree.addChild(ObjectFactory::createParamsTree(paramsArray),
                             -1, nullptr);

    terminationTree.setProperty(Ids::identifier, values[1].trim(), nullptr);

    return terminationTree;
}

//------------------------------------------------------------------------------
// values: param1, identifier
ValueTree MDLParser::getJunctionTree(const String& line, RegularExpression& re)
{
    StringArray values;
    re.fullMatchValues(line, values, 2);

    const Point<int> pos = getPos(line);

    ValueTree junctTree(Ids::junction);
    junctTree.setProperty(Ids::posX, pos.x, nullptr);
    junctTree.setProperty(Ids::posY, pos.y, nullptr);

    ValueTree junctParams(Ids::parameters);
    ValueTree junctParam(Ids::parameter);
    junctParam.setProperty(Ids::value, values[0].trim(), nullptr);
    junctParams.addChild(junctParam, -1, nullptr);
    junctTree.addChild(junctParams, -1, nullptr);

    junctTree.setProperty(Ids::identifier, values[1].trim(), nullptr);

    return junctTree;
}

//------------------------------------------------------------------------------
// values: param, identifier
ValueTree MDLParser::getCommentTree(const String& line, RegularExpression& re)
{
    StringArray values;
    re.fullMatchValues(line, values, 2);

    const Point<int> pos = getPos(line);

    ValueTree newTree(Ids::comment);
    newTree.setProperty(Ids::posX, pos.getX(), nullptr);
    newTree.setProperty(Ids::posY, pos.getY(), nullptr);

    newTree.setProperty(Ids::identifier, values[1], nullptr);

    StringArray paramsArray = MDLHelper::getParamsFromString(values[0]);

    StringArray commVal;
    commVal.addTokens(paramsArray[0].unquoted(), "|" , "\"");
    newTree.setProperty(Ids::value, commVal.joinIntoString("\n"), nullptr);
    newTree.setProperty(Ids::fontSize, paramsArray[1], nullptr);
    newTree.setProperty(Ids::commentColour, paramsArray[2], nullptr);

    return newTree;
}

//------------------------------------------------------------------------------

bool MDLParser::parseMDLX(const File& f, bool onlyExtras)
{
    XmlElement* xml = XmlDocument::parse(f);
    if (xml != nullptr)
    {
        ValueTree mdlxTree = ValueTree::fromXml(*xml);
        if (onlyExtras)
        {
            ValueTree comments = mdlxTree.getChildWithName(Objects::comments);
            if(comments.isValid())
            {
                mdlxTree.removeChild(comments, nullptr);
                ValueTree tmp1 = mdlxTree.createCopy();
                ValueTree tmp2 = mdlFile.mdlRoot.createCopy();
                tmp1.setProperty(Ids::mdlName, "", nullptr);
                tmp1.setProperty(Ids::mdlPath, "", nullptr);
                tmp2.setProperty(Ids::mdlName, "", nullptr);
                tmp2.setProperty(Ids::mdlPath, "", nullptr);
                if(tmp1.isEquivalentTo(tmp2))
                    mdlFile.mdlRoot.addChild(comments, -1, nullptr);
            }
        }
        else
        {
            mdlFile.mdlRoot = mdlxTree.createCopy();
        }
        delete xml;
        return true;
    }
    return false;
}

//==============================================================================
#if UNIT_TESTS

#include "MDLParser_test.h"

#endif
