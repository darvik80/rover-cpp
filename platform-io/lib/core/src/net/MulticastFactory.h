//
// Created by Kishchenko, Ivan on 4/2/21.
//

#ifndef PLATFORM_IO_MULTICASTFACTORY_H
#define PLATFORM_IO_MULTICASTFACTORY_H

#ifdef SUPPORT_ETHERNET

#include "net/Multicast.h"
#include <memory>

class MulticastFactory {
public:
    virtual std::shared_ptr<MulticastReceiver> createReceiver(const char* group, int port) = 0;
    virtual std::shared_ptr<MulticastSender> createSender(const char* group, int port) = 0;
};

#endif

#endif //PLATFORM_IO_MULTICASTFACTORY_H
