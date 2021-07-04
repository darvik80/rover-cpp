//
// Created by Ivan Kishchenko on 01.05.2021.
//

#ifndef PLATFORM_IO_SERIALSERVICE_H
#define PLATFORM_IO_SERIALSERVICE_H

#include "Const.h"
#include "service/Service.h"
#include <serial/Protocol.h>
#include <serial/SerialPortCodec.h>
#include <etl/function.h>
#include <etl/optional.h>

typedef void(*callback)();

class SerialService : public BaseService, public serial::SerialPort, public serial::SerialPortCodecCallback {
    Stream& _stream;
    serial::SerialPortCodec _codec;
public:
    explicit SerialService(Registry& registry, Stream& stream);
    void postConstruct() override;

    void run() override;
    void preDestroy() override;

    void send(uint8_t msgId, const uint8_t* data, uint16_t size);
    void send(uint8_t msgId, const char* data) {
        send(msgId, (const uint8_t*)data, strlen(data));
    }

    const char* deviceId() override;

    int send(const uint8_t *data, size_t size) override;

    void onMessage(const uint8_t *data, size_t size) override;

    uint16_t crc16(const uint8_t *data, size_t size) override;

public:
    void onMessage(serial::SerialPort &port, serial::Message &msg) override;

    void onConnect(SerialPort &port) override;

    void onDisconnect(SerialPort &port) override;
};


#endif //PLATFORM_IO_SERIALSERVICE_H
