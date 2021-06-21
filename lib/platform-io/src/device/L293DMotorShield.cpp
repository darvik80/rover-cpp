//
// Created by Ivan Kishchenko on 21.06.2021.
//

#include "L293DMotorShield.h"

#ifdef L293D_DC_MOTOR_SHIELD

const char *L293DMotorShield::name() {
    return "l293D";
}

L293DMotorShield::L293DMotorShield() {
    //_pins[ENGINE_ONE] = left;
    //_pins[ENGINE_TWO] = right;
    setup(_pins[ENGINE_ONE]);
    setup(_pins[ENGINE_TWO]);
}

void L293DMotorShield::move(Engine engine, DCMotor::Direction direction, int pulse) {
    if (engine == ENGINE_ALL) {
        move(_pins[ENGINE_ONE], direction, pulse);
        move(_pins[ENGINE_TWO], direction, pulse);
    } else {
        move(_pins[engine], direction, pulse);
    }
}

void L293DMotorShield::setup(Pins &pins) {
    pinMode(pins.pinEn, OUTPUT);
    pinMode(pins.pintIn1, OUTPUT);
    pinMode(pins.pintIn2, OUTPUT);
}

void L293DMotorShield::move(Pins &pins, Direction direction, int pulse) {
    if (direction == DIR_FORWARD) {
        digitalWrite(pins.pintIn1, HIGH);
        digitalWrite(pins.pintIn2, LOW);
    } else {
        digitalWrite(pins.pintIn1, LOW);
        digitalWrite(pins.pintIn2, HIGH);
    }

    digitalWrite(pins.pinEn, pulse);
}

#endif