//
// Created by Kishchenko, Ivan on 4/1/21.
//

#ifndef ROVER_BOOSTMULTICAST_H
#define ROVER_BOOSTMULTICAST_H

#include "Config.h"
#include "module/Multicast.h"
#include "module/raspberry/BoostMulticastMessage.h"

class BoostMulticastSender : public MulticastSender {
    boost::asio::ip::udp::socket _socket;
    boost::asio::ip::udp::endpoint _endpoint;
public:
    explicit BoostMulticastSender(boost::asio::io_service& service, std::string_view group, int port);
    std::future<void> multicast(std::string_view message) override;
};

class BoostMulticastReceiver : public MulticastReceiver {
    boost::asio::ip::udp::socket _socket;
    boost::asio::ip::udp::endpoint _senderEndpoint;
    std::array<char, 1024> _data{};
public:
    explicit BoostMulticastReceiver(boost::asio::io_service& service, std::string_view listen, std::string_view group, int port);
    void receive(std::function<void(std::string_view, const SenderAddress& address)> func) override;
};

#endif //ROVER_BOOSTMULTICAST_H
