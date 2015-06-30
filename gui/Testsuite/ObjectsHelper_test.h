/*
  ==============================================================================

    ObjectsHelper_test.h
    Created: 22 Jul 2014 1:57:08am
    Author:  peter

  ==============================================================================
*/

#ifndef OBJECTSHELPER_TEST_H_INCLUDED
#define OBJECTSHELPER_TEST_H_INCLUDED

#include "TestUtils.h"

namespace synthamodeler
{
class ObjectsHelperTest : public UnitTest
{
public:
    ObjectsHelperTest() : UnitTest("ObjectsHelperTest") {}

    void runTest()
    {
        beginTest("fixValue test");

        expectEquals(ObjectsHelper::fixParameterValueIfNeeded(".1"), String("0.1"));
        expectEquals(ObjectsHelper::fixParameterValueIfNeeded("1"), String("1.0"));
        expectEquals(ObjectsHelper::fixParameterValueIfNeeded("1*1"), String("1.0*1.0"));
        expectEquals(ObjectsHelper::fixParameterValueIfNeeded("1*(-1)"), String("1.0*(-1.0)"));

    }
};

static ObjectsHelperTest objectsHelperTest;

}


#endif  // OBJECTSHELPER_TEST_H_INCLUDED
