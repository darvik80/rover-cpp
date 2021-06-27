//
// Created by Ivan Kishchenko on 02.05.2021.
//

#ifndef ROVER_SERIALPROPERTIES_H
#define ROVER_SERIALPROPERTIES_H

#include <Properties.h>

struct SerialProperties : Properties {
    std::string port;
    int baudRate;
};


#endif //ROVER_SERIALPROPERTIES_H
