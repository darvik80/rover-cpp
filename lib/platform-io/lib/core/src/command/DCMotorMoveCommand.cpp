//
// Created by Ivan Kishchenko on 03.04.2021.
//

#include "DCMotorMoveCommand.h"

DCMotorMoveCommand::DCMotorMoveCommand(DCMotor &motor, DCMotor::Engine engine, DCMotor::Direction direction, int pulse)
        : _motor(motor), _engine(engine), _direction(direction), pulse(pulse) {}

int DCMotorMoveCommand::execute() {
    _motor.move(_engine, _direction, pulse);

    return 0;
}
