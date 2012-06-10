/*
  ==============================================================================

    OutputCmd_test.h
    Created: 11 Apr 2012 11:55:25pm
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

#ifndef __OUTPUTCMD_TEST_H_E0BDB8A1__
#define __OUTPUTCMD_TEST_H_E0BDB8A1__

#include "TestUtils.h"
class OutputCmdTest : public UnitTest {
public:
	OutputCmdTest() : UnitTest("OutputCmdTest") {}

	void runTest()
	{
		String inPath = String(TESTSUITE_DATA_PATH) + "test.mdl";
		String expectedPath = String(TESTSUITE_DATA_PATH) + "test_expected.dsp";
		String outPath = String(TESTSUITE_DATA_PATH) + "test.dsp";

		OutputCmd cmd;
		beginTest("isSynthAModelerCmdAvailable");
		expect(cmd.isSynthAModelerCmdAvailable(), "");
		beginTest("isPerlAvailable");
		expect(cmd.isPerlAvailable(), "");
		beginTest("generateFaustCode");
		String inPathFull = File::getCurrentWorkingDirectory().getChildFile(inPath).getFullPathName();
		String outPathFull = File::getCurrentWorkingDirectory().getChildFile(outPath).getFullPathName();
		String result = cmd.generateFaustCode(inPathFull, outPathFull);
		bool resultOk = false;
		if(result.compare("failed to start process") != 0)
			resultOk = true;
		expect(resultOk, "failed to start process");
		File outFile(outPath);
		File expectedFile(expectedPath);
		expectEquals(outFile.loadFileAsString(), expectedFile.loadFileAsString(),
				"actual and expected files differ");
		outFile.deleteFile();

	}
};

static OutputCmdTest outputCmdTest;

#endif  // __OUTPUTCMD_TEST_H_E0BDB8A1__
