//
// Created by Ivan Kishchenko on 26.06.2021.
//

#ifndef PLATFORM_IO_SERVOMOTORSERVICE_H
#define PLATFORM_IO_SERVOMOTORSERVICE_H

#include <device/controller/IRRemoteButton.h>
#include <service/IRControlMessage.h>
#include "service/Service.h"

#include "device/motor/ServoMotor.h"
#include "JoystickEvent.h"


#define SERVO_STEP 10

class ServoMotorService : public BaseService, public etl::message_router<ServoMotorService, IRControlMessage, JoystickEvent> {
    int _angle = 90;
    int _lastAngle = 0;

    ServoMotor* _motor;
public:
    ServoMotorService(Registry &registry, ServoMotor* motor);

    void postConstruct() override;

    void on_receive(const IRControlMessage &msg);

    void on_receive(const JoystickEvent &msg);

    void on_receive_unknown(const etl::imessage &msg) {}



};


#endif //PLATFORM_IO_SERVOMOTORSERVICE_H
