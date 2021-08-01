//
// Created by Ivan Kishchenko on 31.07.2021.
//

#ifndef ROVER_MQTTSERVICE_H
#define ROVER_MQTTSERVICE_H

#include "BaseService.h"
#include <mqtt/Library.h>
#include <joystick/JoystickEvent.h>
#include <event/EventHandler.h>

class MqttService :public BaseServiceShared<MqttService>, public em::TEventHandler<xbox::Xbox380Event> {
    mqtt::Library _library;
    mqtt::Client::Ptr _client;

    std::unique_ptr<mqtt::Publisher> _xboxPublisher;
public:
    const char *name() override {
        return "mqtt";
    }

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry &registry) override;

    void onEvent(const xbox::Xbox380Event &event) override;
};


#endif //ROVER_MQTTSERVICE_H
