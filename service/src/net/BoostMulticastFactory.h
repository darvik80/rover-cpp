//
// Created by Ivan Kishchenko on 09.04.2021.
//

#ifndef ROVER_BOOSTMULTICASTFACTORY_H
#define ROVER_BOOSTMULTICASTFACTORY_H

#include <net/MulticastFactory.h>
#include <boost/asio.hpp>

class BoostMulticastFactory : public MulticastFactory {
    boost::asio::io_service& _service;
public:
    explicit BoostMulticastFactory(boost::asio::io_service& service) : _service(service) {}
    std::shared_ptr<MulticastReceiver> createReceiver(const char* group, int port) override;
    std::shared_ptr<MulticastSender> createSender(const char* group, int port) override;
};


#endif //ROVER_BOOSTMULTICASTFACTORY_H
