//
// Created by Ivan Kishchenko on 31.07.2021.
//

#ifndef ROVER_MQTTSERVICE_H
#define ROVER_MQTTSERVICE_H

#include "BaseService.h"
#include <mqtt/Library.h>

class MqttService :public BaseServiceShared<MqttService>{
    mqtt::Library _library;
    mqtt::Client::Ptr _client;
public:
    const char *name() override {
        return "mqtt";
    }

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry &registry) override;
};


#endif //ROVER_MQTTSERVICE_H
