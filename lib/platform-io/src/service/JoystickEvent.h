//
// Created by Ivan Kishchenko on 04.09.2021.
//

#pragma once

#include "Const.h"
#include <etl/message.h>

struct JoystickAxis {
    int axisX{0};
    int axisY{0};
};

struct JoystickEvent : public etl::message<MSG_JOYSTICK> {
    JoystickAxis leftAxis;
    JoystickAxis rightAxis;
};

