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
		const ValueTree m = mdlFile->mdlRoot.getChildWithName(Objects::masses);
		expect(m.getNumChildren() == 5, "have "+String(m.getNumChildren())+" masses");

		// mass 1
		expectEquals(m.getChild(0).getType().toString(), Ids::mass.toString());
		ValueTree params = m.getChild(0).getChildWithName(Ids::parameters);
		expectEquals(float(params[Ids::idx[0]]), 0.01f);
		expectEquals(m.getChild(0)[Ids::identifier].toString(), String("m1"));
		ValueTree labels = m.getChild(0).getChildWithName(Ids::labels);
		expectEquals(labels[Ids::idx[0]].toString(), String("mymass"));

		// mass 2
		expectEquals(m.getChild(1).getType().toString(), Ids::mass.toString());
		params = m.getChild(1).getChildWithName(Ids::parameters);
		expectEquals(float(params[Ids::idx[0]]), 0.01f);
		expectEquals(m.getChild(1)[Ids::identifier].toString(), String("m2"));
		labels = m.getChild(1).getChildWithName(Ids::labels);
		expectEquals(labels[Ids::idx[0]].toString(), String("mymass"));
		expectEquals(labels[Ids::idx[1]].toString(), String("mmiddle"));

		// mass 3
		expectEquals(m.getChild(2).getType().toString(), Ids::mass.toString());
		params = m.getChild(2).getChildWithName(Ids::parameters);
		expectEquals(float(params[Ids::idx[0]]), 0.03f);
		expectEquals(m.getChild(2)[Ids::identifier].toString(), String("m3"));
		labels = m.getChild(2).getChildWithName(Ids::labels);
		expectEquals(labels[Ids::idx[0]].toString(), String("mymass"));

		// port
		expectEquals(m.getChild(3).getType().toString(), Ids::port.toString());
//		params = m.getChild(3).getChildWithName(Ids::parameters);
		expectEquals(m.getChild(3)[Ids::identifier].toString(), String("dev1"));
		labels = m.getChild(3).getChildWithName(Ids::labels);
		expectEquals(labels[Ids::idx[0]].toString(), String("hapticdev"));

		// ground
		expectEquals(m.getChild(4).getType().toString(), Ids::ground.toString());
