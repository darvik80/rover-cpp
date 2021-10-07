//
// Created by Ivan Kishchenko on 04.07.2021.
//

#pragma once

#include "Properties.h"

struct GrpcProperties : public Properties {
    std::string host;
    uint16_t port;
};
