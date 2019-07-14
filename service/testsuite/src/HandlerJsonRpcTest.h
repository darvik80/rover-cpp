//
// Created by Ivan Kishchenko on 2019-06-16.
//

#ifndef ROVER_HANDLERJSONRPCTEST_H
#define ROVER_HANDLERJSONRPCTEST_H

#define BOOST_TEST_MODULE Service

#include <boost/test/unit_test.hpp>
#include <http/JsonRpcHandler.h>

class HandlerJsonRpcFixture {
public:
    JsonRpcHandler _rpcHandler;
public:

    HandlerJsonRpcFixture();
};

#endif //ROVER_HANDLERJSONRPCTEST_H
