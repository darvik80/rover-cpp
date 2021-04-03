//
// Created by Kishchenko, Ivan on 4/2/21.
//

#include <memory>

#include "MulticastFactory.h"
#include "BoostMulticast.h"

#ifdef PROFILE_RASPBERRY

std::shared_ptr<MulticastReceiver> MulticastFactory::createReceiver(const Context& context, const char* group, int port) {
    return std::make_shared<BoostMulticastReceiver>(*context.service, group, port);
}

std::shared_ptr<MulticastSender> MulticastFactory::createSender(const Context& context, const char* group, int port) {
    return std::make_shared<BoostMulticastSender>(*context.service, group, port);
}

#endif

