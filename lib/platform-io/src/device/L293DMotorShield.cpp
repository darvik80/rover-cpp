//
// Created by Ivan Kishchenko on 21.06.2021.
//

#include "L293DMotorShield.h"

#ifdef L293D_DC_MOTOR_SHIELD

const char *L293DMotorShield::name() {
    return "l293d";
}

void L293DMotorShield::move(Engine engine, Direction direction, int pulse) {
    _motor.motor(engine, direction, pulse);
}

#endif