//
// Created by Ivan Kishchenko on 24.10.2021.
//

#pragma once

#ifdef ESP12E_DC_MOTOR_SHIELD

#ifndef DC_MOTOR
#define DC_MOTOR
#endif

#include <MotorDriver.h>
#include "device/motor/DCMotor.h"

class ESP12EMotorShield  : public DCMotor {
public:
    ESP12EMotorShield();

    const char *name() override;

    void move(Engine engine, Direction direction, int pulse) override;

};

#endif


