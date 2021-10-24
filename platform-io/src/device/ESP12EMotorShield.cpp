//
// Created by Ivan Kishchenko on 24.10.2021.
//

#ifdef ESP12E_DC_MOTOR_SHIELD

#include "ESP12EMotorShield.h"

ESP12EMotorShield::ESP12EMotorShield() {
    // Motor A
    pinMode(D1, OUTPUT);
    pinMode(D3, OUTPUT);

    // Motor B
    pinMode(D2, OUTPUT);
    pinMode(D4, OUTPUT);
}


const char *ESP12EMotorShield::name() {
    return "esp12-motor-shield";
}

void ESP12EMotorShield::move(DCMotor::Engine engine, DCMotor::Direction direction, int pulse) {
    if (engine == ENGINE_ONE) {
        analogWrite(D1, pulse);
        digitalWrite(D3, direction == DIR_FORWARD ? HIGH : LOW);
    }else if (engine == ENGINE_TWO) {
        analogWrite(D2, pulse);
        digitalWrite(D4, direction == DIR_FORWARD ? HIGH : LOW);
    }
}

#endif

