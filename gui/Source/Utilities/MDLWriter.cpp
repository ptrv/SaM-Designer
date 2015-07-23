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

//-----------------------------------------------------------------------------

void MDLWriter::getMDLString(String& mdlContent)
{
    String mdlHeader(BinaryData::mdl_file_header_txt);

    mdlContent = String::empty;
    mdlContent << mdlHeader;

    auto fnWriteContent = [&mdlContent](const String& content)
    {
        if (content.isNotEmpty())
        {
            mdlContent << "\n\n";
        }
        mdlContent << content;
    };
    // ------------------------------------------------------------------------
    // write masses
    fnWriteContent(getMassesString());

    // write links
    fnWriteContent(getLinksString());

    // write waveguides
    fnWriteContent(getWaveguidesString());

    // write terminations
    fnWriteContent(getTerminationsString());

    // write junctions
    fnWriteContent(getJunctionsString());

    // write variables
    fnWriteContent(getFaustCodeString());

    // write audioouts
    fnWriteContent(getAudioOutsString());

    // write comments
    fnWriteContent(getCommentsString());

    // write displays
    fnWriteContent(getDisplaysString());
}

//-----------------------------------------------------------------------------

String MDLWriter::getMassesString()
{
    String massString;
    ValueTree masses = mdlFile.mdlRoot.getChildWithName(Objects::masses);
    for (int mIdx = 0; mIdx < masses.getNumChildren(); ++mIdx)
    {
        ValueTree mo = masses.getChild(mIdx);
        massString << mo.getType().toString();
        massString << "(";
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

                massString << vals.joinIntoString(",");

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
                massString << massParam.getProperty(Ids::value).toString();

                if (i != massParams.getNumChildren() - 1)
                    massString << ",";
            }
        }

        massString << "),";
        massString << mo[Ids::identifier].toString();
        massString << ";";
        massString << " # pos " << mo.getProperty(Ids::posX, "0").toString() << "," << mo.getProperty(Ids::posY, "0").toString();
        massString << "\n";
    }

    return massString;
}

//-----------------------------------------------------------------------------

String MDLWriter::getLinksString()
{
    String linkString;
    ValueTree links = mdlFile.mdlRoot.getChildWithName(Objects::links);
    for (int lIdx = 0; lIdx < links.getNumChildren(); ++lIdx)
    {
        ValueTree li = links.getChild(lIdx);
        linkString << li.getType().toString();
        linkString << "(";
        ValueTree linkParams = li.getChildWithName(Ids::parameters);
        for (int k = 0; k < linkParams.getNumChildren(); ++k) {
            ValueTree link = linkParams.getChild(k);
            linkString << link.getProperty(Ids::value).toString();

            if(k != linkParams.getNumChildren()-1)
                linkString << ",";
        }
        linkString << "),";
        linkString << li[Ids::identifier].toString();
        linkString << ",";
        linkString << li[Ids::startVertex].toString();
        linkString << ",";
        linkString << li[Ids::endVertex].toString();
        linkString << ";";
        linkString << "\n";
    }

    return linkString;
}

//-----------------------------------------------------------------------------

String MDLWriter::getWaveguidesString()
{
    String waveguideString;
    ValueTree waveObjs = mdlFile.mdlRoot.getChildWithName(Objects::waveguides);
    for (int wavIdx = 0; wavIdx < waveObjs.getNumChildren(); ++wavIdx) {
        ValueTree wo = waveObjs.getChild(wavIdx);
        waveguideString << wo.getType().toString();
        waveguideString << "(";
        ValueTree waveParams = wo.getChildWithName(Ids::parameters);

        ValueTree waveParam = waveParams.getChild(0);
        waveguideString << waveParam.getProperty(Ids::value).toString();
        waveguideString << ",";

        ValueTree string = waveParams.getChild(1);
        waveguideString << string[Ids::value].toString();

        waveguideString << "),";
        waveguideString << wo[Ids::identifier].toString();
        waveguideString << ",";
        waveguideString << wo[Ids::startVertex].toString();
        waveguideString << ",";
        waveguideString << wo[Ids::endVertex].toString();
        waveguideString << ";";
        waveguideString << "\n";

    }

    return waveguideString;
}

