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

namespace synthamodeler
{
class MDLFileTest : public UnitTest
{
public:
	MDLFileTest() : UnitTest("MDLFileTest") {}

	void runTest()
	{
		String mdlPath = String(TESTSUITE_DATA_PATH) + "test.mdl";
		ScopedPointer<MDLFile> mdlFile;
		mdlFile = new MDLFile();

		beginTest("openMDL");
		bool loadOk = mdlFile->loadFrom(File(mdlPath), false);
		expect(loadOk, "Error opening MDL file");

		beginTest("getMasses");
		const ValueTree m = mdlFile->mdlRoot.getChildWithName(Objects::masses);
        // test 1
        expect(m.getNumChildren() == 5, "have "+String(m.getNumChildren())+" masses");

        //======================================================================
		// mass 1
        //======================================================================
        // test 2
		expectEquals(m.getChild(0).getType().toString(), Ids::mass.toString());
		ValueTree params = m.getChild(0).getChildWithName(Ids::parameters);
        // test 3
		expectEquals(params.getChild(0)[Ids::value].toString(), String("0.01"));
        // test 4
		expectEquals(m.getChild(0)[Ids::identifier].toString(), String("m1"));

        //======================================================================
		// mass 2
        //======================================================================
        // test 5
		expectEquals(m.getChild(1).getType().toString(), Ids::mass.toString());
		params = m.getChild(1).getChildWithName(Ids::parameters);
        // test 6
		expectEquals(params.getChild(0)[Ids::value].toString(), String("0.01"));
        // test 7
		expectEquals(m.getChild(1)[Ids::identifier].toString(), String("m2"));

        //======================================================================
		// mass 3
        //======================================================================
        // test 8
		expectEquals(m.getChild(2).getType().toString(), Ids::mass.toString());
		params = m.getChild(2).getChildWithName(Ids::parameters);
        // test 9
		expectEquals(params.getChild(0)[Ids::value].toString(), String("0.03"));
        // test 10
		expectEquals(m.getChild(2)[Ids::identifier].toString(), String("m3"));

        //======================================================================
		// port
        //======================================================================
        // test 11
		expectEquals(m.getChild(3).getType().toString(), Ids::port.toString());
//		params = m.getChild(3).getChildWithName(Ids::parameters);
        // test 12
		expectEquals(m.getChild(3)[Ids::identifier].toString(), String("dev1"));

        //======================================================================
		// ground
        //======================================================================
        // test 13
		expectEquals(m.getChild(4).getType().toString(), Ids::ground.toString());
//		params = m.getChild(4).getChildWithName(Ids::parameters);
        // test 14
		expectEquals(m.getChild(4)[Ids::identifier].toString(), String("g"));

		beginTest("getLinks");

		ValueTree ls = mdlFile->mdlRoot.getChildWithName(Objects::links);
        // test 1
		expect(ls.getNumChildren() == 3, "have "+String(ls.getNumChildren())+" links");

        //======================================================================
		// link 1
        //======================================================================
        // test 2
		expectEquals(ls.getChild(0).getType().toString(), Ids::link.toString(), "");
		ValueTree paramsStr = ls.getChild(0).getChildWithName(Ids::parameters);
        // test 3
		expectEquals(paramsStr.getChild(0)[Ids::value].toString(), String("2.0*adjStiffness"));
        // test 4
		expectEquals(paramsStr.getChild(1)[Ids::value].toString(), String("0.003"));
        // test 5
		expectEquals(paramsStr.getChild(2)[Ids::value].toString(), String("0.0"));
        // test 6
		expectEquals(ls.getChild(0)[Ids::identifier].toString(), String("l1"));
        // test 7
		expectEquals(ls.getChild(0)[Ids::startVertex].toString(), String("m1"));
        // test 8
		expectEquals(ls.getChild(0)[Ids::endVertex].toString(), String("m2"));

        //======================================================================
		// link 2
        //======================================================================
        // test 9
		expectEquals(ls.getChild(1).getType().toString(), Ids::link.toString(), "");
		paramsStr = ls.getChild(1).getChildWithName(Ids::parameters);
        // test 10
		expectEquals(paramsStr.getChild(0)[Ids::value].toString(), String("2.0*adjStiffness"));
        // test 11
		expectEquals(paramsStr.getChild(1)[Ids::value].toString(), String("0.004"));
        // test 12
		expectEquals(paramsStr.getChild(2)[Ids::value].toString(), String("0.0"));
        // test 13
		expectEquals(ls.getChild(1)[Ids::identifier].toString(), String("l2"));
        // test 14
		expectEquals(ls.getChild(1)[Ids::startVertex].toString(), String("m2"));
        // test 15
		expectEquals(ls.getChild(1)[Ids::endVertex].toString(), String("m3"));

        //======================================================================
		// link 3
        //======================================================================
        // test 16
		expectEquals(ls.getChild(2).getType().toString(), Ids::link.toString(), "");
		paramsStr = ls.getChild(2).getChildWithName(Ids::parameters);
        // test 17
		expectEquals(paramsStr.getChild(0)[Ids::value].toString(), String("2.0*adjStiffness"));
        // test 18
		expectEquals(paramsStr.getChild(1)[Ids::value].toString(), String("0.005"));
        // test 19
		expectEquals(paramsStr.getChild(2)[Ids::value].toString(), String("0.0"));
        // test 20
		expectEquals(ls.getChild(2)[Ids::identifier].toString(), String("l3"));
        // test 21
		expectEquals(ls.getChild(2)[Ids::startVertex].toString(), String("m2"));
        // test 22
		expectEquals(ls.getChild(2)[Ids::endVertex].toString(), String("dev1"));

		beginTest("getFaustcode");

		ValueTree faustcodeObjs = mdlFile->mdlRoot.getChildWithName(Objects::faustcodeblock);
        // test 1
		expect(faustcodeObjs.getNumChildren() == 2, "have "+String(faustcodeObjs.getNumChildren())+" variables");
        // test 2
        expectEquals(faustcodeObjs.getChild(0)[Ids::value].toString(), String("adjStiffness=hslider(\"stiffness\", 2200.0, 500.0, 4000.0, 100.0);"));
//        // test 3
        expectEquals(faustcodeObjs.getChild(1)[Ids::value].toString(), String("outputDSP=highpass(4,20.0);"));

//		beginTest("getWaveguides");
//
//		ValueTree waveObjs = mdlFile->mdlRoot.getChildWithName(Objects::waveguides);
//		expect(waveObjs.getNumChildren() == 2, "have "+String(waveObjs.getNumChildren())+" waveguides");
//
//		// waveguide 1
//		expectEquals(waveObjs.getChild(0).getType().toString(), Ids::waveguide.toString());
//		expectEquals(waveObjs.getChild(0)[Ids::identifier].toString(), String("wg1"));
//		params = waveObjs.getChild(0).getChildWithName(Ids::parameters);
//		expectEquals(float(params[Ids::idx[0]]), 5.0f);
//		expectEquals(float(params[Ids::idx[1]]), 0.01f);
//		expectEquals(waveObjs.getChild(0)[Ids::objLeft].toString(), String("T1"));
//		expectEquals(waveObjs.getChild(0)[Ids::objRight].toString(), String("junct1"));
//
//
//		// waveguide 2
//		expectEquals(waveObjs.getChild(1).getType().toString(), Ids::waveguide.toString());
//		expectEquals(waveObjs.getChild(1)[Ids::identifier].toString(), String("wg2"));
//		params = waveObjs.getChild(1).getChildWithName(Ids::parameters);
//		expectEquals(float(params[Ids::idx[0]]), 5.0f);
//		expectEquals(float(params[Ids::idx[1]]), 0.01f);
//		expectEquals(waveObjs.getChild(1)[Ids::objLeft].toString(), String("junct1"));
//		expectEquals(waveObjs.getChild(1)[Ids::objRight].toString(), String("T2"));
//
//		beginTest("getTerminations");
//
//		ValueTree termObjs = mdlFile->mdlRoot.getChildWithName(Objects::terminations);
//		expect(termObjs.getNumChildren() == 2, "have "+String(termObjs.getNumChildren())+" terminations");
//
//		// termination 1
//		expectEquals(termObjs.getChild(0).getType().toString(), Ids::termination.toString());
//		expectEquals(termObjs.getChild(0)[Ids::identifier].toString(), String("T1"));
//		params = termObjs.getChild(0).getChildWithName(Ids::parameters);
//		expectEquals(params[Ids::idx[0]].toString(), String("terminationFilter(0.1,5)"));
//
//		// termination 2
//		expectEquals(termObjs.getChild(1).getType().toString(), Ids::termination.toString());
//		expectEquals(termObjs.getChild(1)[Ids::identifier].toString(), String("T2"));
//		params = termObjs.getChild(1).getChildWithName(Ids::parameters);
//		expectEquals(params[Ids::idx[0]].toString(), String("terminationFilter(0.01,12)"));
//
//		beginTest("getJunctions");
//
//		ValueTree junctObjs = mdlFile->mdlRoot.getChildWithName(Objects::junctions);
//		expect(junctObjs.getNumChildren() == 1, "have "+String(junctObjs.getNumChildren())+" junctions");
//
//		// termination 1
//		expectEquals(junctObjs.getChild(0).getType().toString(), Ids::junction.toString());
//		expectEquals(junctObjs.getChild(0)[Ids::identifier].toString(), String("junct1"));
//		params = junctObjs.getChild(0).getChildWithName(Ids::parameters);
//		expectEquals(float(params[Ids::idx[0]]), 0.01f);

		beginTest("getAudioObjects");

		ValueTree aus = mdlFile->mdlRoot.getChildWithName(Objects::audioobjects);
        // test 1
		expect(aus.getNumChildren() == 2, "have "+String(aus.getNumChildren())+" audio objects");

        // test 2
		expectEquals(aus.getChild(0).getType().toString(), Ids::audioout.toString());
        // test 3
		expectEquals(aus.getChild(0)[Ids::identifier].toString(), String("a1"));
        String actualSources1;
        ValueTree sources1 = aus.getChild(0).getChildWithName(Ids::sources);
        for (int i = 0; i < sources1.getNumChildren(); i++)
        {
            ValueTree src = sources1.getChild(i);
            actualSources1 << src[Ids::value].toString();
            if(i != sources1.getNumChildren()-1)
                actualSources1 << "+";
        }
        actualSources1 << aus.getChild(0).getProperty(Ids::optional, "").toString();
        // test 4
		expectEquals(actualSources1, String("m1*(1000.0)+l1*(100.0)"));

        // test 5
		expectEquals(aus.getChild(1).getType().toString(), Ids::audioout.toString());
        // test 6
		expectEquals(aus.getChild(1)[Ids::identifier].toString(), String("a2"));
        String actualSources2 = String::empty;
        ValueTree sources2 = aus.getChild(1).getChildWithName(Ids::sources);
        for (int i = 0; i < sources2.getNumChildren(); i++)
        {
            ValueTree src = sources2.getChild(i);
            actualSources2 << src[Ids::value].toString();
            if(i != sources2.getNumChildren()-1)
                actualSources2 << "+";
        }
        // test 7
		expectEquals(actualSources2, String("l2*(1000.0)+l3*(-50.0)+l1*(0.01)"));
	}
};

static MDLFileTest mdlFileTest;
}

#endif  // __MDLFILE_TEST_H_234EF909__
