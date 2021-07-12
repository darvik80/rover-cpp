//
// Created by Ivan Kishchenko on 10.07.2021.
//

#ifndef PLATFORM_IO_WIFISERVICE_H
#define PLATFORM_IO_WIFISERVICE_H

#include <Arduino.h>
#include <WiFi.h>

#include <service/Service.h>
#include <Ticker.h>

#define WIFI_SSID "ED7-6"
#define WIFI_PASS "1234554321"

class WifiService : public BaseService {
    Ticker _reconnectTimer;
public:
    explicit WifiService(Registry &registry);

    void postConstruct() override;

    void onWifiConnect();
    void onWifiEvent(WiFiEvent_t event, WiFiEventInfo_t info);

};


#endif //PLATFORM_IO_WIFISERVICE_H
