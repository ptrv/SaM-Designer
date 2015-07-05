/*
  ==============================================================================

    MDLWriter.cpp
    Created: 22 Apr 2012 1:07:57am
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

#include "MDLWriter.h"

#include "Application/CommonHeaders.h"

#include "Models/MDLFile.h"


using namespace synthamodeler;

MDLWriter::MDLWriter(const MDLFile& mdlFile_)
: mdlFile(mdlFile_)
{
}

void MDLWriter::getMDLString(String& mdlContent)
{
    String mdlHeader(BinaryData::mdl_file_header_txt);

	mdlContent = String::empty;
	mdlContent << mdlHeader;

	// ------------------------------------------------------------------------
	// write masses
	mdlContent << "\n\n";
	ValueTree masses = mdlFile.mdlRoot.getChildWithName(Objects::masses);
	for (int mIdx = 0; mIdx < masses.getNumChildren(); ++mIdx)
	{
		ValueTree mo = masses.getChild(mIdx);
		mdlContent << mo.getType().toString();
		mdlContent << "(";
		ValueTree massParams = mo.getChildWithName(Ids::parameters);
        if (mo.getType() == Ids::resonators)
        {
            if(massParams.getNumChildren() == 3)
            {
                Array<ValueTree> resParams;
                for (int n = 0; n < 3; ++n)
                    resParams.add(massParams.getChild(n));

                int numRes = jmin<int>(
                    resParams[0].getNumChildren(),
                    resParams[1].getNumChildren(),
                    resParams[2].getNumChildren());
                StringArray vals;

                for (int n = 0; n < numRes; ++n)
                {
                    for (int m = 0; m < 3; ++m)
                        vals.add(resParams[m].getChild(n)[Ids::value].toString());
                }

                mdlContent << vals.joinIntoString(",");

                for (int n = 0; n < 3; ++n)
                {
                    for (int m = resParams[n].getNumChildren(); --m >= numRes;)
                        massParams.getChild(n).removeChild(m, nullptr);
                }
            }
        }
        else
        {
            for (int i = 0; i < massParams.getNumChildren(); ++i)
            {
                ValueTree massParam = massParams.getChild(i);
                mdlContent << massParam.getProperty(Ids::value).toString();

                if (i != massParams.getNumChildren() - 1)
                    mdlContent << ",";
            }
        }

		mdlContent << "),";
		mdlContent << mo[Ids::identifier].toString();
		mdlContent << ";";
		mdlContent << " # pos " << mo.getProperty(Ids::posX, "0").toString() << "," << mo.getProperty(Ids::posY, "0").toString();
		mdlContent << "\n";
	}

	// write links
	mdlContent << "\n\n";
	ValueTree links = mdlFile.mdlRoot.getChildWithName(Objects::links);
	for (int lIdx = 0; lIdx < links.getNumChildren(); ++lIdx)
	{
		ValueTree li = links.getChild(lIdx);
		mdlContent << li.getType().toString();
		mdlContent << "(";
		ValueTree linkParams = li.getChildWithName(Ids::parameters);
		for (int k = 0; k < linkParams.getNumChildren(); ++k) {
            ValueTree link = linkParams.getChild(k);
            mdlContent << link.getProperty(Ids::value).toString();

			if(k != linkParams.getNumChildren()-1)
				mdlContent << ",";
		}
		mdlContent << "),";
		mdlContent << li[Ids::identifier].toString();
		mdlContent << ",";
		mdlContent << li[Ids::startVertex].toString();
		mdlContent << ",";
		mdlContent << li[Ids::endVertex].toString();
		mdlContent << ";";
		mdlContent << "\n";
	}

	// write waveguides
	mdlContent << "\n\n";
	ValueTree waveObjs = mdlFile.mdlRoot.getChildWithName(Objects::waveguides);
	for (int wavIdx = 0; wavIdx < waveObjs.getNumChildren(); ++wavIdx) {
		ValueTree wo = waveObjs.getChild(wavIdx);
		mdlContent << wo.getType().toString();
		mdlContent << "(";
		ValueTree waveParams = wo.getChildWithName(Ids::parameters);

        ValueTree waveParam = waveParams.getChild(0);
        mdlContent << waveParam.getProperty(Ids::value).toString();
        mdlContent << ",";

        ValueTree string = waveParams.getChild(1);
        mdlContent << string[Ids::value].toString();

		mdlContent << "),";
		mdlContent << wo[Ids::identifier].toString();
		mdlContent << ",";
		mdlContent << wo[Ids::startVertex].toString();
		mdlContent << ",";
		mdlContent << wo[Ids::endVertex].toString();
		mdlContent << ";";
		mdlContent << "\n";

	}

	// write terminations
	mdlContent << "\n\n";
	ValueTree termObjs = mdlFile.mdlRoot.getChildWithName(Objects::terminations);
	for (int termIdx = 0; termIdx < termObjs.getNumChildren(); ++termIdx) {
		ValueTree to = termObjs.getChild(termIdx);
		mdlContent << to.getType().toString();
		mdlContent << "(";
		ValueTree termParams = to.getChildWithName(Ids::parameters);

        ValueTree term = termParams.getChild(0);
        mdlContent << term[Ids::value].toString();

		mdlContent << "),";
		mdlContent << to[Ids::identifier].toString();
		mdlContent << ";";
		mdlContent << " # pos " << to.getProperty(Ids::posX, "0").toString() << "," << to.getProperty(Ids::posY, "0").toString();
		mdlContent << "\n";

	}

	// write junctions
	mdlContent << "\n\n";
	ValueTree junctObjs = mdlFile.mdlRoot.getChildWithName(Objects::junctions);
	for (int junctIdx = 0; junctIdx < junctObjs.getNumChildren(); ++junctIdx) {
		ValueTree jo = junctObjs.getChild(junctIdx);
		mdlContent << jo.getType().toString();
		mdlContent << "(";
        ValueTree junctP = jo.getChildWithName(Ids::parameters).getChild(0);
		mdlContent << junctP.getProperty(Ids::value).toString();
		mdlContent << "),";
		mdlContent << jo[Ids::identifier].toString();
		mdlContent << ";";
		mdlContent << " # pos " << jo.getProperty(Ids::posX, "0").toString() << "," << jo.getProperty(Ids::posY, "0").toString();
		mdlContent << "\n";

	}

	// write variables
	mdlContent << "\n\n";
	ValueTree faustcodeblockTree = mdlFile.mdlRoot.getChildWithName(Objects::faustcodeblock);
	for (int varIdx = 0; varIdx < faustcodeblockTree.getNumChildren(); ++varIdx) {
		ValueTree la = faustcodeblockTree.getChild(varIdx);
		mdlContent << "faustcode: ";
		mdlContent << la.getProperty(Ids::value).toString();
		mdlContent << "\n";
	}

	// write audioouts
	mdlContent << "\n\n";
	ValueTree audioTree = mdlFile.mdlRoot.getChildWithName(Objects::audioobjects);
	for (int aIdx = 0; aIdx < audioTree.getNumChildren(); ++aIdx) {
		ValueTree ao = audioTree.getChild(aIdx);
		mdlContent << ao.getType().toString();
		mdlContent << ",";
		mdlContent << ao[Ids::identifier].toString();
		mdlContent << ",";
        ValueTree sources = ao.getChildWithName(Ids::sources);
        bool isOpt = ao[Ids::optional].toString() != String::empty;
        if(isOpt)
            mdlContent << "(";
        if(sources.getNumChildren() == 0)
        {
            mdlContent << "0.0";
        }

        StringArray audioSources;
        for (int q = 0; q < sources.getNumChildren(); ++q)
        {
            ValueTree source = sources.getChild(q);
            String sourceStr = source.getProperty(Ids::value).toString();
            audioSources.add(sourceStr.isNotEmpty() ? sourceStr : "0.0");
        }
        mdlContent << audioSources.joinIntoString("+");

        if(isOpt)
        {
            mdlContent << ")";
            if(! ao[Ids::optional].toString().contains("outputDSP"))
            {
                mdlContent << ":outputDSP";
            }
            mdlContent << ":" << ao[Ids::optional].toString();
        }

		mdlContent << ";";
		mdlContent << " # pos " << ao.getProperty(Ids::posX, "0").toString() << "," << ao.getProperty(Ids::posY, "0").toString();
		mdlContent << "\n";

	}
    // write comments
    ValueTree commentTree = mdlFile.mdlRoot.getChildWithName(Objects::comments);
    for (int commIdx = 0; commIdx < commentTree.getNumChildren(); ++commIdx)
    {
        ValueTree comm = commentTree.getChild(commIdx);
        mdlContent << "## ";
        mdlContent << comm.getType().toString();
        mdlContent << "(";
        StringArray commVal;
        commVal.addTokens(comm[Ids::value].toString(),"\n" ,"\"");
        mdlContent << commVal.joinIntoString("|").quoted();
        mdlContent << ",";
        mdlContent << comm[Ids::fontSize].toString();
        mdlContent << ",";
        mdlContent << comm[Ids::commentColour].toString();
        mdlContent << "),";
        mdlContent << comm[Ids::identifier].toString();
		mdlContent << ";";
		mdlContent << " # pos " << comm.getProperty(Ids::posX, "0").toString();
        mdlContent << ",";
        mdlContent << comm.getProperty(Ids::posY, "0").toString();
		mdlContent << "\n";
    }
}
bool MDLWriter::writeMDL(const File& saveFile)
{
	const File& outFile = saveFile;//(savePath);

    String mdlContent;
    getMDLString(mdlContent);
    // ------------------------------------------------------------------------
    return Utils::writeStringToFile(mdlContent, outFile);
}

bool MDLWriter::writeMDLX(const File& saveFile)
{
	const File& outFile = saveFile;//(savePath);

    ValueTree mdlxTree = mdlFile.mdlRoot.createCopy();
    mdlxTree.setProperty(Ids::mdlName, outFile.getFileName(), nullptr);
	mdlxTree.setProperty(Ids::mdlPath, outFile.getFullPathName(), nullptr);

    String mdlContent = mdlxTree.toXmlString();
    // ------------------------------------------------------------------------
    return Utils::writeStringToFile(mdlContent, outFile);
}
