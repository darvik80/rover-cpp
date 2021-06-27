//
// Created by Ivan Kishchenko on 2019-06-16.
//

#ifndef ROVER_JSONRPCHANDLERTEST_H
#define ROVER_JSONRPCHANDLERTEST_H

#define BOOST_TEST_MODULE Incubator

#include <boost/test/unit_test.hpp>
#include <net/http/JsonRpcHandler.h>

class JsonRpcHandlerFixture {
public:
    JsonRpcHandler _rpcHandler;

    JsonRpcHandlerFixture();
};

#endif //ROVER_JSONRPCHANDLERTEST_H
