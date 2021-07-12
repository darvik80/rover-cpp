//
// Created by Ivan Kishchenko on 01.05.2021.
//

#include "SerialService.h"
#include <serial/Protocol.h>
#include <etl/crc16.h>

#include "SerialMessage.h"

SerialService::SerialService(Registry& registry, Stream &stream)
        : BaseService(registry), _stream(stream), _codec(serial::SerialPortCodec::MODE_SLAVE, *this) {
}

void SerialService::postConstruct() {
    Serial.println("ARM Device, proto: v1");
}

void SerialService::run() {
    if (!_stream.available()) {
        return;
    }

    uint8_t data[32];
    while (_stream.available()) {
        size_t size = min(_stream.available(), 32);
        size = _stream.readBytes((uint8_t*)(&data), size);
        _codec.onMessage(*this, (uint8_t*)(&data), size);
    }

}

void SerialService::preDestroy() {

}

void SerialService::send(uint8_t msgId, const uint8_t *data, uint16_t size) {
    _codec.sendMessage(*this, serial::Message{msgId, size, data, crc16(data, size)});
}

void SerialService::onMessage(serial::SerialPort &port, serial::Message &message) {

}

const char* SerialService::deviceId() {
    return "serial";
}

int SerialService::send(const uint8_t *data, size_t size) {
    _stream.write(data, size);
    _stream.flush();
    return size;
}

uint16_t SerialService::crc16(const uint8_t *data, size_t size) {
    if (data == nullptr) {
        return 0;
    }

    etl::crc16 crc;
    crc.add(data, data + size);
    return crc.value();
}

void SerialService::onMessage(const uint8_t *data, size_t size) {
    _codec.onMessage(*this, data, size);
}

void SerialService::onConnect(serial::SerialPort &port) {
    etl::send_message(getRegistry().getMessageBus(), SerialConnected(*this));
}

void SerialService::onDisconnect(serial::SerialPort &port) {
    etl::send_message(getRegistry().getMessageBus(), SerialDisconnected(*this));
}
