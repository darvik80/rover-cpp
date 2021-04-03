//
// Created by Kishchenko, Ivan on 4/2/21.
//
#ifdef ARDUINO_ARCH_ESP8266

#include "net/MulticastFactory.h"

#include <memory>

#include "EspMulticast.h"

std::shared_ptr<MulticastReceiver> MulticastFactory::createReceiver(const Context& context, const char* group, int port) {
    return std::make_shared<EspMulticastReceiver>(group, port);
}

std::shared_ptr<MulticastSender> MulticastFactory::createSender(const Context& context, const char* group, int port) {
    return std::shared_ptr<MulticastSender>();
}

#endif

