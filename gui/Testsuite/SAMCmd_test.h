/*
  ==============================================================================

    SAMCmd_test.h
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

#ifndef __SAMCMD_TEST_H_E0BDB8A1__
#define __SAMCMD_TEST_H_E0BDB8A1__

#include "JuceHeader.h"

#include "TestUtils.h"

#include "Utilities/CmdHelper.h"


namespace synthamodeler
{
class SAMCmdTest : public UnitTest {
public:
    SAMCmdTest() : UnitTest("OutputCmdTest") {}

    void runTest()
    {
        const String dataPath = TestUtils::getTestsuiteDataPath();
        const String inPath = dataPath + "test.mdl";
        const String expectedPath = dataPath + "test_expected.dsp";
        const String outPath = dataPath + "test.dsp";

//		SAMCmd cmd(inPath, SAMCmd::NONE);
        beginTest("isPerlAvailable");
        expect(CmdHelper::isPerlAvailable(), "");
        beginTest("isSAMpreprocessorCmdAvailable");
        expect(CmdHelper::isSAMpreprocessorCmdAvailable(), "");
        beginTest("isSynthAModelerCmdAvailable");
        expect(CmdHelper::isSynthAModelerCmdAvailable(), "");
        beginTest("generateFaustCode");
//		String inPathFull = File::getCurrentWorkingDirectory().getChildFile(inPath).getFullPathName();
//		String outPathFull = File::getCurrentWorkingDirectory().getChildFile(outPath).getFullPathName();
//		String result = cmd.generateFaustCode(inPathFull, outPathFull, false);
//		bool resultOk = false;
//		if(result.compare("failed to start process") != 0)
//			resultOk = true;
//		expect(resultOk, "failed to start process");
//		File outFile(outPath);
//		File expectedFile(expectedPath);
//		expectEquals(outFile.loadFileAsString(), expectedFile.loadFileAsString(),
//				"actual and expected files differ");
//		outFile.deleteFile();

    }
};

static SAMCmdTest samCmdTest;
}

#endif  // __SAMCMD_TEST_H_E0BDB8A1__
