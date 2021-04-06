//
// Created by Kishchenko, Ivan on 4/6/21.
//

#ifndef PLATFORM_IO_TRANSPORT_H
#define PLATFORM_IO_TRANSPORT_H

#include <stdint.h>
#include <stddef.h>

class Transport {
public:
    virtual void onMessage(const uint8_t* data, size_t size) = 0;
    virtual int send(const uint8_t * data, size_t size) = 0;

    virtual int send(const char* data, size_t size) {
        return send((const uint8_t*)data, size);
    }
};


#endif //PLATFORM_IO_TRANSPORT_H
