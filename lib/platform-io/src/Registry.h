//
// Created by Ivan Kishchenko on 01.05.2021.
//

#ifndef PLATFORM_IO_REGISTRY_H
#define PLATFORM_IO_REGISTRY_H

#include "CoreConfig.h"
#include <etl/message_bus.h>

enum {
    ROUTER_APP = 1,
    DC_MOTOR_SERVICE,
    SERVO_MOTOR_SERVICE,
};

enum {
    MSG_SERIAL_CONNECTED,
    MSG_SERIAL_DISCONNECTED,
    MSG_SERIAL_MESSAGE,

    MSG_IR_CONTROL,
};

class Registry {
public:
    virtual etl::imessage_bus& getMessageBus() = 0;
};


#endif //PLATFORM_IO_REGISTRY_H
