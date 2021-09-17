//
// Created by Ivan Kishchenko on 29.08.2021.
//

#ifdef ESP8266

#include "ZeroMQService.h"
#include <zeromq/ZeroMQUtils.h>
#include <ArduinoJson.hpp>
#include <service/JoystickEvent.h>

using namespace ArduinoJson;

ZeroMQService::ZeroMQService(Registry &registry)
        : BaseService(registry), message_router(ZERO_MQ_SERVICE) {
    ZeroMQUtils::init();
}

void ZeroMQService::postConstruct() {
    Serial.println("ZeroMQ init");
    getRegistry().getMessageBus().subscribe(*this);

    etl::send_message(getRegistry().getMessageBus(), JoystickEvent{});
    _server.onTopicEvent([this](const ZeroMQTopicEvent& event) {
        if (event.topic == "joystick") {
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, event.data);

            JoystickEvent joystickEvent;
            auto axis = doc["leftAxis"];
            joystickEvent.leftAxis.axisX = axis["axisX"];
            joystickEvent.leftAxis.axisY = axis["axisY"];
            axis = doc["rightAxis"];
            joystickEvent.rightAxis.axisX = axis["axisX"];
            joystickEvent.rightAxis.axisY = axis["axisY"];

            etl::send_message(getRegistry().getMessageBus(), joystickEvent);
        }
    });
    _server.start(5556);
}

void ZeroMQService::on_receive(const WifiMessageConnected &msg) {
}

void ZeroMQService::on_receive(const WifiMessageDisconnected &msg) {
    //_server.shutdown();
    etl::send_message(getRegistry().getMessageBus(), JoystickEvent{});
}

void ZeroMQService::on_receive(const WifiClientMessageClientDisconnected &msg) {
    etl::send_message(getRegistry().getMessageBus(), JoystickEvent{});
}

#endif