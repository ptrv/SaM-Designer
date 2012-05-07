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

#include "../../JuceLibraryCode/JuceHeader.h"
#include "MDLParser.h"
#include "../Models/ObjectIDs.h"

MDLParser::MDLParser(MDLFile& mdlFile_)
: mdlFile(mdlFile_)
{

}
static Point<int> getPos(const String& posStr)
{
	Point<int> p(0,0);
	int pp = posStr.indexOf("# pos");
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

	StringArray lines;
	lines.addTokens(mdlContent, "\n", "\"");
	for (int i = 0; i < lines.size(); ++i) {
		String line = lines[i];

		line = line.trimCharactersAtStart(" ");

		if(line[0] != '#')
		{
			if(line.startsWith("mass")
					|| line.startsWith("port")
					|| line.startsWith("resonator")
					|| line.startsWith("ground"))
			{
				int indexParantese = line.indexOf("(");
				String objType = line.substring(0, indexParantese);
				if(objType.compare("mass") == 0 ||
						objType.compare("port") == 0 ||
						objType.compare("resonator") == 0 ||
						objType.compare("ground") == 0)
				{
					ValueTree newTree;
					if(objType.compare("mass") == 0)
					{
						newTree = ValueTree(Ids::mass);
					}
					else if(objType.compare("port") == 0)
					{
						newTree = ValueTree(Ids::port);
					}
					else if(objType.compare("ground") == 0)
					{
						newTree = ValueTree(Ids::ground);
					}
					else if(objType.compare("resonator") == 0)
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

					//get values between first parantheses
					int indexCloseParan = line.indexOf(")");

					if(newTree.getType() != Ids::port)
//					if(mass->getType() != PortType)
					{
						String params = line.substring(indexParantese+1, indexCloseParan);
						StringArray paramsArray;
						paramsArray.addTokens(params, ",", "\"");
						ValueTree paramsTree(Ids::parameters);
						for (int param = 0; param < paramsArray.size(); ++param) {
							float paramVal = paramsArray[param].trimCharactersAtStart(" ").getFloatValue();
							paramsTree.setProperty(Ids::idx[param], paramVal, nullptr);
						}
						newTree.addChild(paramsTree, -1, nullptr);
					}
					// get remaining line content
					line = line.substring(indexCloseParan+1);
					line = line.trimCharactersAtStart(",");

					// get string till next comma
					int commaIndex = line.indexOf(",");
					if(commaIndex != -1)
					{
						newTree.setProperty(Ids::identifier, line.substring(0, commaIndex), nullptr);
					}

					line = line.substring(commaIndex);

					indexParantese = line.indexOf("(");
					indexCloseParan = line.indexOf(")");
					String labels = line.substring(indexParantese+1, indexCloseParan);
					StringArray labelsArray;
					labelsArray.addTokens(labels, ",", "\"");
					ValueTree labelsTree(Ids::labels);
					for (int l = 0; l < labelsArray.size(); ++l) {
						labelsTree.setProperty(Ids::idx[l], labelsArray[l], nullptr);
					}
					newTree.addChild(labelsTree, -1, nullptr);

					ValueTree masses = mdlTree.getOrCreateChildWithName(Objects::masses, nullptr);
					masses.addChild(newTree, -1, nullptr);
				}
			}
			else if(line.startsWith("link")
					|| line.startsWith("touch")
					|| line.startsWith("pluck"))
			{
				int indexParantese = line.indexOf("(");
				String objType = line.substring(0, indexParantese);
				if(objType.compare("link") == 0 ||
						objType.compare("touch") == 0 ||
						objType.compare("pluck") == 0)
				{
					ValueTree linkTree;
					if(objType.compare("link") == 0)
					{
						linkTree = ValueTree(Ids::link);
					}
					else if(objType.compare("touch") == 0)
					{
						linkTree = ValueTree(Ids::touch);
					}
					else if(objType.compare("pluck") == 0)
					{
						linkTree = ValueTree(Ids::pluck);
					}
					else
					{
						DBG("Something went really wrong!");
						return false;
					}

					Point<int> pos = getPos(line);
					linkTree.setProperty(Ids::posX, pos.x, nullptr);
					linkTree.setProperty(Ids::posY, pos.y, nullptr);

					//get values between first parantheses
					int indexCloseParan = line.indexOf(")");
					String params = line.substring(indexParantese+1, indexCloseParan);
					StringArray paramsArray;
					paramsArray.addTokens(params, ",", "\"");
					ValueTree paramsTree(Ids::parameters);
					for (int param = 0; param < paramsArray.size(); ++param) {
						String paramVal = paramsArray[param].trimCharactersAtStart(" ");
						paramsTree.setProperty(Ids::idx[param], paramVal, nullptr);
					}
					linkTree.addChild(paramsTree, -1, nullptr);

					// get remaining line content
					line = line.substring(indexCloseParan+1);
					line = line.trimCharactersAtStart(",");

					// get string till next opening paranthese
					indexParantese = line.indexOf("(");

					String params2 = line.substring(0,indexParantese);
					StringArray paramsArray2;
					paramsArray2.addTokens(params2, ",", "\"");
					if(paramsArray2.size() >= 3)
					{
						linkTree.setProperty(Ids::identifier, paramsArray2[0], nullptr);
						linkTree.setProperty(Ids::startVertex, paramsArray2[1], nullptr);
						linkTree.setProperty(Ids::endVertex, paramsArray2[2], nullptr);
					}
					line = line.substring(indexParantese);
					indexCloseParan = line.indexOf(")");
					String labels = line.substring(1, indexCloseParan);
					StringArray labelsArray;
					labelsArray.addTokens(labels, ",", "\"");
					ValueTree labelsTree(Ids::labels);
					for (int l = 0; l < labelsArray.size(); ++l) {
						labelsTree.setProperty(Ids::idx[l], labelsArray[l], nullptr);
					}
					linkTree.addChild(labelsTree, -1, nullptr);

					ValueTree linksTree = mdlTree.getOrCreateChildWithName(Objects::links, nullptr);
					linksTree.addChild(linkTree, -1, nullptr);
				}
			}
			else if(line.startsWith("faustcode:"))
			{
				StringArray labelAttributeList;
				int indexSemicolon = line.indexOf(";");
				int indexColon = line.indexOf(":");
				String lineTmp = line.substring(indexColon+1, indexSemicolon);
				lineTmp = lineTmp.trimCharactersAtStart(" ");
				int indexEquals = lineTmp.indexOf("=");
				ValueTree labelTree(Ids::label);
				labelTree.setProperty(Ids::identifier, lineTmp.substring(0,indexEquals), nullptr);
				labelTree.setProperty(Ids::faustCode, lineTmp.substring(indexEquals+1), nullptr);
				ValueTree labelsTree = mdlTree.getOrCreateChildWithName(Objects::labels, nullptr);
				labelsTree.addChild(labelTree, -1, nullptr);
			}
			else if(line.startsWith("audioout"))
			{
				StringArray audioOutAttributeList;

				int indexSemicolon = line.length()-1;
				String lineTmp = line.substring(0, indexSemicolon);
				audioOutAttributeList.addTokens(lineTmp, ",", "\"");
				if(audioOutAttributeList.size() > 2)
				{
					Point<int> pos = getPos(line);

					ValueTree audioTree(Ids::audioout);
					audioTree.setProperty(Ids::posX, pos.x, nullptr);
					audioTree.setProperty(Ids::posY, pos.y, nullptr);
					audioTree.setProperty(Ids::identifier, audioOutAttributeList[1], nullptr);
					audioTree.setProperty(Ids::sources, audioOutAttributeList[2], nullptr);
					ValueTree audioObjectsTree = mdlTree.getOrCreateChildWithName(Objects::audioobjects, nullptr);
					audioObjectsTree.addChild(audioTree, -1, nullptr);
				}
			}
			else if(line.startsWith("waveguide"))
			{
				int indexParantese = line.indexOf("(");

				Point<int> pos = getPos(line);

				ValueTree waveguideTree(Ids::waveguide);
				waveguideTree.setProperty(Ids::posX, pos.x, nullptr);
				waveguideTree.setProperty(Ids::posY, pos.y, nullptr);

				String objType = line.substring(0, indexParantese);
				int indexCloseParan = line.indexOf(")");
				String params = line.substring(indexParantese+1, indexCloseParan);
				StringArray paramsArray;
				paramsArray.addTokens(params, ",", "\"");
				ValueTree waveParams(Ids::parameters);
				for (int param = 0; param < paramsArray.size(); ++param) {
					float paramVal = paramsArray[param].trimCharactersAtStart(" ").getFloatValue();
					waveParams.setProperty(Ids::idx[param], paramVal, nullptr);
				}
				waveguideTree.addChild(waveParams, -1, nullptr);

				// get remaining line content
				line = line.substring(indexCloseParan+1);
				line = line.trimCharactersAtStart(",");

				// get string till next comma
				int commaIndex = line.indexOf(",");
				if(commaIndex != -1)
				{
					waveguideTree.setProperty(Ids::identifier, line.substring(0, commaIndex), nullptr);
				}

				// get left object
				line = line.substring(commaIndex+1);
				commaIndex = line.indexOf(",");
				waveguideTree.setProperty(Ids::objLeft, line.substring(0, commaIndex), nullptr);

				// get right object
				line = line.substring(commaIndex+1);
				commaIndex = line.indexOf(",");
				waveguideTree.setProperty(Ids::objRight, line.substring(0, commaIndex), nullptr);

				indexParantese = line.indexOf("(");
				indexCloseParan = line.indexOf(")");
				String labels = line.substring(indexParantese+1, indexCloseParan);
				StringArray labelsArray;
				labelsArray.addTokens(labels, ",", "\"");
				ValueTree labelTree(Ids::labels);
				for (int l = 0; l < labelsArray.size(); ++l) {
					labelTree.setProperty(Ids::idx[l], labelsArray[l], nullptr);
				}
				waveguideTree.addChild(labelTree, -1, nullptr);

				ValueTree wavesTree = mdlTree.getOrCreateChildWithName(Objects::waveguides, nullptr);
				wavesTree.addChild(waveguideTree, -1, nullptr);

			}
			else if(line.startsWith("termination"))
			{
				int indexParantese = line.indexOf("(");

				Point<int> pos = getPos(line);

				ValueTree terminationTree(Ids::termination);
				terminationTree.setProperty(Ids::posX, pos.x, nullptr);
				terminationTree.setProperty(Ids::posY, pos.y, nullptr);

				int indexCloseParan = line.indexOf(")");
				String params = line.substring(indexParantese+1, indexCloseParan+1);
				ValueTree termParams(Ids::parameters);
				termParams.setProperty(Ids::idx[0], params, nullptr);
				terminationTree.addChild(termParams, -1, nullptr);

				// get remaining line content
				line = line.substring(indexCloseParan+2);
				line = line.trimCharactersAtStart(",");

				// get string till next comma
				int commaIndex = line.indexOf(",");
				if(commaIndex != -1)
				{
					terminationTree.setProperty(Ids::identifier, line.substring(0, commaIndex), nullptr);
				}

				indexParantese = line.indexOf("(");
				indexCloseParan = line.indexOf(")");
				String labels = line.substring(indexParantese+1, indexCloseParan);
				StringArray labelsArray;
				labelsArray.addTokens(labels, ",", "\"");
				ValueTree labelTree(Ids::labels);
				for (int l = 0; l < labelsArray.size(); ++l) {
					labelTree.setProperty(Ids::idx[l], labelsArray[l], nullptr);
				}
				terminationTree.addChild(labelTree, -1, nullptr);


				ValueTree termsTree = mdlTree.getOrCreateChildWithName(Objects::terminations, nullptr);
				termsTree.addChild(terminationTree, -1, nullptr);
			}
			else if(line.startsWith("junction"))
			{
				int indexParantese = line.indexOf("(");

				Point<int> pos = getPos(line);

				ValueTree junctTree(Ids::junction);
				junctTree.setProperty(Ids::posX, pos.x, nullptr);
				junctTree.setProperty(Ids::posY, pos.y, nullptr);

				int indexCloseParan = line.indexOf(")");
				String params = line.substring(indexParantese+1, indexCloseParan);
				ValueTree junctParams(Ids::parameters);
				junctParams.setProperty(Ids::idx[0], params, nullptr);
				junctTree.addChild(junctParams, -1, nullptr);

				// get remaining line content
				line = line.substring(indexCloseParan+1);
				line = line.trimCharactersAtStart(",");

				// get string till next comma
				int commaIndex = line.indexOf(",");
				if(commaIndex != -1)
				{
					junctTree.setProperty(Ids::identifier, line.substring(0, commaIndex), nullptr);
				}

				indexParantese = line.indexOf("(");
				indexCloseParan = line.indexOf(")");
				String labels = line.substring(indexParantese+1, indexCloseParan);
				StringArray labelsArray;
				labelsArray.addTokens(labels, ",", "\"");
				ValueTree labelTree(Ids::labels);
				for (int l = 0; l < labelsArray.size(); ++l) {
					labelTree.setProperty(Ids::idx[l], labelsArray[l], nullptr);
				}
				junctTree.addChild(labelTree, -1, nullptr);

				ValueTree junctsTree = mdlTree.getOrCreateChildWithName(Objects::junctions, nullptr);
				junctsTree.addChild(junctTree, -1, nullptr);

			}
		}
	}

	mdlFile.mdlRoot = mdlTree;
	return true;
}

