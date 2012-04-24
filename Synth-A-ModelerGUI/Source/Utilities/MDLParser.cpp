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
#include "../Controller/ObjectFactory.h"

MDLParser::MDLParser(MDLFile& mdlFile_)
: mdlFile(mdlFile_)
{

}

bool MDLParser::parseMDL()
{
	File in(mdlFile.getFilePath());
	String mdlContent = in.loadFileAsString();

	int fileLength = mdlContent.length();

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
				// mass or link type
				if(indexParantese != -1)
				{
					String objType = line.substring(0, indexParantese);
					if(objType.compare("mass") == 0 ||
							objType.compare("port") == 0 ||
							objType.compare("resonator") == 0 ||
							objType.compare("ground") == 0)
					{
						MassObject* mass;
						if(objType.compare("mass") == 0)
						{
							mass = ObjectFactory::createNewMass();
						}
						else if(objType.compare("port") == 0)
						{
							mass = ObjectFactory::createNewPort();
						}
						else if(objType.compare("ground") == 0)
						{
							mass = ObjectFactory::createNewGround();
						}
						else if(objType.compare("resonator") == 0)
						{
							mass = ObjectFactory::createNewResonator();
						}
						else
						{
							DBG("Something went really wrong!");
							return false;
						}

						//get values between first parantheses
						int indexCloseParan = line.indexOf(")");
						if(mass->getType() != PortType)
						{
							String params = line.substring(indexParantese+1, indexCloseParan);
							StringArray paramsArray;
							paramsArray.addTokens(params, ",", "\"");
							for (int param = 0; param < paramsArray.size(); ++param) {
								float paramVal = paramsArray[param].trimCharactersAtStart(" ").getFloatValue();
								mass->addParameter(paramVal);
							}
						}
						// get remaining line content
						line = line.substring(indexCloseParan+1);
						line = line.trimCharactersAtStart(",");

						// get string till next comma
						int commaIndex = line.indexOf(",");
						if(commaIndex != -1)
							mass->setName(line.substring(0, commaIndex));

						line = line.substring(commaIndex);

						indexParantese = line.indexOf("(");
						indexCloseParan = line.indexOf(")");
						String labels = line.substring(indexParantese+1, indexCloseParan);
						StringArray labelsArray;
						labelsArray.addTokens(labels, ",", "\"");
						for (int l = 0; l < labelsArray.size(); ++l) {
							mass->addLabel(labelsArray[l]);
						}

						mdlFile.addMassObject(mass);
					}
				}
			}
			else if(line.startsWith("link")
					|| line.startsWith("touch")
					|| line.startsWith("pluck"))
			{
				int indexParantese = line.indexOf("(");
				// mass or link type
				if(indexParantese != -1)
				{
					String objType = line.substring(0, indexParantese);
					if(objType.compare("link") == 0 ||
							objType.compare("touch") == 0 ||
							objType.compare("pluck") == 0)
					{
						LinkObject* link;
						if(objType.compare("link") == 0)
						{
							link = ObjectFactory::createNewLink();
						}
						else if(objType.compare("touch") == 0)
						{
							link = ObjectFactory::createNewTouch();
						}
						else if(objType.compare("pluck") == 0)
						{
							link = ObjectFactory::createNewPluck();
						}
						else
						{
							DBG("Something went really wrong!");
							return false;
						}

						//get values between first parantheses
						int indexCloseParan = line.indexOf(")");
						String params = line.substring(indexParantese+1, indexCloseParan);
						StringArray paramsArray;
						paramsArray.addTokens(params, ",", "\"");
						for (int param = 0; param < paramsArray.size(); ++param) {
							String paramVal = paramsArray[param].trimCharactersAtStart(" ");
							link->addParameter(paramVal);
						}
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
							link->setName(paramsArray2[0]);
							link->setStartVertex(paramsArray2[1]);
							link->setEndVertex(paramsArray2[2]);
						}
						line = line.substring(indexParantese);
						indexCloseParan = line.indexOf(")");
						String labels = line.substring(1, indexCloseParan);
						StringArray labelsArray;
						labelsArray.addTokens(labels, ",", "\"");
						for (int l = 0; l < labelsArray.size(); ++l) {
							link->addLabel(labelsArray[l]);
						}

						mdlFile.addLinkObject(link);
					}
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
				LabelObject* labelObj = ObjectFactory::createNewLabelObject();
				labelObj->setName(lineTmp.substring(0,indexEquals));
				labelObj->addParameter(lineTmp.substring(indexEquals+1));
				mdlFile.addLabelObject(labelObj);
			}
			else if(line.startsWith("audioout"))
			{
				StringArray audioOutAttributeList;
				int indexSemicolon = line.length()-1;
				String lineTmp = line.substring(0, indexSemicolon);
				audioOutAttributeList.addTokens(lineTmp, ",", "\"");
				if(audioOutAttributeList.size() > 2)
				{
					AudioObject* audioObj = ObjectFactory::createNewAudioObject();
					audioObj->setName(audioOutAttributeList[1]);
					audioObj->addParameter(audioOutAttributeList[2]);
					mdlFile.addAudioObject(audioObj);
				}
			}
			else if(line.startsWith("waveguide"))
			{

			}
			else if(line.startsWith("termination"))
			{

			}
			else if(line.startsWith("junction"))
			{

			}

		}

	}


	return true;
}
