//
// Created by Ivan Kishchenko on 26.06.2021.
//

#include "DCMotorService.h"

int SPEED_INDEX[]{-255, -224, -192, -128, -96, -64, 0, 64, 96, 128, 192, 224, 255};

DCMotorService::DCMotorService(Registry &registry, DCMotor *motor)
        : BaseService(registry), message_router(DC_MOTOR_SERVICE), _motor(motor) {
}

void DCMotorService::on_receive(const IRControlMessage &msg) {
    switch (msg.getCode()) {
        case BTN_ENTER:
            if (_pos < 6) {
                _pos++;
            } else if (_pos > 6) {
                _pos--;
            }
            break;
        case BTN_UP:
            if (++_pos > 12) {
                _pos = 12;
            }
            break;
        case BTN_DOWN:
            if (--_pos < 0) {
                _pos = 0;
            }
            break;
        default:
            return;
    }
    if (_lastPos != _pos) {
        _motor->move(
                DCMotor::ENGINE_FOUR,
                SPEED_INDEX[_pos] > 0 ? DCMotor::DIR_FORWARD : DCMotor::DIR_BACKWARD,
                abs(SPEED_INDEX[_pos])
        );
        _lastPos = _pos;
    }
}

void DCMotorService::on_receive(const JoystickEvent &msg) {
    int value = abs(msg.rightAxis.axisY);
    if (value < 128) {
        value = 0;
    } else if (value > 255) {
        value = 255;
    }

    _motor->move(
            DCMotor::Engine::ENGINE_ONE,
            msg.rightAxis.axisY < 0 ? DCMotor::Direction::DIR_FORWARD : DCMotor::Direction::DIR_BACKWARD,
            value
    );

    value = abs(msg.leftAxis.axisY);
    if (value < 128) {
        value = 0;
    } else if (value > 255) {
        value = 255;
    }

    _motor->move(
            DCMotor::Engine::ENGINE_TWO,
            msg.leftAxis.axisY < 0 ? DCMotor::Direction::DIR_FORWARD : DCMotor::Direction::DIR_BACKWARD,
            value
    );
}

void DCMotorService::postConstruct() {
    getRegistry().getMessageBus().subscribe(*this);
    logging::info("DC init");
}

