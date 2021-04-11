//
// Created by Kishchenko, Ivan on 4/1/21.
//

#ifndef ROVER_BOOSTMULTICAST_H
#define ROVER_BOOSTMULTICAST_H

#ifdef RASPBERRY_ARCH

#include "Properties.h"
#include "BoostMulticastMessage.h"
#include <net/Multicast.h>
#include <boost/asio.hpp>

class BoostMulticastSender : public MulticastSender {
    boost::asio::ip::udp::socket _socket;
    boost::asio::ip::udp::endpoint _endpoint;
public:
    explicit BoostMulticastSender(boost::asio::io_service& service, const char* group, int port);
    std::future<void> multicast(const std::string& message) override;
};

class BoostMulticastReceiver : public MulticastReceiver {
    boost::asio::ip::udp::socket _socket;
    boost::asio::ip::udp::endpoint _senderEndpoint;
    std::array<char, 1024> _data{};
public:
    explicit BoostMulticastReceiver(boost::asio::io_service& service, const char* group, int port);
    void receive(std::function<void(const std::string&, const SenderAddress& address)> func) override;
};

#endif

#endif //ROVER_BOOSTMULTICAST_H
