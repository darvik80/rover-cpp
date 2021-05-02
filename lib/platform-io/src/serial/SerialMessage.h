//
// Created by Ivan Kishchenko on 01.05.2021.
//

#ifndef PLATFORM_IO_SERIALMESSAGE_H
#define PLATFORM_IO_SERIALMESSAGE_H

#include <Arduino.h>
#include <etl/message.h>

enum {
    MSG_SERIAL_CONNECTED,
    MSG_SERIAL_DISCONNECTED,
    MSG_SERIAL_MESSAGE,
};

class SerialService;

class SerialMessage {
    const SerialService &service;

public:
    explicit SerialMessage(const SerialService &service)
            : service(service) {}

    const SerialService &getService() const {
        return service;
    }
};

class SerialConnected : public SerialMessage, public etl::message<MSG_SERIAL_CONNECTED> {
public:
    explicit SerialConnected(const SerialService &service)
            : SerialMessage(service) {}
};

class SerialDisconnected : public SerialMessage, public etl::message<MSG_SERIAL_DISCONNECTED> {
public:
    explicit SerialDisconnected(const SerialService &service)
            : SerialMessage(service) {}
};

class SerialDataMessage : public SerialMessage, public etl::message<MSG_SERIAL_MESSAGE> {
    uint8_t msgId;
    std::vector<uint8_t> &data;

public:
    SerialDataMessage(const SerialService &service, uint8_t msgId, std::vector<uint8_t> &data)
            : SerialMessage(service), msgId(msgId), data(data) {}
};

#endif //PLATFORM_IO_SERIALMESSAGE_H
