//
// Created by Kishchenko, Ivan on 4/2/21.
//

#ifndef PLATFORM_IO_MULTICASTFACTORY_H
#define PLATFORM_IO_MULTICASTFACTORY_H

#include "Multicast.h"
#include "Context.h"
#include <memory>

class MulticastFactory {
public:
    static std::shared_ptr<MulticastReceiver> createReceiver(const Context& context, const char* group, int port);
    static std::shared_ptr<MulticastSender> createSender(const Context& context, const char* group, int port);
};

#endif //PLATFORM_IO_MULTICASTFACTORY_H