//		params = m.getChild(4).getChildWithName(Ids::parameters);
		expectEquals(m.getChild(4)[Ids::identifier].toString(), String("g"));
		labels = m.getChild(4).getChildWithName(Ids::labels);
		expectEquals(labels[Ids::idx[0]].toString(), String("glabel"));


		beginTest("getLinks");

		ValueTree ls = mdlFile->mdlRoot.getChildWithName(Objects::links);
		expect(ls.getNumChildren() == 4, "have "+String(ls.getNumChildren())+" links");

		// link 1
		expectEquals(ls.getChild(0).getType().toString(), Ids::link.toString(), "");
		ValueTree paramsStr = ls.getChild(0).getChildWithName(Ids::parameters);
		expectEquals(paramsStr[Ids::idx[0]].toString(), String("2.0*adjStiffness"));
		expectEquals(paramsStr[Ids::idx[1]].toString(), String("0.003"));
		expectEquals(paramsStr[Ids::idx[2]].toString(), String("0.0"));
		expectEquals(ls.getChild(0)[Ids::identifier].toString(), String("l1"));
		expectEquals(ls.getChild(0)[Ids::startVertex].toString(), String("m1"));
		expectEquals(ls.getChild(0)[Ids::endVertex].toString(), String("m2"));
		labels = ls.getChild(0).getChildWithName(Ids::labels);
		expectEquals(labels[Ids::idx[0]].toString(), String("thisl"));

		// link 2
		expectEquals(ls.getChild(1).getType().toString(), Ids::link.toString(), "");
		paramsStr = ls.getChild(1).getChildWithName(Ids::parameters);
		expectEquals(paramsStr[Ids::idx[0]].toString(), String("2.0*adjStiffness"));
		expectEquals(paramsStr[Ids::idx[1]].toString(), String("0.004"));
		expectEquals(paramsStr[Ids::idx[2]].toString(), String("0.0"));
		expectEquals(ls.getChild(1)[Ids::identifier].toString(), String("l2"));
		expectEquals(ls.getChild(1)[Ids::startVertex].toString(), String("m2"));
		expectEquals(ls.getChild(1)[Ids::endVertex].toString(), String("m3"));
		labels = ls.getChild(1).getChildWithName(Ids::labels);
		expectEquals(labels[Ids::idx[0]].toString(), String("thisLLL"));
		expectEquals(labels[Ids::idx[1]].toString(), String("thisl"));

		// link 3
		expectEquals(ls.getChild(2).getType().toString(), Ids::link.toString(), "");
		paramsStr = ls.getChild(2).getChildWithName(Ids::parameters);
		expectEquals(paramsStr[Ids::idx[0]].toString(), String("2.0*adjStiffness"));
		expectEquals(paramsStr[Ids::idx[1]].toString(), String("0.005"));
		expectEquals(paramsStr[Ids::idx[2]].toString(), String("0.0"));
		expectEquals(ls.getChild(2)[Ids::identifier].toString(), String("l3"));
		expectEquals(ls.getChild(2)[Ids::startVertex].toString(), String("m2"));
		expectEquals(ls.getChild(2)[Ids::endVertex].toString(), String("dev1"));
		labels = ls.getChild(2).getChildWithName(Ids::labels);
		expectEquals(labels[Ids::idx[0]].toString(), String("thisl"));

		beginTest("getLabels");

		ValueTree labelObjs = mdlFile->mdlRoot.getChildWithName(Objects::labels);
		expect(labelObjs.getNumChildren() == 1, "have "+String(labelObjs.getNumChildren())+" labels");
		expectEquals(labelObjs.getChild(0)[Ids::identifier].toString(), String("adjStiffness"));
		expectEquals(labelObjs.getChild(0)[Ids::faustCode].toString(), String("hslider(\"stiffness\", 2200.0, 500.0, 100.0, 4000.0)"));

		beginTest("getWaveguides");

		ValueTree waveObjs = mdlFile->mdlRoot.getChildWithName(Objects::waveguides);
		expect(waveObjs.getNumChildren() == 2, "have "+String(waveObjs.getNumChildren())+" waveguides");

		// waveguide 1
		expectEquals(waveObjs.getChild(0).getType().toString(), Ids::waveguide.toString());
		expectEquals(waveObjs.getChild(0)[Ids::identifier].toString(), String("wg1"));
		params = waveObjs.getChild(0).getChildWithName(Ids::parameters);
		expectEquals(float(params[Ids::idx[0]]), 5.0f);
		expectEquals(float(params[Ids::idx[1]]), 0.01f);
		labels = waveObjs.getChild(0).getChildWithName(Ids::labels);
		expectEquals(labels[Ids::idx[0]].toString(), String("wglabel"));
		expectEquals(waveObjs.getChild(0)[Ids::objLeft].toString(), String("T1"));
		expectEquals(waveObjs.getChild(0)[Ids::objRight].toString(), String("junct1"));


		// waveguide 2
		expectEquals(waveObjs.getChild(1).getType().toString(), Ids::waveguide.toString());
		expectEquals(waveObjs.getChild(1)[Ids::identifier].toString(), String("wg2"));
		params = waveObjs.getChild(1).getChildWithName(Ids::parameters);
		expectEquals(float(params[Ids::idx[0]]), 5.0f);
		expectEquals(float(params[Ids::idx[1]]), 0.01f);
		labels = waveObjs.getChild(1).getChildWithName(Ids::labels);
		expectEquals(labels[Ids::idx[0]].toString(), String("wglabel2"));
		expectEquals(waveObjs.getChild(1)[Ids::objLeft].toString(), String("junct1"));
		expectEquals(waveObjs.getChild(1)[Ids::objRight].toString(), String("T2"));

		beginTest("getTerminations");

		ValueTree termObjs = mdlFile->mdlRoot.getChildWithName(Objects::terminations);
		expect(termObjs.getNumChildren() == 2, "have "+String(termObjs.getNumChildren())+" terminations");

		// termination 1
		expectEquals(termObjs.getChild(0).getType().toString(), Ids::termination.toString());
		expectEquals(termObjs.getChild(0)[Ids::identifier].toString(), String("T1"));
		params = termObjs.getChild(0).getChildWithName(Ids::parameters);
		expectEquals(params[Ids::idx[0]].toString(), String("terminationFilter(0.1,5)"));
		labels = termObjs.getChild(0).getChildWithName(Ids::labels);
		expectEquals(labels[Ids::idx[0]].toString(), String("labelT"));
		expectEquals(labels[Ids::idx[1]].toString(), String("anotherTLabel"));

		// termination 2
		expectEquals(termObjs.getChild(1).getType().toString(), Ids::termination.toString());
		expectEquals(termObjs.getChild(1)[Ids::identifier].toString(), String("T2"));
		params = termObjs.getChild(1).getChildWithName(Ids::parameters);
		expectEquals(params[Ids::idx[0]].toString(), String("terminationFilter(0.01,12)"));
		labels = termObjs.getChild(1).getChildWithName(Ids::labels);
		expectEquals(labels[Ids::idx[0]].toString(), String("labelT"));

		beginTest("getJunctions");

		ValueTree junctObjs = mdlFile->mdlRoot.getChildWithName(Objects::junctions);
		expect(junctObjs.getNumChildren() == 1, "have "+String(junctObjs.getNumChildren())+" junctions");

		// termination 1
		expectEquals(junctObjs.getChild(0).getType().toString(), Ids::junction.toString());
		expectEquals(junctObjs.getChild(0)[Ids::identifier].toString(), String("junct1"));
		params = junctObjs.getChild(0).getChildWithName(Ids::parameters);
		expectEquals(float(params[Ids::idx[0]]), 0.01f);
		labels = junctObjs.getChild(0).getChildWithName(Ids::labels);
		expectEquals(labels[Ids::idx[0]].toString(), String("junctLabel"));

		beginTest("getAudioObjects");

		ValueTree aus = mdlFile->mdlRoot.getChildWithName(Objects::audioobjects);
		expect(aus.getNumChildren() == 2, "have "+String(aus.getNumChildren())+" audio objects");

		expectEquals(aus.getChild(0).getType().toString(), Ids::audioout.toString());
		expectEquals(aus.getChild(0)[Ids::identifier].toString(), String("a1"));
		expectEquals(aus.getChild(0)[Ids::sources].toString(), String("m1*(1000.0)+l1*(100.0)"));
		expectEquals(aus.getChild(1).getType().toString(), Ids::audioout.toString());
		expectEquals(aus.getChild(1)[Ids::identifier].toString(), String("a2"));
		expectEquals(aus.getChild(1)[Ids::sources].toString(), String("l2*(1000.0)+l3*(-50.0)+l1*(0.01)"));

	}
};

static MDLFileTest mdlFileTest;

#endif  // __MDLFILE_TEST_H_234EF909__