//-----------------------------------------------------------------------------

String MDLWriter::getTerminationsString()
{
    String terminationString;
    ValueTree termObjs = mdlFile.mdlRoot.getChildWithName(Objects::terminations);
    for (int termIdx = 0; termIdx < termObjs.getNumChildren(); ++termIdx) {
        ValueTree to = termObjs.getChild(termIdx);
        terminationString << to.getType().toString();
        terminationString << "(";
        ValueTree termParams = to.getChildWithName(Ids::parameters);

        ValueTree term = termParams.getChild(0);
        terminationString << term[Ids::value].toString();

        terminationString << "),";
        terminationString << to[Ids::identifier].toString();
        terminationString << ";";
        terminationString << " # pos " << to.getProperty(Ids::posX, "0").toString() << "," << to.getProperty(Ids::posY, "0").toString();
        terminationString << "\n";

    }

    return terminationString;
}

//-----------------------------------------------------------------------------

String MDLWriter::getJunctionsString()
{
    String junctionsString;
    ValueTree junctObjs = mdlFile.mdlRoot.getChildWithName(Objects::junctions);
    for (int junctIdx = 0; junctIdx < junctObjs.getNumChildren(); ++junctIdx) {
        ValueTree jo = junctObjs.getChild(junctIdx);
        junctionsString << jo.getType().toString();
        junctionsString << "(";
        ValueTree junctP = jo.getChildWithName(Ids::parameters).getChild(0);
        junctionsString << junctP.getProperty(Ids::value).toString();
        junctionsString << "),";
        junctionsString << jo[Ids::identifier].toString();
        junctionsString << ";";
        junctionsString << " # pos " << jo.getProperty(Ids::posX, "0").toString() << "," << jo.getProperty(Ids::posY, "0").toString();
        junctionsString << "\n";

    }

    return junctionsString;
}

//-----------------------------------------------------------------------------

String MDLWriter::getFaustCodeString()
{
    String faustCodeString;
    ValueTree faustcodeblockTree = mdlFile.mdlRoot.getChildWithName(Objects::faustcodeblock);
    for (int varIdx = 0; varIdx < faustcodeblockTree.getNumChildren(); ++varIdx) {
        ValueTree la = faustcodeblockTree.getChild(varIdx);
        faustCodeString << "faustcode: ";
        faustCodeString << la.getProperty(Ids::value).toString();
        faustCodeString << "\n";
    }
    return faustCodeString;
}

//-----------------------------------------------------------------------------

String MDLWriter::getAudioOutsString()
{
    String audioOutsString;
    ValueTree audioTree = mdlFile.mdlRoot.getChildWithName(Objects::audioobjects);
    for (int aIdx = 0; aIdx < audioTree.getNumChildren(); ++aIdx) {
        ValueTree ao = audioTree.getChild(aIdx);
        audioOutsString << ao.getType().toString();
        audioOutsString << ",";
        audioOutsString << ao[Ids::identifier].toString();
        audioOutsString << ",";
        ValueTree sources = ao.getChildWithName(Ids::sources);
        bool isOpt = ao[Ids::optional].toString() != String::empty;
        if(isOpt)
            audioOutsString << "(";
        if(sources.getNumChildren() == 0)
        {
            audioOutsString << "0.0";
        }

        StringArray audioSources;
        for (int q = 0; q < sources.getNumChildren(); ++q)
        {
            ValueTree source = sources.getChild(q);
            String sourceStr = source.getProperty(Ids::value).toString();
            audioSources.add(sourceStr.isNotEmpty() ? sourceStr : "0.0");
        }
        audioOutsString << audioSources.joinIntoString("+");

        if(isOpt)
        {
            audioOutsString << ")";
            // if(! ao[Ids::optional].toString().contains("outputDSP"))
            // {
            //     audioOutsString << ":outputDSP";
            // }
            audioOutsString << ":" << ao[Ids::optional].toString();
        }

        audioOutsString << ";";
        audioOutsString << " # pos " << ao.getProperty(Ids::posX, "0").toString() << "," << ao.getProperty(Ids::posY, "0").toString();
        audioOutsString << "\n";

    }

    return audioOutsString;
}

