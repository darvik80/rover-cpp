//
// Created by Kishchenko, Ivan on 4/1/21.
//

#ifndef ROVER_MULTICAST_H
#define ROVER_MULTICAST_H

#ifdef SUPPORT_ETHERNET

#include <string>
#include <future>
#include <functional>
#include <utility>
#include "net/MulticastMessage.h"

class SenderAddress {
    std::string host;
    int port;
public:
    SenderAddress(const std::string& host, int port) : host(host), port(port) {}

    [[nodiscard]] const std::string &getHost() const {
        return host;
    }

    [[nodiscard]] int getPort() const {
        return port;
    }
};

class MulticastSender {
public:
    virtual std::future<void> multicast(const std::string& message) = 0;
};

class MulticastReceiver {
public:
    virtual void receive(std::function<void(const std::string&, const SenderAddress& address)> func) = 0;
};

#endif

#endif //ROVER_MULTICAST_H
