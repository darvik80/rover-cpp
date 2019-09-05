//
// Created by Ivan Kishchenko on 2019-07-11.
//

#include "Protocol.h"
#include <string>
#include <boost/algorithm/string.hpp>

#include <iostream>

using namespace boost;

int main(int argc, char *argv[]) {
    std::string packet = "?MSG Hello World";

    protocol::OperationManager manager;
    auto op = manager.parse(packet);

    std::cout << op->toString();

    return 0;
}
