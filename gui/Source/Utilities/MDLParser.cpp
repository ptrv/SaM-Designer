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

#include <vector>

#include "../Application/CommonHeaders.h"
#include "../Models/MDLFile.h"
#include "../Models/ObjectFactory.h"

#include "MDLParser.h"

#include "RegularExpression.h"
#include "../Utilities/SAMRegex.h"

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
			p.setXY(posArray[0].getIntValue(), posArray[1].getIntValue());
	}
	return p;
}

bool MDLParser::parseMDL()
{
    const File& in = mdlFile.getFile();
	String mdlContent = in.loadFileAsString();

	ValueTree mdlTree(Objects::MDLROOT);// = mdlFile.mdlRoot;

	mdlTree.setProperty(Ids::mdlName, in.getFileName(), nullptr);
	mdlTree.setProperty(Ids::mdlPath, in.getFullPathName(), nullptr);

	StringArray lines;
	lines.addTokens(mdlContent, "\n", "\"");
	for (int i = 0; i < lines.size(); ++i) {
		String line = lines[i];

        RegularExpression re;
        RegularExpression reComment("\\A\\s*#");
        RegularExpression reParams(SAMRegex::paramsDetail);
        RegularExpression reLabel(SAMRegex::word);

        if(reComment.partialMatch(line) || line.isEmpty())
            continue;

        if(re.fullMatch(SAMRegex::getVertexLine(), line))
        {

            StringArray values;
            re.fullMatchValues(line, values, 5);

            ValueTree newTree;
            if (values[0].compare("mass") == 0)
            {
                newTree = ValueTree(Ids::mass);
            }
            else if (values[0].compare("port") == 0)
            {
                newTree = ValueTree(Ids::port);
            }
            else if (values[0].compare("ground") == 0)
            {
                newTree = ValueTree(Ids::ground);
            }
            else if (values[0].compare("resonator") == 0)
            {
                newTree = ValueTree(Ids::resonator);
            }
            else
            {
                DBG("Something went really wrong!");
                return false;
            }

            Point<int> pos = getPos(line);
            newTree.setProperty(Ids::posX, pos.getX(), nullptr);
            newTree.setProperty(Ids::posY, pos.getY(), nullptr);


            if(newTree.getType() != Ids::port && newTree.getType() != Ids::ground)
            {
                String params = values[1];
                StringArray paramsArray;

                reParams.fullMatchValues(SAMRegex::getParamsLine(3),
                                         params, paramsArray, 3);
                newTree.addChild(ObjectFactory::createParamsTree(paramsArray),
                                 -1, nullptr);
            }
            if(newTree.getType() == Ids::ground)
            {
                ValueTree paramsTree(Ids::parameters);
                ValueTree param(Ids::parameter);
                param.setProperty(Ids::value, values[1].trim(), nullptr);
                paramsTree.addChild(param, -1, nullptr);
                newTree.addChild(paramsTree, -1, nullptr);
            }

            newTree.setProperty(Ids::identifier, values[2].trim(), nullptr);

            StringArray labelsArray;
            reLabel.findAndConsume(values[3], labelsArray);
            newTree.addChild(ObjectFactory::createLabelsTree(labelsArray),
                             -1, nullptr);

            ValueTree masses = mdlTree.getOrCreateChildWithName(Objects::masses, nullptr);
            masses.addChild(newTree, -1, nullptr);


        }
        else if(re.fullMatch(SAMRegex::getLinkLine(), line))
        {
            StringArray values;
            re.fullMatchValues(line, values, 6);

            ValueTree linkTree;
            if (values[0].compare("link") == 0)
            {
                linkTree = ValueTree(Ids::link);
            }
            else if (values[0].compare("touch") == 0)
            {
                linkTree = ValueTree(Ids::touch);
            }
            else if (values[0].compare("pluck") == 0)
            {
                linkTree = ValueTree(Ids::pluck);
            }
            else
            {
                DBG("Something went really wrong!");
                return false;
            }
            String params = values[1];
            StringArray paramsArray;

            int numParams = 3;
            if(linkTree.getType() == Ids::pluck)
                numParams = 4;
            reParams.fullMatchValues(SAMRegex::getParamsLine(numParams),
                                     params, paramsArray, numParams);
            linkTree.addChild(ObjectFactory::createParamsTree(paramsArray),
                              -1, nullptr);

            linkTree.setProperty(Ids::identifier, values[2].trim(), nullptr);
            linkTree.setProperty(Ids::startVertex, values[3].trim(), nullptr);
            linkTree.setProperty(Ids::endVertex, values[4].trim(), nullptr);

            StringArray labelsArray;

            reLabel.findAndConsume(values[5], labelsArray);
            linkTree.addChild(ObjectFactory::createLabelsTree(labelsArray),
                              -1, nullptr);

            ValueTree linksTree = mdlTree.getOrCreateChildWithName(Objects::links, nullptr);
            linksTree.addChild(linkTree, -1, nullptr);
        }
        else if(re.fullMatch(SAMRegex::getFaustLine(), line))
        {
            StringArray values;
            re.fullMatchValues(line, values, 3);
            
            ValueTree variableTree(Ids::variable);
            variableTree.setProperty(Ids::identifier, values[1].trim(), nullptr);
            variableTree.setProperty(Ids::faustCode, values[2].trim(), nullptr);
            ValueTree variablesTree = mdlTree.getOrCreateChildWithName(Objects::variables, nullptr);
            variablesTree.addChild(variableTree, -1, nullptr);
        }
        else if(re.fullMatch(SAMRegex::getAudioOutLine(), line))
        {
            StringArray values;
            re.fullMatchValues(line, values, 4);

            Point<int> pos = getPos(line);
            ValueTree audioTree(Ids::audioout);
            audioTree.setProperty(Ids::posX, pos.x, nullptr);
            audioTree.setProperty(Ids::posY, pos.y, nullptr);
            audioTree.setProperty(Ids::identifier, values[1].trim(), nullptr);

            // split everything from line starting with first colon
            int posColon = values[2].indexOf(":");
            String audioLine;
            if(posColon > 0)
            {
                audioLine = values[2].substring(0, posColon);
                audioTree.setProperty(Ids::optional,values[2].substring(posColon), nullptr);
            }
            else
            {
                audioTree.setProperty(Ids::optional, "", nullptr);
                audioLine = values[2];
            }

            // remove surrounding paranthese if there are some.
            RegularExpression rePar("\\(?(.+)\\)?|(.+)");
            StringArray audioLineClean;
            rePar.fullMatchValues(audioLine, audioLineClean, 1);
            
            StringArray audioOutSourcesList;
            audioOutSourcesList.addTokens(audioLineClean[0], "+", "\"");
            ValueTree audioSources(Ids::sources);
            for (int l = 0; l < audioOutSourcesList.size(); ++l)
            {
                ValueTree aoSource(Ids::audiosource);
                aoSource.setProperty(Ids::value, audioOutSourcesList[l].trim(), nullptr);
                audioSources.addChild(aoSource, -1, nullptr);
            }
            audioTree.addChild(audioSources, -1, nullptr);
            ValueTree audioObjectsTree = mdlTree.getOrCreateChildWithName(Objects::audioobjects, nullptr);
            audioObjectsTree.addChild(audioTree, -1, nullptr);

        }
        else if(re.fullMatch(SAMRegex::getWaveguideLine(), line))
        {
            StringArray values;
            re.fullMatchValues(line, values, 6);

            ValueTree waveguideTree(Ids::waveguide);

            String params = values[1];
            StringArray paramsArray;

            reParams.fullMatchValues(SAMRegex::getParamsLine(2),
                                     params, paramsArray, 2);
            waveguideTree.addChild(ObjectFactory::createParamsTree(paramsArray),
                                   -1, nullptr);

            waveguideTree.setProperty(Ids::identifier, values[2].trim(), nullptr);
            waveguideTree.setProperty(Ids::startVertex, values[3].trim(), nullptr);
            waveguideTree.setProperty(Ids::endVertex, values[4].trim(), nullptr);

            StringArray labelsArray;

            reLabel.findAndConsume(values[5], labelsArray);
            waveguideTree.addChild(ObjectFactory::createLabelsTree(labelsArray),
                              -1, nullptr);

			ValueTree wavesTree = mdlTree.getOrCreateChildWithName(Objects::waveguides, nullptr);
            wavesTree.addChild(waveguideTree, -1, nullptr);

        }
        else if(re.fullMatch(SAMRegex::getTerminationLine(), line))
        {
            StringArray values;
            re.fullMatchValues(line, values, 5);

            Point<int> pos = getPos(line);

            ValueTree terminationTree(Ids::termination);
            terminationTree.setProperty(Ids::posX, pos.x, nullptr);
            terminationTree.setProperty(Ids::posY, pos.y, nullptr);

            String params = values[1];
            StringArray paramsArray;

            reParams.fullMatchValues(SAMRegex::getParamsLine(1),
                                     params, paramsArray, 1);
            terminationTree.addChild(ObjectFactory::createParamsTree(paramsArray),
                                     -1, nullptr);

            terminationTree.setProperty(Ids::identifier, values[2].trim(), nullptr);

            StringArray labelsArray;
            reLabel.findAndConsume(values[3], labelsArray);
            terminationTree.addChild(ObjectFactory::createLabelsTree(labelsArray),
                                     -1, nullptr);

            ValueTree terminations = mdlTree.getOrCreateChildWithName(Objects::terminations, nullptr);
            terminations.addChild(terminationTree, -1, nullptr);

        }
        else if(re.fullMatch(SAMRegex::getJunctionLine(), line))
        {
            StringArray values;
            re.fullMatchValues(line, values, 5);

            Point<int> pos = getPos(line);

            ValueTree junctTree(Ids::junction);
            junctTree.setProperty(Ids::posX, pos.x, nullptr);
            junctTree.setProperty(Ids::posY, pos.y, nullptr);

            ValueTree junctParams(Ids::parameters);
            ValueTree junctParam(Ids::parameter);
            junctParam.setProperty(Ids::value, values[1].trim(), nullptr);
            junctParams.addChild(junctParam, -1, nullptr);
            junctTree.addChild(junctParams, -1, nullptr);

            junctTree.setProperty(Ids::identifier, values[2].trim(), nullptr);

            StringArray labelsArray;
            reLabel.findAndConsume(values[3], labelsArray);
            junctTree.addChild(ObjectFactory::createLabelsTree(labelsArray),
                               -1, nullptr);
            
            ValueTree junctsTree = mdlTree.getOrCreateChildWithName(Objects::junctions, nullptr);
            junctsTree.addChild(junctTree, -1, nullptr);
        }
    }
    mdlFile.mdlRoot = mdlTree;
	return true;
}

//==============================================================================
#if UNIT_TESTS

#include "../../Testsuite/MDLParser_test.h"

#endif