/*
  ==============================================================================

    TestRunner.h
    Created: 11 Apr 2012 1:25:46pm
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

#ifndef __TESTRUNNER_H_53168B6B__
#define __TESTRUNNER_H_53168B6B__

#include "../Source/Utilities/StoredSettings.h"
namespace synthamodeler
{
static const char* delimLine = "-----------------------------------------------------------------";
class TestRunner
{
private:
    TestRunner(){}
    ~TestRunner(){}
public:
    static void doTests()
    {
#if JUCE_MAC
        Process::setDockIconVisible(false);
#endif

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
}

#endif  // __TESTRUNNER_H_53168B6B__
