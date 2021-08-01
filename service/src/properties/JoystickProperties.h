//
// Created by Ivan Kishchenko on 01.08.2021.
//

#ifndef ROVER_JOYSTICKPROPERTIES_H
#define ROVER_JOYSTICKPROPERTIES_H

#include "Properties.h"

struct JoystickProperties : Properties {
    std::string source = "/dev/input/js0";
    std::string mqttTopic = "/joystick";
};


#endif //ROVER_JOYSTICKPROPERTIES_H
