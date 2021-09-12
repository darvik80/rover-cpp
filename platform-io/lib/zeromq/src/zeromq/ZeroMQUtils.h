//
// Created by Ivan Kishchenko on 29.08.2021.
//

#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <algorithm>
#ifdef RASPBERRY_ARCH
#include <machine/endian.h>
#else
#include <lwip/def.h>
#endif

class ZeroMQUtils {
public:
    static void init();

    static std::string netDump(const uint8_t *data, std::size_t size);

    static uint64_t swapEndian(uint64_t val) {
#ifdef RASPBERRY_ARCH
        return htonll(val);
#else
        return 0;
#endif
    }

    static uint32_t swapEndian(uint32_t val) {
#ifdef RASPBERRY_ARCH
        return htonl(val);
#else
        return lwip_htonl(val);
#endif
    }

    static uint16_t swapEndian(uint16_t val) {
#ifdef RASPBERRY_ARCH
        return htons(val);
#else
        return lwip_htons(val);
#endif
    }
};
