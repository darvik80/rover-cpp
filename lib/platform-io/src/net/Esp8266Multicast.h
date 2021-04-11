#ifdef ARDUINO_ARCH_ESP8266
//
// Created by Kishchenko, Ivan on 4/2/21.
//
#ifndef NET_ESP8266_MULTICAST
#define NET_ESP8266_MULTICAST


#include "net/Multicast.h"
#include <ESPAsyncUDP.h>

class Esp8266MulticastReceiver : public MulticastReceiver {
    AsyncUDP _udp;
    IPAddress _group;
    int _port;
public:
    explicit Esp8266MulticastReceiver(const char *group, int port);

    void receive(std::function<void(const std::string &, const SenderAddress &)> func) override;
};

#endif
#endif
