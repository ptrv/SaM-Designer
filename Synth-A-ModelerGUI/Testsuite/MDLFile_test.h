/*
  ==============================================================================

    MDLFile_test.h
    Created: 16 Apr 2012 2:00:10am
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

#ifndef __MDLFILE_TEST_H_234EF909__
#define __MDLFILE_TEST_H_234EF909__

#include "TestUtils.h"

class MDLFileTest : public UnitTest
{
public:
	MDLFileTest() : UnitTest("MDLFileTest") {}

	void runTest()
	{
		String mdlPath = String(TESTSUITE_DATA_PATH) + "examplelinks.mdl";
		ScopedPointer<MDLFile> mdlFile;
		mdlFile = new MDLFile();

		beginTest("openMDL");
		bool loadOk = mdlFile->loadFrom(File(mdlPath), false);
		expect(loadOk, "Error opening MDL file");

		beginTest("getMasses");
		const Array<Mass*>& m = mdlFile->getMasses();
		expect(m.size() == 5, "have "+String(m.size())+" masses");
		expect(mdlFile->getNumberOfObjectsByType(MassType) == 3, "");
		// mass 1
		expectEquals(m[0]->getType(), MassType);
		Array<float> params = m[0]->getParameters();
		expectEquals(params[0], 0.01f);
		expectEquals(m[0]->getName(), String("m1"));
		Array<String> labels = m[0]->getLabels();
		expectEquals(labels[0], String("mymass"));
		// mass 2
		expectEquals(m[1]->getType(), MassType);
		params = m[1]->getParameters();
		expectEquals(params[0], 0.01f);
		expectEquals(m[1]->getName(), String("m2"));
		labels = m[1]->getLabels();
		expectEquals(labels[0], String("mymass"));
		expectEquals(labels[1], String("mmiddle"));
		// mass 3
		expectEquals(m[2]->getType(), MassType);
		params = m[2]->getParameters();
		expectEquals(params[0], 0.03f);
		expectEquals(m[2]->getName(), String("m3"));
		labels = m[2]->getLabels();
		expectEquals(labels[0], String("mymass"));
		// port
		expectEquals(m[3]->getType(), PortType);
		params = m[3]->getParameters();
//		expect(params.size() == 0, "size is "+String(params.size()));
		expectEquals(m[3]->getName(), String("dev1"));
		labels = m[3]->getLabels();
		expectEquals(labels[0], String("hapticdev"));
		// port
		expectEquals(m[4]->getType(), GroundType);
		params = m[4]->getParameters();
		expectEquals(m[4]->getName(), String("g"));
		labels = m[4]->getLabels();
		expectEquals(labels[0], String("glabel"));

		beginTest("getLinks");

		const Array<LinkObject*>& ls = mdlFile->getLinks();
		expect(ls.size() == 4, "have "+String(ls.size())+" links");

		// link 1
		expectEquals(ls[0]->getType(), LinkType, "");
		Array<String> paramsStr = ls[0]->getParameters();
		expectEquals(paramsStr[0], String("2.0*adjStiffness"));
		expectEquals(paramsStr[1], String("0.003"));
		expectEquals(paramsStr[2], String("0.0"));
		expectEquals(ls[0]->getName(), String("l1"));
		expectEquals(ls[0]->getStartVertex(), String("m1"));
		expectEquals(ls[0]->getEndVertex(), String("m2"));
		labels = ls[0]->getLabels();
		expectEquals(labels[0], String("thisl"));

		// link 2
		expectEquals(ls[1]->getType(), LinkType, "");
		paramsStr = ls[1]->getParameters();
		expectEquals(paramsStr[0], String("2.0*adjStiffness"));
		expectEquals(paramsStr[1], String("0.004"));
		expectEquals(paramsStr[2], String("0.0"));
		expectEquals(ls[1]->getName(), String("l2"));
		expectEquals(ls[1]->getStartVertex(), String("m2"));
		expectEquals(ls[1]->getEndVertex(), String("m3"));
		labels = ls[1]->getLabels();
		expectEquals(labels[0], String("thisLLL"));
		expectEquals(labels[1], String("thisl"));

		// link 3
		expectEquals(ls[2]->getType(), LinkType, "");
		paramsStr = ls[2]->getParameters();
		expectEquals(paramsStr[0], String("2.0*adjStiffness"));
		expectEquals(paramsStr[1], String("0.005"));
		expectEquals(paramsStr[2], String("0.0"));
		expectEquals(ls[2]->getName(), String("l3"));
		expectEquals(ls[2]->getStartVertex(), String("m2"));
		expectEquals(ls[2]->getEndVertex(), String("dev1"));
		labels = ls[2]->getLabels();
		expectEquals(labels[0], String("thisl"));


		beginTest("getLabels");

		const Array<LabelObject*>& labelObjs = mdlFile->getLabels();
		expect(labelObjs.size() == 1, "have "+String(labelObjs.size())+" labels");
		expectEquals(labelObjs[0]->getName(), String("adjStiffness"));
		expectEquals(labelObjs[0]->getParameters(), String("hslider(\"stiffness\", 2200.0, 500.0, 100.0, 4000.0)"));

		beginTest("getWaveguides");

		const Array<WaveguideObject*>& waveObjs = mdlFile->getWaveguides();
		expect(waveObjs.size() == 2, "have "+String(waveObjs.size())+" waveguides");

		// waveguide 1
		expectEquals(waveObjs[0]->getType(), WaveguideType);
		expectEquals(waveObjs[0]->getName(), String("wg1"));
		params = waveObjs[0]->getParameters();
		expectEquals(params[0], 5.0f);
		expectEquals(params[1], 0.01f);
		labels = waveObjs[0]->getLabels();
		expectEquals(labels[0], String("wglabel"));
		expectEquals(waveObjs[0]->getObjectLeft(), String("T1"));
		expectEquals(waveObjs[0]->getObjectRight(), String("junct1"));

		// waveguide 2
		expectEquals(waveObjs[1]->getType(), WaveguideType);
		expectEquals(waveObjs[1]->getName(), String("wg2"));
		params = waveObjs[1]->getParameters();
		expectEquals(params[0], 5.0f);
		expectEquals(params[1], 0.01f);
		labels = waveObjs[1]->getLabels();
		expectEquals(labels[0], String("wglabel2"));
		expectEquals(waveObjs[1]->getObjectLeft(), String("junct1"));
		expectEquals(waveObjs[1]->getObjectRight(), String("T2"));

		beginTest("getTerminations");

		const Array<TerminationObject*>& termObjs = mdlFile->getTerminations();
		expect(termObjs.size() == 2, "have "+String(termObjs.size())+" terminations");

		// termination 1
		expectEquals(termObjs[0]->getType(), TerminationType);
		expectEquals(termObjs[0]->getName(), String("T1"));
		paramsStr = termObjs[0]->getParameters();
		expectEquals(paramsStr[0], String("terminationFilter(0.1,5)"));
		labels = termObjs[0]->getLabels();
		expectEquals(labels[0], String("labelT"));
		expectEquals(labels[1], String("anotherTLabel"));

		// termination 2
		expectEquals(termObjs[1]->getType(), TerminationType);
		expectEquals(termObjs[1]->getName(), String("T2"));
		paramsStr = termObjs[1]->getParameters();
		expectEquals(paramsStr[0], String("terminationFilter(0.01,12)"));
		labels = termObjs[1]->getLabels();
		expectEquals(labels[0], String("labelT"));

		beginTest("getJunctions");

		const Array<JunctionObject*>& junctObjs = mdlFile->getJunctions();
		expect(junctObjs.size() == 1, "have "+String(junctObjs.size())+" junctions");

		// termination 1
		expectEquals(junctObjs[0]->getType(), JunctionType);
		expectEquals(junctObjs[0]->getName(), String("junct1"));
		float param = junctObjs[0]->getParameters();
		expectEquals(param, 0.01f);
		labels = junctObjs[0]->getLabels();
		expectEquals(labels[0], String("junctLabel"));

		beginTest("getAudioObjects");

		const Array<AudioObject*>& aus = mdlFile->getAudioObjects();
		expect(aus.size() == 2, "have "+String(aus.size())+" audio objects");

		expectEquals(aus[0]->getType(), AudioObjectType);
		expectEquals(aus[0]->getName(), String("a1"));
		expectEquals(aus[0]->getParameters(), String("m1*(1000.0)+l1*(100.0)"));

		expectEquals(aus[1]->getType(), AudioObjectType);
		expectEquals(aus[1]->getName(), String("a2"));
		expectEquals(aus[1]->getParameters(), String("l2*(1000.0)+l3*(-50.0)+l1*(0.01)"));
	}
};

static MDLFileTest mdlFileTest;

#endif  // __MDLFILE_TEST_H_234EF909__
