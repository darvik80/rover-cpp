//
// Created by Kishchenko, Ivan on 4/2/21.
//
#ifdef RASPBERRY_ARCH

#include <memory>

#include "BoostMulticastFactory.h"
#include "BoostMulticast.h"


std::shared_ptr<MulticastReceiver> BoostMulticastFactory::createReceiver(const char* group, int port) {
    return std::make_shared<BoostMulticastReceiver>(_service, group, port);
}

std::shared_ptr<MulticastSender> BoostMulticastFactory::createSender(const char* group, int port) {
    return std::make_shared<BoostMulticastSender>(_service, group, port);
}

#endif

