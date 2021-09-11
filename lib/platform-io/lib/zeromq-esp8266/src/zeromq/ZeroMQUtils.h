//
// Created by Ivan Kishchenko on 29.08.2021.
//

#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <algorithm>
#include <lwip/def.h>


class ZeroMQUtils {
    template<typename T>
    static T swapEndian(T val) {
        if (sizeof(val) == 1) {
            return val;
        }
        T res = val;
        void *buf = &res;
        static_assert(std::is_pod<T>::value, "swapEndian support POD type only");
        char *startIndex = static_cast<char *>((void *) buf);
        char *endIndex = startIndex + sizeof(T);
        std::reverse(startIndex, endIndex);

        return res;
    }
public:
    static void init();

    static std::string netDump(const uint8_t *data, std::size_t size);

    static uint64_t htonll(uint64_t val) {
        return swapEndian(val);
    }

    static uint32_t htonl(uint32_t val) {
        return lwip_htonl(val);
    }

    static uint16_t htons(uint16_t val) {
        return lwip_htons(val);
    }
};
