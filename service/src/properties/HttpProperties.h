//
// Created by Ivan Kishchenko on 26.04.2021.
//

#ifndef ROVER_HTTPPROPERTIES_H
#define ROVER_HTTPPROPERTIES_H

#include "Properties.h"

struct HttpProperties : Properties {
    std::string host;
    uint16_t port;
    std::optional<std::string> root;
};


#endif //ROVER_HTTPPROPERTIES_H
