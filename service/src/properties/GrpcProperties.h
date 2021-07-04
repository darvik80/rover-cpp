//
// Created by Ivan Kishchenko on 04.07.2021.
//

#ifndef ROVER_GRPCPROPERTIES_H
#define ROVER_GRPCPROPERTIES_H

#include "Properties.h"

struct GrpcProperties : public Properties {
    std::string host;
    uint16_t port;
};

#endif //ROVER_GRPCPROPERTIES_H
