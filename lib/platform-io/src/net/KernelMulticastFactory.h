//
// Created by Ivan Kishchenko on 09.04.2021.
//

#ifndef PLATFORM_IO_ESP8266_MULTICASTFACTORY_H
#define PLATFORM_IO_ESP8266_MULTICASTFACTORY_H


#include <CoreContext.h>
#include <net/Multicast.h>
#include <net/MulticastFactory.h>
#include "Esp8266Multicast.h"

#ifdef ARDUINO_ARCH_ESP8266

class KernelMulticastFactory : public MulticastFactory {
private:
    static KernelMulticastFactory _instance;
    KernelMulticastFactory() = default;
    KernelMulticastFactory(KernelMulticastFactory&) {}
public:
    static KernelMulticastFactory& instance();
    std::shared_ptr<MulticastReceiver> createReceiver(const char* group, int port) override;
    std::shared_ptr<MulticastSender> createSender(const char* group, int port) override;

};

#endif

#endif //PLATFORM_IO_MULTICASTFACTORY_H
