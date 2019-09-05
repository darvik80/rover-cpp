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
    uint32_t cmd = *(uint32_t*)packet.substr(0, 4).c_str();


    switch (cmd) {
        case 1:
            std::cout << "Init message" << std::endl;
    }

        return 0;
}
