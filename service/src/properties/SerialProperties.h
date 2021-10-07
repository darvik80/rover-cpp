//
// Created by Ivan Kishchenko on 02.05.2021.
//

#pragma once

#include <Properties.h>

struct SerialProperties : Properties {
    std::string port;
    int baudRate;
};
