/*
  ==============================================================================

    OutputCmd_test.h
    Created: 11 Apr 2012 11:55:25pm
    Author:  peter

  ==============================================================================
*/

#ifndef __OUTPUTCMD_TEST_H_E0BDB8A1__
#define __OUTPUTCMD_TEST_H_E0BDB8A1__


class OutputCmdTest : public UnitTest {
public:
	OutputCmdTest() : UnitTest("OutputCmdTest") {}

	void runTest()
	{
		String inPath = "../../../Testsuite/data/examplelinks.mdl";
		String expectedPath = "../../../Testsuite/data/examplelinks_expected.dsp";
		String outPath = "../../../Testsuite/data/examplelinks.dsp";
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
