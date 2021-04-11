//
// Created by Kishchenko, Ivan on 4/2/21.
//
#ifdef ARDUINO_ARCH_ESP8266

#include "KernelMulticastFactory.h"
#include "Esp8266Multicast.h"

#include <memory>

KernelMulticastFactory KernelMulticastFactory::_instance;

KernelMulticastFactory& KernelMulticastFactory::instance() {
    return _instance;
}

std::shared_ptr<MulticastReceiver> KernelMulticastFactory::createReceiver(const char* group, int port) {
#ifdef ARDUINO_ARCH_ESP8266
    return std::make_shared<Esp8266MulticastReceiver>(group, port);
#else
    return std::shared_ptr<MulticastReceiver>();
#endif
}

std::shared_ptr<MulticastSender> KernelMulticastFactory::createSender(const char* group, int port) {
    return std::shared_ptr<MulticastSender>();
}

#endif

