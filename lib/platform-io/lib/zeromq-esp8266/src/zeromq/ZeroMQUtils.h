//
// Created by Ivan Kishchenko on 29.08.2021.
//

#ifndef PLATFORM_IO_ZEROMQUTILS_H
#define PLATFORM_IO_ZEROMQUTILS_H

#include <cstdint>
#include <cstddef>
#include <string>

class ZeroMQUtils {
public:
    static void init();

    static std::string netDump(const uint8_t *data, std::size_t size);
};


#endif //PLATFORM_IO_ZEROMQUTILS_H
