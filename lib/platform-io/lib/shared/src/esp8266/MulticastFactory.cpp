//
// Created by Kishchenko, Ivan on 4/2/21.
//

#include "MulticastFactory.h"

#include <memory>

#include "esp8266/EspMulticast.h"

std::shared_ptr<MulticastReceiver> MulticastFactory::createReceiver(const Context& context, const char* group, int port) {
    return std::make_shared<EspMulticastReceiver>(group, port);
}

std::shared_ptr<MulticastSender> MulticastFactory::createSender(const Context& context, const char* group, int port) {
    return std::shared_ptr<MulticastSender>();
}

