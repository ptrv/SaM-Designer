/*
  ==============================================================================

    MDLParser_test.h
    Created: 20 Sep 2012 4:18:11pm
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

#ifndef __MDLPARSER_TEST_H_E5DA3686__
#define __MDLPARSER_TEST_H_E5DA3686__

#include "TestUtils.h"

class MDLParserTest : public UnitTest
{
public:
    MDLParserTest() : UnitTest("MDLParserTest") {}

    void runTest()
    {
        beginTest("Test regular expressions");

        RegularExpression re;
        // test 1
        expect(re.fullMatch(SAMRegex::getVertexLine(),
                            "mass(0.003,0.0,0.0),bounceme,(); # pos 100, 100"));
        // test 2
        expect(re.fullMatch(SAMRegex::getVertexLine(),
                            "mass(0.003,0.0,0.0),bounceme,();"));
        // test 3
        expect(re.fullMatch(SAMRegex::getLinkLine(),
                            "link(3200.0, 2.5, 0.00),touch2 , bounceme, dev1,();"));
        // test 4
        expect(re.fullMatch(SAMRegex::getAudioOutLine(),
                            "audioout,a3,bounceme*1.0; # pos 100, 100"));
        // test 5
        expect(re.fullMatch(SAMRegex::getAudioOutLine(),
                            "audioout,a3,bounceme*1.0;"));
        // test 6
        expect(re.fullMatch(SAMRegex::getAudioOutLine(),
                            "audioout,a3,(bounceme*1.0):filter;"));
        // test 7
        expect(re.fullMatch(SAMRegex::getFaustLine(),
                            "faustcode:frequencyScaler = 0.3;"));
        // test 8
        expect(re.fullMatch(SAMRegex::getTerminationLine(),
                            "termination(simpleStringTerm(-0.991,30) ),t1 ,(); #pos 100, 100"));
        // test 9
        expect(re.fullMatch(SAMRegex::getTerminationLine(),
                            "termination(simpleStringTerm(-0.991,30) ),t1 ,();"));
        // test 10
        expect(re.fullMatch(SAMRegex::getJunctionLine(),
                            "junction(0.0),junct1,(); # pos 100, 100"));
        // test 11
        expect(re.fullMatch(SAMRegex::getJunctionLine(),
                            "junction(0.0),junct1,();"));
        // test 12
        expect(re.fullMatch(SAMRegex::getWaveguideLine(),
                            "waveguide(0.5 ,simpleString(0.023, 0.023*frequencyScaler)),w4,t4, junct1,();"));
        // test 13
        expect(re.fullMatch(SAMRegex::getVertexLine(),
                            "port(),dev1,();"));
        // test 14
        expect(re.fullMatch(SAMRegex::getVertexLine(),
                            "port(),dev1,(); # pos 100, 100"));
        // test 15
        expect(re.fullMatch(SAMRegex::getVertexLine(),
                            "ground(0.0),g,(); # pos 100, 100"));
        // test 16
        expect(re.fullMatch(SAMRegex::getVertexLine(),
                            "ground(0.0),g,();"));
        // test 17
        expect(re.fullMatch(SAMRegex::getVertexLine(),
                            "resonator(200.0,1.5,0.01),r0,(); # pos 223,385"));
        // test 18
        expect(re.fullMatch(SAMRegex::getVertexLine(),
                            "resonator(200.0,1.5,0.01),r0,();"));

    }
};

static MDLParserTest mdlParserTest;


#endif  // __MDLPARSER_TEST_H_E5DA3686__
