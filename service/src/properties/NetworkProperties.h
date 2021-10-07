//
// Created by Ivan Kishchenko on 11.04.2021.
//

#pragma once

#include <Properties.h>

struct NetworkProperties : Properties {
    std::string registryHost = "127.0.0.1";
    uint16_t registryPort = 54545;
};
