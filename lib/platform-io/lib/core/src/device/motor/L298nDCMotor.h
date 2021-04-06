//
// Created by Ivan Kishchenko on 03.04.2021.
//

#ifndef PLATFORM_IO_L298NDCMOTOR_H
#define PLATFORM_IO_L298NDCMOTOR_H

#include "DCMotor.h"

class L298nDCMotor : public DCMotor {
public:
    const char *name() override;

    void move(Engine engine, Direction direction, int pulse) override;
};


#endif //PLATFORM_IO_L298NDCMOTOR_H