//-----------------------------------------------------------------------------

String MDLWriter::getCommentsString()
{
    String commentsString;
    ValueTree commentTree = mdlFile.mdlRoot.getChildWithName(Objects::comments);
    for (int commIdx = 0; commIdx < commentTree.getNumChildren(); ++commIdx)
    {
        ValueTree comm = commentTree.getChild(commIdx);
        commentsString << "## ";
        commentsString << comm.getType().toString();
        commentsString << "(";
        StringArray commVal;
        commVal.addTokens(comm[Ids::value].toString(),"\n" ,"\"");
        commentsString << commVal.joinIntoString("|").quoted();
        commentsString << ",";
        commentsString << comm[Ids::fontSize].toString();
        commentsString << ",";
        commentsString << comm[Ids::commentColour].toString();
        commentsString << "),";
        commentsString << comm[Ids::identifier].toString();
        commentsString << ";";
        commentsString << " # pos " << comm.getProperty(Ids::posX, "0").toString();
        commentsString << ",";
        commentsString << comm.getProperty(Ids::posY, "0").toString();
        commentsString << "\n";
    }
    return commentsString;
}

//-----------------------------------------------------------------------------

String MDLWriter::getDisplaysString()
{
    String displaysString;
    ValueTree displaysTree = mdlFile.mdlRoot.getChildWithName(Objects::displays);
    for (int dIdx = 0; dIdx < displaysTree.getNumChildren(); ++dIdx) {
        ValueTree display = displaysTree.getChild(dIdx);
        displaysString << display.getType().toString();
        displaysString << "(";
        const ValueTree& params = display.getChildWithName(Ids::parameters);
        if (params.isValid())
        {
            StringArray paramsArray;
            for (int pIdx = 0; pIdx < params.getNumChildren(); ++pIdx)
            {
                if (pIdx == 0)
                {
                    String labelValue = params.getChild(pIdx).getProperty(Ids::value, "");
                    paramsArray.add(labelValue.quoted());
                }
                else
                {
                    paramsArray.add(params.getChild(pIdx).getProperty(Ids::value, "0.0"));
                }
            }
            displaysString << paramsArray.joinIntoString(",");
        }
        displaysString << "),";
        displaysString << display[Ids::identifier].toString();
        displaysString << ",";
        ValueTree sources = display.getChildWithName(Ids::sources);
        bool isOpt = display[Ids::optional].toString() != String::empty;
        if(isOpt)
            displaysString << "(";
        if(sources.getNumChildren() == 0)
        {
            displaysString << "0.0";
        }

        StringArray audioSources;
        for (int q = 0; q < sources.getNumChildren(); ++q)
        {
            ValueTree source = sources.getChild(q);
            String sourceStr = source.getProperty(Ids::value).toString();
            audioSources.add(sourceStr.isNotEmpty() ? sourceStr : "0.0");
        }
        displaysString << audioSources.joinIntoString("+");

        if(isOpt)
        {
            displaysString << ")";
            displaysString << ":" << display[Ids::optional].toString();
        }

        displaysString << ";";
        displaysString << " # pos " << display.getProperty(Ids::posX, "0").toString() << "," << display.getProperty(Ids::posY, "0").toString();
        displaysString << "\n";

    }

    return displaysString;
}

//-----------------------------------------------------------------------------

bool MDLWriter::writeMDL(const File& saveFile)
{
    const File& outFile = saveFile;//(savePath);

    String mdlContent;
    getMDLString(mdlContent);
    // ------------------------------------------------------------------------
    return Utils::writeStringToFile(mdlContent, outFile);
}

//-----------------------------------------------------------------------------

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
