//
// Created by Ivan Kishchenko on 10.07.2021.
//

#ifndef PLATFORM_IO_WIFISERVICE_H
#define PLATFORM_IO_WIFISERVICE_H

#ifdef SUPPORT_ETHERNET

#include <Arduino.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <service/Service.h>
#include <Ticker.h>

class WifiService : public BaseService {
#ifdef ESP32
    Ticker _reconnectTimer;
#endif
#ifdef ESP8266
    WiFiEventHandler _handlerSoftApConnected;
    WiFiEventHandler _handlerSoftApDisconnected;
    WiFiEventHandler _handlerGotIp;
    WiFiEventHandler _handlerDisconnect;
#endif
public:
    explicit WifiService(Registry &registry);

    void postConstruct() override;

    void onWifiConnect();
#ifdef ESP32
    void onWifiEvent(WiFiEvent_t event, WiFiEventInfo_t info);
#endif
};

#endif

#endif //PLATFORM_IO_WIFISERVICE_H
