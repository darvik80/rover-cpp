//
// Created by Ivan Kishchenko on 20.06.2021.
//

#ifndef PLATFORM_IO_SERVOMOTOR_H
#define PLATFORM_IO_SERVOMOTOR_H

#include "device/Device.h"

class ServoMotor : public Device {
public:
    DeviceType type() override {
        return DeviceType::Servo_Motor;
    }

    virtual void move(int angle) = 0;
    virtual int status() = 0;

    virtual void moveMicroseconds(int microSec) = 0;
};

#endif //PLATFORM_IO_SERVOMOTOR_H
