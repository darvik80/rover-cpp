//
// Created by Ivan Kishchenko on 16.05.2021.
//

#include "SerialService.h"
#include "BoostSerialPort.h"

void SerialService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    auto& props = registry.getProperties<SerialProperties>();

    _serial = std::make_unique<BoostSerialPort>(registry.getIoService(), props, *this);
}

void SerialService::onMessage(serial::SerialPort &port, serial::Message &message) {
    logging::debug("[dev][evt] msgId: {}, msgSize: {}", message.msgId, message.len);
    if (message.msgId == serial::MSG_LOG) {
        logging::info("[dev] {}", std::string_view((const char*)message.data, message.len));
    }
}

void SerialService::onConnect(serial::SerialPort &port) {
    logging::info("{} connected", port.deviceId());
}

void SerialService::onDisconnect(serial::SerialPort &port) {
    logging::info("{} disconnected", port.deviceId());
}
