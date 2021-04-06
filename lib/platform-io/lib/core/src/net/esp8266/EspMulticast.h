//
// Created by Kishchenko, Ivan on 4/2/21.
//
#ifndef NET_ESP_MULTICAST
#define NET_ESP_MULTICAST

#ifdef ARDUINO_ARCH_ESP8266

#include "net/Multicast.h"
#include <ESPAsyncUDP.h>

class EspMulticastReceiver : public MulticastReceiver {
    AsyncUDP _udp;
    IPAddress _group;
    int _port;
public:
    explicit EspMulticastReceiver(const char *group, int port);

    void receive(std::function<void(const std::string &, const SenderAddress &)> func) override;
};

#endif
#endif
