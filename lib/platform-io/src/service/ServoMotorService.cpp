//
// Created by Ivan Kishchenko on 26.06.2021.
//

#include "ServoMotorService.h"

ServoMotorService::ServoMotorService(Registry &registry, ServoMotor *motor)
        : BaseService(registry), message_router(SERVO_MOTOR_SERVICE), _motor(motor) {
}

void ServoMotorService::postConstruct() {
    getRegistry().getMessageBus().subscribe(*this);

    Serial.println("Servo init");
    _motor->move(90);
}

void ServoMotorService::on_receive(const IRControlMessage &msg) {
    switch (msg.getCode()) {
        case BTN_LEFT:
            _angle -= SERVO_STEP;
            break;
        case BTN_RIGHT:
            _angle += SERVO_STEP;
            break;
        case BTN_2:
            _angle = 90;
            break;
        default:
            return;
    }

    if (_angle > 150) {
        _angle = 150;
    } else if (_angle < 30) {
        _angle = 30;
    }

    if (_lastAngle != _angle) {
        _motor->move(_angle);
        _lastAngle = _angle;
    }
}

void ServoMotorService::on_receive(const JoystickEvent &msg) {
    _angle = (msg.rightAxis.axisX*60/255)+90;
    //Serial.printf("Servo move: %d:%d\n", msg.rightAxis.axisX, _angle);

    if (_lastAngle != _angle) {
        _motor->move(_angle);
        _lastAngle = _angle;
    }
}
