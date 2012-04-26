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

MDLWriter::MDLWriter(MDLFile& mdlFile_)
: mdlFile(mdlFile_)
{
}

bool MDLWriter::writeMDL(const String& savePath)
{
	File outFile(savePath);

	String mdlHeader(BinaryData::mdl_file_header_txt);

	String mdlContent;
	mdlContent << mdlHeader;

	// ------------------------------------------------------------------------
	// write masses
	mdlContent << "\n\n";
	const Array<MassObject*>& masses = mdlFile.getMasses();
	for (int mIdx = 0; mIdx < masses.size(); ++mIdx)
	{
		MassObject* mo = masses[mIdx];
		mdlContent << mo->getTypeString();
		mdlContent << "(";
		for (int i = 0; i < mo->getParameters().size(); ++i)
		{
			mdlContent << mo->getParameters()[i];
			if(i != mo->getParameters().size()-1)
				mdlContent << ",";
		}
		mdlContent << "),";
		mdlContent << mo->getName();
		mdlContent << ",(";
		for (int j = 0; j < mo->getLabels().size(); ++j)
		{
			mdlContent << mo->getLabels()[j];
			if(j != mo->getLabels().size()-1)
				mdlContent << ",";
		}
		mdlContent << ");";
		mdlContent << " # pos " << mo->getPosition().x << "," << mo->getPosition().y;
		mdlContent << "\n";
	}

	// write links
	mdlContent << "\n\n";
	const Array<LinkObject*>& links = mdlFile.getLinks();
	for (int lIdx = 0; lIdx < links.size(); ++lIdx)
	{
		LinkObject* li = links[lIdx];
		mdlContent << li->getTypeString();
		mdlContent << "(";
		for (int k = 0; k < li->getParameters().size(); ++k) {
			mdlContent << li->getParameters()[k];
			if(k != li->getParameters().size()-1)
				mdlContent << ",";
		}
		mdlContent << "),";
		mdlContent << li->getName();
		mdlContent << ",";
		mdlContent << li->getStartVertex();
		mdlContent << ",";
		mdlContent << li->getEndVertex();
		mdlContent << ",(";
		for (int j = 0; j < li->getLabels().size(); ++j) {
			mdlContent << li->getLabels()[j];
			if(j != li->getLabels().size()-1)
				mdlContent << ",";
		}
		mdlContent << ");";
		mdlContent << " # pos " << li->getPosition().x << "," << li->getPosition().y;
		mdlContent << "\n";
	}

	// write waveguides
	mdlContent << "\n\n";
	const Array<WaveguideObject*>& waveObjs = mdlFile.getWaveguides();
	for (int wavIdx = 0; wavIdx < waveObjs.size(); ++wavIdx) {
		WaveguideObject* wo = waveObjs[wavIdx];
		mdlContent << wo->getTypeString();
		mdlContent << "(";
		for (int m = 0; m < wo->getParameters().size(); ++m) {
			mdlContent << wo->getParameters()[m];
			if(m != wo->getParameters().size()-1)
				mdlContent << ",";
		}
		mdlContent << "),";
		mdlContent << wo->getName();
		mdlContent << ",";
		mdlContent << wo->getObjectLeft();
		mdlContent << ",";
		mdlContent << wo->getObjectRight();
		mdlContent << ",(";
		for (int n = 0; n < wo->getLabels().size(); ++n) {
			mdlContent << wo->getLabels()[n];
			if(n != wo->getLabels().size()-1)
				mdlContent << ",";
		}
		mdlContent << ");";
		mdlContent << " # pos " << wo->getPosition().x << "," << wo->getPosition().y;
		mdlContent << "\n";

	}

	// write terminations
	mdlContent << "\n\n";
	const Array<TerminationObject*>& termObjs = mdlFile.getTerminations();
	for (int termIdx = 0; termIdx < termObjs.size(); ++termIdx) {
		TerminationObject* to = termObjs[termIdx];
		mdlContent << to->getTypeString();
		mdlContent << "(";
		for (int o = 0; o < to->getParameters().size(); ++o) {
			mdlContent << to->getParameters()[0];
			if(o != to->getParameters().size()-1)
				mdlContent << ",";
		}
		mdlContent << "),";
		mdlContent << to->getName();
		mdlContent << ",";
		for (int p = 0; p < to->getLabels().size(); ++p) {
			mdlContent << to->getLabels()[p];
			if(p != to->getLabels().size()-1)
				mdlContent << ",";
		}
		mdlContent << ");";
		mdlContent << " # pos " << to->getPosition().x << "," << to->getPosition().y;
		mdlContent << "\n";

	}

	// write junctions
	mdlContent << "\n\n";
	const Array<JunctionObject*>& junctObjs = mdlFile.getJunctions();
	for (int junctIdx = 0; junctIdx < junctObjs.size(); ++junctIdx) {
		JunctionObject* jo = junctObjs[junctIdx];
		mdlContent << jo->getTypeString();
		mdlContent << "(";
		mdlContent << jo->getParameters();
		mdlContent << "),";
		mdlContent << jo->getName();
		mdlContent << ",";
		for (int q = 0; q < jo->getLabels().size(); ++q) {
			mdlContent << jo->getLabels()[q];
			if(q != jo->getLabels().size()-1)
				mdlContent << ",";
		}
		mdlContent << ");";
		mdlContent << " # pos " << jo->getPosition().x << "," << jo->getPosition().y;
		mdlContent << "\n";

	}

	// write labels
	mdlContent << "\n\n";
	const Array<LabelObject*>& labels = mdlFile.getLabels();
	for (int labIdx = 0; labIdx < labels.size(); ++labIdx) {
		LabelObject* la = labels[labIdx];
		mdlContent << "faustcode: ";
		mdlContent << la->getName();
		mdlContent << "=";
		mdlContent << la->getParameters();
		mdlContent << ";";
		mdlContent << "\n";
	}

	// write audioouts
	mdlContent << "\n\n";
	const Array<AudioObject*>& audioobjs = mdlFile.getAudioObjects();
	for (int aIdx = 0; aIdx < audioobjs.size(); ++aIdx) {
		AudioObject* ao = audioobjs[aIdx];
		mdlContent << ao->getTypeString();
		mdlContent << ",";
		mdlContent << ao->getName();
		mdlContent << ",";
		mdlContent << ao->getParameters();
		mdlContent << ";";
		mdlContent << " # pos " << ao->getPosition().x << "," << ao->getPosition().y;
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
