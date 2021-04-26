//
// Created by Ivan Kishchenko on 03.04.2021.
//

#ifndef PLATFORM_IO_DCMOTORMOVECOMMAND_H
#define PLATFORM_IO_DCMOTORMOVECOMMAND_H

#include "Command.h"
#include "device/motor/DCMotor.h"

class DCMotorMoveCommand : public Command {
    DCMotor& _motor;
    DCMotor::Engine _engine;
    DCMotor::Direction _direction;
    int pulse;
public:
    DCMotorMoveCommand(DCMotor &motor, DCMotor::Engine engine, DCMotor::Direction direction, int pulse);

    int execute() override;
};


#endif //PLATFORM_IO_DCMOTORMOVECOMMAND_H
