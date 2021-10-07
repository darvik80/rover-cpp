//
// Created by Ivan Kishchenko on 01.08.2021.
//

#pragma once

#include "Properties.h"

struct JoystickProperties : Properties {
    std::string source = "/dev/input/js0";
    std::string mqttTopic = "/joystick";
};
