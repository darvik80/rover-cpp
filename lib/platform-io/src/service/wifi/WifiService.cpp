//
// Created by Ivan Kishchenko on 10.07.2021.
//

#include "WifiService.h"
#include "WifiMessage.h"

using namespace std::placeholders;

void onTimer(WifiService *service) {
    service->onWifiConnect();
}

WifiService::WifiService(Registry &registry)
        : BaseService(registry) {}

void WifiService::postConstruct() {
    BaseService::postConstruct();

    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
        onWifiEvent(event, info);
    }, SYSTEM_EVENT_STA_GOT_IP);

    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
        onWifiEvent(event, info);
    }, SYSTEM_EVENT_STA_DISCONNECTED);

    _reconnectTimer.once_ms(2000, ::onTimer, this);
}

void WifiService::onWifiConnect() {
    Serial.println("Connecting to Wi-Fi...");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
}

void WifiService::onWifiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
    switch (event) {
        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.println("WiFi connected");
            Serial.println("Got IP address: ");
            Serial.println(WiFi.localIP());
            etl::send_message(getRegistry().getMessageBus(), WifiMessageConnected{});
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("WiFi lost connection");
            _reconnectTimer.once_ms(2000, ::onTimer, this);
            etl::send_message(getRegistry().getMessageBus(), WifiMessageDisconnected{});
            break;
        default:
            break;
    }
}
