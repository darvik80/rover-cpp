//
// Created by Ivan Kishchenko on 03.04.2021.
//

#ifndef PLATFORM_IO_L298NDCMOTOR_H
#define PLATFORM_IO_L298NDCMOTOR_H

#ifdef L298N_DC_MOTOR

#ifndef DC_MOTOR
#define DC_MOTOR
#endif

#include "device/motor/DCMotor.h"
#ifdef ARDUINO_ARCH_AVR
#include <stdint.h>
#else
#include <cstdint>
#endif

class L298nDCMotor : public DCMotor {
public:
    struct Pins {
        Pins(int pinEn, int pintIn1, int pintIn2)
                : pinEn(pinEn), pintIn1(pintIn1), pintIn2(pintIn2) {
        }

        Pins()
                : pinEn(0), pintIn1(0), pintIn2(0) {}

        uint8_t pinEn;
        uint8_t pintIn1;
        uint8_t pintIn2;
    };

private:
    Pins _pins[2];
public:
    L298nDCMotor(const Pins &left, const Pins &right);

    const char *name() override;

    void move(Engine engine, Direction direction, int pulse) override;

private:
    static void setup(Pins &pins);

    static void move(Pins &pins, Direction direction, int pulse);
};

#endif

#endif //PLATFORM_IO_L298NDCMOTOR_H
