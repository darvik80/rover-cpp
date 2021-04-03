//
// Created by Kishchenko, Ivan on 4/2/21.
//
#ifdef ARDUINO_ARCH_ESP8266

#include "EspMulticast.h"

EspMulticastReceiver::EspMulticastReceiver(const char* group, int port)
        : _port(port) {
    _group.fromString(group);
}

void EspMulticastReceiver::receive(std::function<void(const std::string &, const SenderAddress &)> func) {
    _udp.onPacket([this, func](AsyncUDPPacket &packet) {
        std::string data((const char*)packet.data(), packet.length());
        SenderAddress addr(packet.remoteIP().toString().c_str(), packet.remotePort());
        func(data, addr);
    });
    _udp.listenMulticast(_group, _port);
}

#endif