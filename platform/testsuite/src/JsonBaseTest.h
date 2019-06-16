//
// Created by Ivan Kishchenko on 2019-06-16.
//

#ifndef ROVER_JSONBASETEST_H
#define ROVER_JSONBASETEST_H

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class JsonBaseTest: public CppUnit::TestCase {
public:

    void checkMarshal();
    void checkUnMarshal();

    CPPUNIT_TEST_SUITE( JsonBaseTest );
    CPPUNIT_TEST( checkMarshal );
    CPPUNIT_TEST( checkUnMarshal );
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION ( JsonBaseTest );


#endif //ROVER_JSONBASETEST_H
