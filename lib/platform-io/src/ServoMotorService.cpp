//
// Created by Ivan Kishchenko on 26.06.2021.
//

#include "ServoMotorService.h"

ServoMotorService::ServoMotorService(Registry &registry, ServoMotor *motor)
        : BaseService(registry), message_router(SERVO_MOTOR_SERVICE), _motor(motor) {
}

void ServoMotorService::on_receive(etl::imessage_router &source, const IRControlMessage &msg) {
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

void ServoMotorService::postConstruct() {
    getRegistry().getMessageBus().subscribe(*this);
}
