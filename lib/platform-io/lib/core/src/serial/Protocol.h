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
        WAIT_SYNC,
        CONN
    };

    enum SystemMessage {
        MSG_SYNC = 0x01,
        MSG_CONN = 0x02,
        MSG_PING = 0x03,
        MSG_PONG = 0x04,
        MSG_DOWN = 0x05,
        MSG_LOG = 0x06,
    };

    struct Message {
        uint8_t msgId;
        uint16_t len;
        const uint8_t* data;
        uint16_t crc16;
    };
}

#endif //PLATFORM_IO_PROTOCOL_H
