/*
  ==============================================================================

    MiscUtilities_test.h
    Created: 8 Aug 2013 2:28:14pm
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

#ifndef __MISCUTILITIES_TEST_H_C3534391__
#define __MISCUTILITIES_TEST_H_C3534391__

#include "TestUtils.h"
namespace synthamodeler
{
class MiscUtilitiesTest : public UnitTest {
public:
	MiscUtilitiesTest() : UnitTest("MiscUtilitiesTest") {}

	void runTest()
	{
//		String inPath = String(TESTSUITE_DATA_PATH) + "test.mdl";
//		String expectedPath = String(TESTSUITE_DATA_PATH) + "test_expected.dsp";
//		String outPath = String(TESTSUITE_DATA_PATH) + "test.dsp";

        beginTest("uniquifyPaths test");

        HashMap<String, String> pathsToTest;
        pathsToTest.set("/tmp/test/oud.mdl", "oud.mdl");
        pathsToTest.set("/tmp/oud.mdl", "oud.mdl");
        pathsToTest.set("/oud.mdl", "oud.mdl");
        pathsToTest.set("/tmp/test/test2/oud.mdl", "oud.mdl");
        pathsToTest.set("/tmp/test2/oud.mdl", "oud.mdl");

        Utils::uniquifyPaths(pathsToTest);

        expectEquals(pathsToTest["/tmp/test/oud.mdl"], String("tmp/test/oud.mdl"), String::empty);
        expectEquals(pathsToTest["/tmp/oud.mdl"], String("/tmp/oud.mdl"), String::empty);
        expectEquals(pathsToTest["/oud.mdl"], String("/oud.mdl"), String::empty);
        expectEquals(pathsToTest["/tmp/test/test2/oud.mdl"], String("test/test2/oud.mdl"), String::empty);
        expectEquals(pathsToTest["/tmp/test2/oud.mdl"], String("tmp/test2/oud.mdl"), String::empty);
	}
};

static MiscUtilitiesTest miscUtilitiesTest;
}


#endif  // __MISCUTILITIES_TEST_H_C3534391__
