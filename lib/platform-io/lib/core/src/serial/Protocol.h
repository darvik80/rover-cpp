//
// Created by Ivan Kishchenko on 01.05.2021.
//

#ifndef PLATFORM_IO_PROTOCOL_H
#define PLATFORM_IO_PROTOCOL_H

#include "CoreConfig.h"
#include <stdint.h>

namespace serial {

    const uint8_t MSG_MAGIC = 0x7e;

    enum ConnState {
        IDLE,
        WAIT_RECV,
        WAIT_SYNC,
        CONN
    };

    enum SystemMessage {
        MSG_SYNC = 0x01,
        MSG_CONN = 0x02,
    };
}

#endif //PLATFORM_IO_PROTOCOL_H
