//
// Created by Ivan Kishchenko on 10.07.2021.
//

#include "WifiService.h"
#include "WifiMessage.h"

using namespace std::placeholders;

void onTimer(WifiService *service) {
    service->onWifiConnect();
}

void onConnect(const WiFiEventStationModeGotIP &event) {
    Serial.println("Connected to Wi-Fi.");
}

WifiService::WifiService(Registry &registry)
        : BaseService(registry) {}

void WifiService::postConstruct() {
#ifdef ESP8266
    _handlerSoftApConnected = WiFi.onSoftAPModeStationConnected([this](const WiFiEventSoftAPModeStationConnected& event) {
       etl::send_message(getRegistry().getMessageBus(), WifiMessageConnected{});
    });

    _handlerGotIp = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP &event) {
        Serial.println("Got IP address: ");
        Serial.println(event.ip);
        etl::send_message(getRegistry().getMessageBus(), WifiMessageConnected{});
    });
    _handlerDisconnect = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected &event) {
        Serial.println("WiFi lost connection");
        etl::send_message(getRegistry().getMessageBus(), WifiMessageDisconnected{});
    });
    WiFi.setAutoReconnect(true);
    onWifiConnect();
#else
    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
        onWifiEvent(event, info);
    }, SYSTEM_EVENT_STA_GOT_IP);

    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
        onWifiEvent(event, info);
    }, SYSTEM_EVENT_STA_DISCONNECTED);
    _reconnectTimer.once_ms(2000, ::onTimer, this);
#endif

}

/* Put IP Address details */
IPAddress localIp(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

#define WIFI_SSID "ED7-6"
#define WIFI_AP_SSID "rover"
#define WIFI_PASS "1234554321"

void WifiService::onWifiConnect() {
    Serial.println("Init Wi-Fi...");
//    WiFi.mode(WIFI_AP);
//    WiFi.softAPConfig(localIp, gateway, subnet);
//    WiFi.softAP(WIFI_AP_SSID, WIFI_PASS);
//
//    Serial.println("SoftAP started");
//    Serial.println(WiFi.softAPIP());
//
//    etl::send_message(getRegistry().getMessageBus(), WifiMessageConnected{});
    WiFi.begin(WIFI_SSID, WIFI_PASS);
}

#ifdef ESP32
void WifiService::onWifiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
    switch (event) {
        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.println("WiFi connected");
            Serial.print("Got IP address: ");
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
#endif