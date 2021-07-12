//
// Created by Ivan Kishchenko on 10.07.2021.
//

#ifndef PLATFORM_IO_MQTTSERVICE_H
#define PLATFORM_IO_MQTTSERVICE_H

#include <service/Service.h>
#include "service/wifi/WifiMessage.h"
#include <AsyncMqttClient.h>

#include <Ticker.h>

#define MQTT_HOST IPAddress(192, 168, 100, 3)
#define MQTT_PORT 1883

class MqttService : public BaseService, public etl::message_router<MqttService, WifiMessageConnected, WifiMessageDisconnected> {
    Ticker _reconnectTimer;
    AsyncMqttClient _mqttClient;
public:
    explicit MqttService(Registry &registry);

    void onTimer();

    void postConstruct() override;

    void onMqttConnect(bool sessionPresent);

    void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);

    void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);

    void on_receive(const WifiMessageConnected &msg);

    void on_receive(const WifiMessageDisconnected &msg);

    void on_receive_unknown(const etl::imessage &msg);
};


#endif //PLATFORM_IO_MQTTSERVICE_H
