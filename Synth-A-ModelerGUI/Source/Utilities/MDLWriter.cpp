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

#include "../../JuceLibraryCode/JuceHeader.h"
#include "MDLWriter.h"
#include "../Models/ObjectIDs.h"

MDLWriter::MDLWriter(MDLFile& mdlFile_)
: mdlFile(mdlFile_)
{
}

bool MDLWriter::writeMDL(const File& saveFile)
{
	const File& outFile = saveFile;//(savePath);

	String mdlHeader(BinaryData::mdl_file_header_txt);

	String mdlContent;
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
		for (int i = 0; i < massParams.getNumProperties(); ++i)
		{
//			mdlContent << String::formatted("%f", float(massParams[Ids::idx[i]]));
			mdlContent << float(massParams[Ids::idx[i]]);
			if(i != massParams.getNumProperties()-1)
				mdlContent << ",";
		}
		mdlContent << "),";
		mdlContent << mo[Ids::identifier].toString();
		mdlContent << ",(";
		ValueTree labelTree = mo.getChildWithName(Ids::labels);
		for (int j = 0; j < labelTree.getNumProperties(); ++j)
		{
			mdlContent << labelTree[Ids::idx[j]].toString();
			if(j != labelTree.getNumProperties()-1)
				mdlContent << ",";
		}
		mdlContent << ");";
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
		for (int k = 0; k < linkParams.getNumProperties(); ++k) {
			mdlContent << linkParams[Ids::idx[k]].toString();
			if(k != linkParams.getNumProperties()-1)
				mdlContent << ",";
		}
		mdlContent << "),";
		mdlContent << li[Ids::identifier].toString();
		mdlContent << ",";
		mdlContent << li[Ids::startVertex].toString();
		mdlContent << ",";
		mdlContent << li[Ids::endVertex].toString();
		mdlContent << ",(";
		ValueTree labelTree = li.getChildWithName(Ids::labels);
		for (int j = 0; j < labelTree.getNumProperties(); ++j) {
			mdlContent << labelTree[Ids::idx[j]].toString();
			if(j != labelTree.getNumProperties()-1)
				mdlContent << ",";
		}
		mdlContent << ");";
		mdlContent << " # pos " << li.getProperty(Ids::posX, "0").toString() << "," << li.getProperty(Ids::posY, "0").toString();
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
		for (int m = 0; m < waveParams.getNumProperties(); ++m) {
			mdlContent << waveParams[Ids::idx[m]].toString();
			if(m != waveParams.getNumProperties()-1)
				mdlContent << ",";
		}
		mdlContent << "),";
		mdlContent << wo[Ids::identifier].toString();
		mdlContent << ",";
		mdlContent << wo[Ids::objLeft].toString();
		mdlContent << ",";
		mdlContent << wo[Ids::objRight].toString();
		mdlContent << ",(";
		ValueTree labelTree = wo.getChildWithName(Ids::labels);
		for (int n = 0; n < labelTree.getNumProperties(); ++n) {
			mdlContent << labelTree[Ids::idx[n]].toString();
			if(n != labelTree.getNumProperties()-1)
				mdlContent << ",";
		}
		mdlContent << ");";
		mdlContent << " # pos " << wo.getProperty(Ids::posX, "0").toString() << "," << wo.getProperty(Ids::posY, "0").toString();
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
		for (int o = 0; o < termParams.getNumProperties(); ++o) {
			mdlContent << termParams[Ids::idx[0]].toString();
			if(o != termParams.getNumProperties()-1)
				mdlContent << ",";
		}
		mdlContent << "),";
		mdlContent << to[Ids::identifier].toString();
		mdlContent << ",";
		ValueTree labelTree = to.getChildWithName(Ids::labels);
		for (int p = 0; p < labelTree.getNumProperties(); ++p) {
			mdlContent << labelTree[Ids::idx[p]].toString();
			if(p != labelTree.getNumProperties()-1)
				mdlContent << ",";
		}
		mdlContent << ");";
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
		mdlContent << jo.getChildWithName(Ids::parameters).getProperty(Ids::idx[0]).toString();
		mdlContent << "),";
		mdlContent << jo[Ids::identifier].toString();
		mdlContent << ",";
		ValueTree labelTree = jo.getChildWithName(Ids::labels);
		for (int q = 0; q < labelTree.getNumProperties(); ++q) {
			mdlContent << labelTree[Ids::idx[q]].toString();
			if(q != labelTree.getNumProperties()-1)
				mdlContent << ",";
		}
		mdlContent << ");";
		mdlContent << " # pos " << jo.getProperty(Ids::posX, "0").toString() << "," << jo.getProperty(Ids::posY, "0").toString();
		mdlContent << "\n";

	}

	// write labels
	mdlContent << "\n\n";
	ValueTree labelsTree = mdlFile.mdlRoot.getChildWithName(Objects::labels);
	for (int labIdx = 0; labIdx < labelsTree.getNumChildren(); ++labIdx) {
		ValueTree la = labelsTree.getChild(labIdx);
		mdlContent << "faustcode: ";
		mdlContent << la[Ids::identifier].toString();
		mdlContent << "=";
//		ValueTree laParams = la.getChildWithName(Ids::parameters);
		mdlContent << la.getProperty(Ids::faustCode).toString();
		mdlContent << ";";
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
		mdlContent << ao.getProperty(Ids::sources).toString();
		mdlContent << ";";
		mdlContent << " # pos " << ao.getProperty(Ids::posX, "0").toString() << "," << ao.getProperty(Ids::posY, "0").toString();
		mdlContent << "\n";

	}

	// ------------------------------------------------------------------------

	TemporaryFile temp (outFile);

	ScopedPointer <FileOutputStream> out (temp.getFile().createOutputStream());

	if (out != nullptr)
	{

		out->write(mdlContent.toUTF8(), mdlContent.getNumBytesAsUTF8());
		out = nullptr; // (deletes the stream)

		bool succeeded = temp.overwriteTargetFileWithTemporary();
		return succeeded;
	}
	else
	{
		return false;
	}
}
