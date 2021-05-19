//
// Created by Ivan Kishchenko on 16.05.2021.
//

#ifndef ROVER_SERIALPORT_H
#define ROVER_SERIALPORT_H

#include <stdint.h>
#include "Protocol.h"

namespace serial {

    class SerialPort {
    public:
        virtual const char* deviceId() = 0;

        virtual int send(const uint8_t *data, size_t size) = 0;
        virtual int send(const uint8_t data) {
            return send(&data, sizeof(uint8_t));
        }
        virtual int send(const uint16_t data)  {
            return send((const uint8_t *)&data, sizeof(uint16_t));
        }
        virtual int send(const uint32_t data)  {
            return send((const uint8_t *)&data, sizeof(uint32_t));
        }
        virtual int send(const char* data)  {
            return send((const uint8_t *)&data, strlen(data));
        }
        virtual int send(const Message& msg) {
            int res = send(MSG_MAGIC);
            res += send(msg.msgId);
            res += send(crc16(msg.data, msg.len));
            res += send(msg.len);
            if (msg.len > 0) {
                res += send(msg.data, msg.len);
            }
            res += send(MSG_MAGIC);

            return res;
        }

        virtual void onMessage(const uint8_t *data, size_t size) = 0;
        virtual uint16_t crc16(const uint8_t *data, size_t size) = 0;

        virtual ~SerialPort() = default;
    };

    class SerialPortHandler {
    public:
        virtual void onConnect(SerialPort& port) = 0;

        virtual void onDisconnect(SerialPort& port) = 0;

        virtual void onIdle(SerialPort& port) = 0;

        virtual void onMessage(SerialPort& port, const uint8_t *data, size_t size) = 0;

        virtual void onError(SerialPort& port, int ec) = 0;
    };
}

#endif //ROVER_SERIALPORT_H
