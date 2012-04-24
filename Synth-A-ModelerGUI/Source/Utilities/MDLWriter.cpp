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
