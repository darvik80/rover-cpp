//
// Created by Ivan Kishchenko on 2019-06-16.
//

#ifndef ROVER_HANDLERJSONRPCTEST_H
#define ROVER_HANDLERJSONRPCTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "handlers/JsonRpcHandler.h"

class HandlerJsonRpcTest: public CppUnit::TestCase {
private:
    handlers::JsonRpcHandler handler;
public:
    void setUp() override;;

    void testHandleFunction();
    void testHandleSupplier();
    void testHandleConsumer();
    void testHandleMethodNotExists();

    CPPUNIT_TEST_SUITE(HandlerJsonRpcTest);
        CPPUNIT_TEST(testHandleFunction);
        CPPUNIT_TEST(testHandleSupplier);
        CPPUNIT_TEST(testHandleConsumer);
        CPPUNIT_TEST(testHandleMethodNotExists);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION (HandlerJsonRpcTest);


#endif //ROVER_HANDLERJSONRPCTEST_H
