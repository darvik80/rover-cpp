//
// Created by Ivan Kishchenko on 01.05.2021.
//

#ifndef PLATFORM_IO_REGISTRY_H
#define PLATFORM_IO_REGISTRY_H

#include "Const.h"
#include <etl/message_bus.h>

class Registry {
public:
    virtual etl::imessage_bus& getMessageBus() = 0;
};


#endif //PLATFORM_IO_REGISTRY_H
