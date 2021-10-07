//
// Created by Ivan Kishchenko on 26.04.2021.
//
#pragma once

#include "Properties.h"

struct HttpProperties : Properties {
    std::string host;
    uint16_t port;
    std::optional<std::string> root;
};
