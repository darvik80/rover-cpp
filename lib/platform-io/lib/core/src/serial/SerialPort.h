//
// Created by Ivan Kishchenko on 16.05.2021.
//

#ifndef ROVER_SERIALPORT_H
#define ROVER_SERIALPORT_H

#include <stdint.h>

namespace serial {

    class SerialPort {
    public:
        virtual std::string deviceId() = 0;

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
        virtual void flush() { }

        virtual void onMessage(const uint8_t *data, size_t size) = 0;
        virtual uint16_t crc16(const uint8_t *data, size_t size) = 0;
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
