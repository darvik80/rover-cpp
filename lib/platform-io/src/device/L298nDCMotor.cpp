//
// Created by Ivan Kishchenko on 03.04.2021.
//

#include "L298nDCMotor.h"

#ifdef L298N_DC_MOTOR

const char *L298nDCMotor::name() {
    return "l298n";
}

L298nDCMotor::L298nDCMotor(const L298nDCMotor::Pins &left, const L298nDCMotor::Pins &right) {
    _pins[ENGINE_LEFT] = left;
    _pins[ENGINE_RIGHT] = right;
    setup(_pins[ENGINE_LEFT]);
    setup(_pins[ENGINE_RIGHT]);
}

void L298nDCMotor::move(Engine engine, DCMotor::Direction direction, int pulse) {
    if (engine == ENGINE_BOTH) {
        move(_pins[ENGINE_LEFT], direction, pulse);
        move(_pins[ENGINE_RIGHT], direction, pulse);
    } else {
        move(_pins[engine], direction, pulse);
    }
}

void L298nDCMotor::setup(Pins &pins) {
    pinMode(pins.pinEn, OUTPUT);
    pinMode(pins.pintIn1, OUTPUT);
    pinMode(pins.pintIn2, OUTPUT);
}

void L298nDCMotor::move(Pins &pins, Direction direction, int pulse) {
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