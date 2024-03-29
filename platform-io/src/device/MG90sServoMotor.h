//
// Created by Ivan Kishchenko on 20.06.2021.
//

#ifndef PLATFORM_IO_MG90SSERVOMOTOR_H
#define PLATFORM_IO_MG90SSERVOMOTOR_H

#if defined ARDUINO_ARCH_AVR || ARDUINO_ARCH_ESP8266

#if defined MG90S_SERVO_MOTOR || defined SG90_SERVO_MOTOR

#ifndef SERVO_MOTOR
    #define SERVO_MOTOR
#endif

#include "device/motor/ServoMotor.h"
#include <Arduino.h>
#include <Servo.h>

class MG90sServoMotor : public ServoMotor {
    Servo _servo;
public:
    explicit MG90sServoMotor() = default;

    explicit MG90sServoMotor(int pwmPin) {
        attach(pwmPin);
    }

    void attach(int pin) {
        _servo.attach(pin);
    }

    const char *name() override {
        return "mg90s";
    }

    void move(int angle) override {
        _servo.write(angle);
    }

    int status() override {
        return _servo.read();
    }

    void moveMicroseconds(int microSec) override {
        return _servo.writeMicroseconds(microSec);
    }
};

#ifdef SG90_SERVO_MOTOR

class SG90ServoMotor : public MG90sServoMotor {
public:
    const char *name() override {
        return "sg90";
    }
};

#endif

#endif

#endif

#endif //PLATFORM_IO_MG90SSERVOMOTOR_H
