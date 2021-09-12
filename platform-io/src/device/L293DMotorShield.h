//
// Created by Ivan Kishchenko on 21.06.2021.
//

#ifndef PLATFORM_IO_L293DMOTORSHIELD_H
#define PLATFORM_IO_L293DMOTORSHIELD_H

#ifdef L293D_DC_MOTOR_SHIELD

#ifndef DC_MOTOR
#define DC_MOTOR
#endif

#include "device/motor/DCMotor.h"
#include <MotorDriver.h>

class L293DMotorShield : public DCMotor {
    MotorDriver _motor;
public:
    const char *name() override;

    void move(Engine engine, Direction direction, int pulse) override;
};

#endif

#endif //PLATFORM_IO_L293DMOTORSHIELD_H
