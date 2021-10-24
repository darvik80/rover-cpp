//
// Created by Ivan Kishchenko on 26.06.2021.
//

#ifndef PLATFORM_IO_DCMOTORSERVICE_H
#define PLATFORM_IO_DCMOTORSERVICE_H

#include "logging/Logging.h"

#include <device/motor/DCMotor.h>
#include <device/controller/IRRemoteButton.h>
#include <service/IRControlMessage.h>
#include "service/Service.h"
#include "JoystickEvent.h"

class DCMotorService : public BaseService, public etl::message_router<DCMotorService, IRControlMessage, JoystickEvent> {
    int _pos = 6;
    int _lastPos = 0;

    DCMotor *_motor{nullptr};

public:
    DCMotorService(Registry &registry, DCMotor *motor);

    void postConstruct() override;

    void on_receive(const IRControlMessage &msg);

    void on_receive(const JoystickEvent &msg);

    void on_receive_unknown(const etl::imessage &msg) {}
};


#endif //PLATFORM_IO_DCMOTORSERVICE_H
