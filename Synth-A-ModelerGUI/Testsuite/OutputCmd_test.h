/*
  ==============================================================================

    OutputCmd_test.h
    Created: 11 Apr 2012 11:55:25pm
    Author:  peter

  ==============================================================================
*/

#ifndef __OUTPUTCMD_TEST_H_E0BDB8A1__
#define __OUTPUTCMD_TEST_H_E0BDB8A1__

#include "TestUtils.h"
class OutputCmdTest : public UnitTest {
public:
	OutputCmdTest() : UnitTest("OutputCmdTest") {}

	void runTest()
	{
		String inPath = String(TESTSUITE_DATA_PATH) + "examplelinks.mdl";
		String expectedPath = String(TESTSUITE_DATA_PATH) + "examplelinks_expected.dsp";
		String outPath = String(TESTSUITE_DATA_PATH) + "examplelinks.dsp";
		OutputCmd cmd;
		beginTest("isSynthAModelerCmdAvailable");
		expect(cmd.isSynthAModelerCmdAvailable(), "");
		beginTest("isFaustAvailable");
		expect(cmd.isFaustAvailable(), "");
		beginTest("isFaust2supercolliderAvailable");
		expect(cmd.isFaust2supercolliderAvailable(), "");
		beginTest("isFaust2puredataAvailable");
		expect(cmd.isFaust2puredataAvailable(), "");
		beginTest("isPerlAvailable");
		expect(cmd.isPerlAvailable(), "");
		beginTest("generateFaustCode");
		String result = cmd.generateFaustCode(inPath, outPath);
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

private:
};

static OutputCmdTest outputCmdTest;

#endif  // __OUTPUTCMD_TEST_H_E0BDB8A1__
