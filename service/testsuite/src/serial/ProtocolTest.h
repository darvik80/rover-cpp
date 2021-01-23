//
// Created by Kishchenko, Ivan on 9/5/19.
//

#ifndef ROVER_PROTOCOLTEST_H
#define ROVER_PROTOCOLTEST_H

#include <serial/Protocol.h>
#include <boost/test/unit_test.hpp>

class ProtocolFixture {
public:
    protocol::OperationManager _manager;
};


#endif //ROVER_PROTOCOLTEST_H
