
#include "Arduino.h"

#ifndef MotorDriver_h
#define MotorDriver_h

// 8-bit bus after the 74HC595 shift register
// (not Arduino pins)
// These are used to set the direction of the bridge driver.
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR3_A 5
#define MOTOR3_B 7
#define MOTOR4_A 0
#define MOTOR4_B 6

#ifdef ARDUINO_AVR_UNO

#define MOTORLATCH 12
#define MOTORCLK 4
#define MOTORENABLE 7
#define MOTORDATA 8

// Arduino pins for the PWM signals.
#define MOTOR1_PWM 11
#define MOTOR2_PWM 3
#define MOTOR3_PWM 6
#define MOTOR4_PWM 5
#define SERVO1_PWM 10
#define SERVO2_PWM 9

#elif ESP8266

#define MOTORLATCH D6
#define MOTORCLK D2
#define MOTORENABLE D5
#define MOTORDATA D6

// ESP8266 pins for the PWM signals.
#define MOTOR1_PWM D7
#define MOTOR2_PWM D1
#define MOTOR3_PWM D4
#define MOTOR4_PWM D3

#endif

// Codes for the motor function.
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

class MotorDriver {

public:
    MotorDriver() = default;;

    void motor(int nMotor, int command, int speed);

private:
    void shiftWrite(int output, int high_low);

    void motorOutput(int output, int high_low, int speed);

};

#endif

