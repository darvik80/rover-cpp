//
// Created by Kishchenko, Ivan on 4/1/21.
//

#ifndef ROVER_MULTICASTMESSAGE_H
#define ROVER_MULTICASTMESSAGE_H

#include <string>

struct MulticastMessage {
    std::string id{};
    std::string name{};
    std::string type{};
};

#endif //ROVER_MULTICASTMESSAGE_H
