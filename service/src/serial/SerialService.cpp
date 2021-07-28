//
// Created by Ivan Kishchenko on 16.05.2021.
//

#include "SerialService.h"
#include "BoostSerialPort.h"
#include "SerialEvent.h"

void SerialService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    _eventManager = registry.getService<EventManagerService>().shared_from_this();
    auto& props = registry.getProperties<SerialProperties>();

    _serial = std::make_unique<BoostSerialPort>(registry.getIoService(), props, *this);
}

void SerialService::onMessage(serial::SerialPort &port, serial::Message &msg) {
    logging::debug("[{}][msg] msgId: {}, msgSize: {}", port.deviceId(), msg.msgId, msg.len);
    switch (msg.msgId ) {
        case serial::MSG_LOG:
            logging::info("[{}] {}", port.deviceId(), std::string_view((const char*)msg.data, msg.len));
            break;
        case serial::MSG_PING:
        case serial::MSG_PONG:
            logging::info("[{}] ping/pong", port.deviceId());
            break;
        default:
            _eventManager->raiseEvent(SerialEvent{shared_from_this(), msg.msgId, msg.data, msg.len});
    }

}

void SerialService::onConnect(serial::SerialPort &port) {
    logging::info("[{}] connected", port.deviceId());
}

void SerialService::onDisconnect(serial::SerialPort &port) {
    logging::info("[{}] disconnected", port.deviceId());
}
