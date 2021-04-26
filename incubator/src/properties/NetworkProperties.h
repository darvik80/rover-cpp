//
// Created by Ivan Kishchenko on 11.04.2021.
//

#ifndef ROVER_NETWORKPROPERTIES_H
#define ROVER_NETWORKPROPERTIES_H

#include <Properties.h>

struct NetworkProperties : Properties {
    std::string registryHost = "127.0.0.1";
    uint16_t registryPort = 54545;
};

#endif //ROVER_NETWORKPROPERTIES_H
