/*
  ==============================================================================

    TestRunner.h
    Created: 11 Apr 2012 1:25:46pm
    Author:  Peter Vasil

  ==============================================================================
*/

#ifndef __TESTRUNNER_H_53168B6B__
#define __TESTRUNNER_H_53168B6B__

#include "../Source/StoredSettings.h"
static const char* delimLine = "-----------------------------------------------------------------";
class TestRunner {
private:
    TestRunner(){}
    ~TestRunner(){}
public:
    static void doTests()
    {
        UnitTestRunner runner;
        runner.setAssertOnFailure(false);
        runner.runAllTests();

        //----------------------------------------------------------------------

        int numTestsPassed = 0;
        int numTestsFailed = 0;
        // iterate over all results
        for (int i = 0; i < runner.getNumResults(); ++i)
        {
            const UnitTestRunner::TestResult* result = runner.getResult(i);

            // count total passes and failures
            numTestsPassed += result->passes;
            numTestsFailed += result->failures;

//            // print messages for those tests that failed
//            StringArray messages = result->messages;
//            for (int j = 0; j < messages.size(); ++j)
//            {
//                String failedMsg;
//                failedMsg << result->unitTestName << ": ";
//                failedMsg << messages[j] << newLine;
//                Logger::writeToLog(failedMsg);
//            }
        }

        //----------------------------------------------------------------------
        String logMsg;
        logMsg << newLine << delimLine << newLine;
        if (numTestsFailed > 0)
        {   // return the number of failed tests
            JUCEApplication::getInstance()->setApplicationReturnValue(numTestsFailed);
            logMsg << numTestsFailed << " tests failed, and ";
            logMsg << numTestsPassed << " tests were succesfull." << newLine;
        }
        else
        {
            logMsg << "All " << numTestsPassed << " tests passed." << newLine;
        }
        logMsg << delimLine << newLine;
        Logger::writeToLog(logMsg);

        StoredSettings::deleteInstance();

    }
};


#endif  // __TESTRUNNER_H_53168B6B__
