//
// Created by Ivan Kishchenko on 10.07.2021.
//

#include <etl/message_router.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include "MqttService.h"

MqttService::MqttService(Registry &registry)
        : BaseService(registry), message_router(MQTT_SERVICE) {
}

void onTimer(MqttService *service) {
    service->onTimer();
}

void MqttService::postConstruct() {
    BaseService::postConstruct();
    getRegistry().getMessageBus().subscribe(*this);

    _mqttClient.onConnect([this](bool sessionPresent) {
        onMqttConnect(sessionPresent);
    });
    _mqttClient.onDisconnect([this](AsyncMqttClientDisconnectReason reason) {
        onMqttDisconnect(reason);
    });
//   _mqttClient.onSubscribe(onMqttSubscribe);
//    _mqttClient.onUnsubscribe(onMqttUnsubscribe);
    _mqttClient.onMessage([this](char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
        onMqttMessage(topic, payload, properties, len, index, total);
    });
//    _mqttClient.onPublish(onMqttPublish);
    _mqttClient.setClientId("esp32");
    _mqttClient.setServer(MQTT_HOST, MQTT_PORT);
}

void MqttService::onTimer() {
    Serial.println("Connecting to MQTT...");
    _mqttClient.connect();
}

void MqttService::onMqttConnect(bool sessionPresent) {

    Serial.println("Connected to MQTT.");
    Serial.print("Session present: ");
    Serial.println(sessionPresent);
    uint16_t packetIdSub = _mqttClient.subscribe("/system/event", 2);
    Serial.print("Subscribing at QoS 2, packetId: ");
    Serial.println(packetIdSub);
//    mqttClient.publish("test/lol", 0, true, "test 1");
//    Serial.println("Publishing at QoS 0");
//    uint16_t packetIdPub1 = mqttClient.publish("test/lol", 1, true, "test 2");
//    Serial.print("Publishing at QoS 1, packetId: ");
//    Serial.println(packetIdPub1);
//    uint16_t packetIdPub2 = mqttClient.publish("test/lol", 2, true, "test 3");
//    Serial.print("Publishing at QoS 2, packetId: ");
//    Serial.println(packetIdPub2);
}

void MqttService::onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    Serial.print("OnMessage: ");
    Serial.println(topic);
    Serial.print("payload: ");
    Serial.println(len);

    Serial.print("index: ");
    Serial.print(index);
    Serial.print(", total: ");
    Serial.println(total);
}

void MqttService::onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    Serial.println("Disconnected from MQTT.");

    if (WiFi.isConnected()) {
        //xTimerStart(_reconnectTimer, 0);
    }
}

void MqttService::on_receive(const WifiMessageConnected &msg) {
    _reconnectTimer.once_ms(2000, ::onTimer, this);
}

void MqttService::on_receive(const WifiMessageDisconnected &msg) {

}

void MqttService::on_receive_unknown(const etl::imessage &msg) {}
