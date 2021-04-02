//
// Created by Kishchenko, Ivan on 4/2/21.
//

#ifndef PLATFORM_IO_ESPMULTICAST_H
#define PLATFORM_IO_ESPMULTICAST_H

#include "Multicast.h"
#include <ESPAsyncUDP.h>

class EspMulticastReceiver : public MulticastReceiver {
    AsyncUDP _udp;
    IPAddress _group;
    int _port;
public:
    explicit EspMulticastReceiver(const char *group, int port);

    void receive(std::function<void(const std::string &, const SenderAddress &)> func) override;
};

#endif //PLATFORM_IO_ESPMULTICAST_H
