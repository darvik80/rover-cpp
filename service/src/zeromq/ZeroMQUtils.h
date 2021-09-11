//
// Created by Ivan Kishchenko on 29.08.2021.
//

#pragma once

#include <cstdint>
#include <cstddef>
#include <string>

class ZeroMQUtils {
public:
    static void init();
    static std::string netDump(const uint8_t* data, std::size_t size);
};
