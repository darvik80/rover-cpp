//
// Created by Ivan Kishchenko on 03.04.2021.
//

#ifndef PLATFORM_IO_DCMOTOR_H
#define PLATFORM_IO_DCMOTOR_H

#include "device/Device.h"

class DCMotor : public Device {
public:
    enum Direction {
        DIR_FORWARD,
        DIR_BACKWARD,
        DIR_LEFT,
        DIR_RIGHT,
        DIR_UP,
        DIR_DOWN
    };

    enum Engine {
        ENGINE_LEFT,
        ENGINE_RIGHT,
        ENGINE_BOTH,
    };

public:
    virtual void move(Engine engine, Direction direction, int pulse) = 0;
};


#endif //PLATFORM_IO_DCMOTOR_H
