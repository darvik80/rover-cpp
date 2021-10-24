//
// Created by Ivan Kishchenko on 29.08.2021.
//

#ifdef ESP8266

#include "ZeroMQService.h"
#include <ArduinoJson.hpp>
#include <service/JoystickEvent.h>

#include <network/handler/NetworkLogger.h>
#include <network/zeromq/ZeroMQHandler.h>
#include <network/zeromq/ZeroMQCodec.h>
#include <network/zeromq/ZeroMQLogging.h>

using namespace ArduinoJson;

ZeroMQService::ZeroMQService(Registry &registry)
        : BaseService(registry), message_router(ZERO_MQ_SERVICE) {
}

void ZeroMQService::postConstruct() {
    auto subscriber = std::make_shared<network::zeromq::ZeroMQSubscriber>();
    subscriber->subscribe("joystick", [this](std::string_view topic, std::string_view data) {
        if (topic == "joystick") {
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, data);

            JoystickEvent joystickEvent;
            auto axis = doc["leftAxis"];
            joystickEvent.leftAxis.axisX = axis["axisX"];
            joystickEvent.leftAxis.axisY = axis["axisY"];
            axis = doc["rightAxis"];
            joystickEvent.rightAxis.axisX = axis["axisX"];
            joystickEvent.rightAxis.axisY = axis["axisY"];

            etl::send_message(getRegistry().getMessageBus(), joystickEvent);
        }

        network::zeromq::log::info("sub: {}:{}", topic, data);
    });


    _server = new network::AsyncTcpServer([subscriber](const std::shared_ptr<network::AsyncChannel> &channel) {
        link(
                channel,
                //std::make_shared<network::handler::NetworkLogger>(),
                std::make_shared<network::zeromq::ZeroMQCodec>(),
                subscriber
        );
    });
    _server->start(5556);

    network::zeromq::log::info("service started");
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