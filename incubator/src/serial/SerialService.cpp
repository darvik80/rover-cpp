//
// Created by Ivan Kishchenko on 02.05.2021.
//

#include "SerialService.h"

void SerialService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    auto& props = registry.getProperties<SerialProperties>();

    _serial = std::make_unique<BoostSerialPort>(registry.getIoService(), props.port, props.baudRate);

    _serial->setOnConnected([this]() {
        _connected = false;
    });

    _serial->setOnIdle([this]() {
        if (!_connected) {
            _serial->send(serial::MSG_SYNC, nullptr, 0);
        } else {
            _serial->send(serial::MSG_PING, nullptr, 0);
        }
    });

    _serial->setOnMessage([this](uint8_t msgId, const uint8_t *data, size_t size) {
        if (msgId == serial::MSG_SYNC) {
            _serial->send(serial::MSG_CONN, nullptr, 0);
        } else if (msgId == serial::MSG_CONN) {
            info("Connected!");
            _connected = true;
        } else if (msgId == serial::MSG_PONG) {
            debug("RECV PING/PONG");
        } else {
            info(std::string((const char*)data, size));
        }
    });
}
