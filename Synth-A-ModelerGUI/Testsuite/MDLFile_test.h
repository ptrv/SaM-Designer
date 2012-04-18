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
		MDLFile mdlFile;

		beginTest("openMDL");
		bool loadOk = mdlFile.openMDL(mdlPath.toUTF8().getAddress());
		expect(loadOk, "Error opening MDL file");

		beginTest("getMasses");
		const Array<Mass*>& m = mdlFile.getMasses();
		expect(m.size() == 4, "have "+String(m.size())+" masses");
		expect(mdlFile.getNumberOfObjectsByType(MassType) == 3, "");
		expectEquals(m[0]->getType(), MassType);
		Array<float> params = m[0]->getParameters();
		expectEquals(params[0], 0.01f);
		Array<String> labels = m[0]->getLabels();
		expectEquals(labels[0], String("mymass"));
	}
};

static MDLFileTest mdlFileTest;

#endif  // __MDLFILE_TEST_H_234EF909__
