/*
  ==============================================================================

    TestUtils.h
    Created: 13 Apr 2012 3:21:43pm
    Author:  peter

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

#ifndef __TESTUTILS_H_D0F1300A__
#define __TESTUTILS_H_D0F1300A__

#include "../JuceLibraryCode/JuceHeader.h"

#if JUCE_MAC
static const char* TESTSUITE_DATA_PATH = "../../../../Testsuite/data/";
#else
static const char* TESTSUITE_DATA_PATH = "../../../Testsuite/data/";
#endif


#endif  // __TESTUTILS_H_D0F1300A__
