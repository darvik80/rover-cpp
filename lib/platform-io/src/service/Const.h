//
// Created by Ivan Kishchenko on 03.07.2021.
//

#ifndef PLATFORM_IO_CONST_H
#define PLATFORM_IO_CONST_H

#ifdef ARDUINO_ARCH_AVR

#define ETL_NO_STL
#define ETL_NO_CPP_NAN_SUPPORT 1
#define ETL_NO_HUGE_VAL_SUPPORT

#include <etl/profiles/arduino_arm.h>

#endif

#include <Arduino.h>

enum {
    ROUTER_APP = 1,
    MQTT_SERVICE,
    ZERO_MQ_SERVICE,
    DC_MOTOR_SERVICE,
    SERVO_MOTOR_SERVICE,
};

enum {
    MSG_SERIAL_CONNECTED,
    MSG_SERIAL_DISCONNECTED,
    MSG_SERIAL_MESSAGE,

    MSG_IR_CONTROL,

    MSG_WIFI_CONNECTED,
    MSG_WIFI_DISCONNECTED,
};

#endif //PLATFORM_IO_CONST_H
