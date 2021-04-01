//
// Created by Kishchenko, Ivan on 4/1/21.
//

#ifndef ROVER_MULTICAST_H
#define ROVER_MULTICAST_H

#include <string>
#include <future>
#include <functional>

class SenderAddress {
    std::string host;
    int port;
public:
    SenderAddress(std::string_view host, int port) : host(host), port(port) {}

    [[nodiscard]] const std::string &getHost() const {
        return host;
    }

    [[nodiscard]] int getPort() const {
        return port;
    }
};

class MulticastSender {
public:
    virtual std::future<void> multicast(std::string_view message) = 0;
};

class MulticastReceiver {
public:
    virtual void receive(std::function<void(std::string_view, const SenderAddress& address)> func) = 0;
};

#endif //ROVER_MULTICAST_H
