//
// Created by Ivan Kishchenko on 01.05.2021.
//

#include "Application.h"
#include <device/DefaultDeviceManager.h>
#include "serial/SerialService.h"
#include "control/IRControllerService.h"

#include "device/MG90sServoMotor.h"
#include <device/HX1838IRRemote.h>
#include <device/L293DMotorShield.h>

etl::message_bus<1> appMessageBus;

Application::Application()
        : message_router(ROUTER_APP) {
}

void Application::postConstruct() {
    Serial.begin(115200);
    appMessageBus.subscribe(*this);

    _deviceManager = new DefaultDeviceManager();
    _deviceManager->registerDevice(new SG90ServoMotor(10));
    _deviceManager->registerDevice(new L293DMotorShield());
    _services.emplace_back(new SerialService(getRegistry(), Serial));
    _services.emplace_back(new IRControllerService(getRegistry()));

    for (const auto &service : _services) {
        service->postConstruct();
    }
}

void Application::run() {
    for (const auto &service : _services) {
        service->run();
    }
}

void Application::preDestroy() {
    for (const auto &service : _services) {
        service->preDestroy();
    }
    appMessageBus.unsubscribe(*this);
}

void Application::on_receive(etl::imessage_router &source, const SerialConnected &msg) {
    msg.getService().send(serial::MSG_LOG, "Hello World!");
}

void Application::on_receive(etl::imessage_router &source, const SerialDisconnected &msg) {

}

void Application::controlServo(Button code) {
    auto *motor = (ServoMotor *) _deviceManager->getDevice("mg90s");
    if (motor != nullptr) {
        switch (code) {
            case BTN_LEFT:
                motor->move(45);
                break;
            case BTN_RIGHT:
                motor->move(135);
                break;
            default:
                break;
        }
    }
}

int pos = 7;
int speedIndex[] = { -255, -128, -64, -32, -16, -8, -4, 0, 4, 8, 16, 32, 64, 128, 255 };
int speed, lastSpeed = 0;

void Application::controlMotor(Button code) {

    auto *motor = (DCMotor *) _deviceManager->getDevice("l293D");
    if (motor != nullptr) {
        switch (code) {
            case BTN_ENTER:
                if (pos < 7) {
                    pos++;
                } else if (pos > 7){
                    pos--;
                }
                break;
            case BTN_UP:
                pos += 1;
                if (pos > 14) {
                    pos = 14;
                }
                break;
            case BTN_DOWN:
                pos -= 1;
                if (pos < 0) {
                    pos = 0;
                }
                break;
            default:
                break;
        }

        speed = speedIndex[pos];

        if (lastSpeed != speed) {
            Serial.print("Move: ");
            if (speed > 0) {
                Serial.print("DIR_FORWARD");
            } else {
                Serial.print("DIR_BACKWARD");
            }
            Serial.print(" ");
            Serial.println(abs(speed));
            motor->move(
                    DCMotor::ENGINE_FOUR,
                    speed > 0 ? DCMotor::DIR_FORWARD : DCMotor::DIR_BACKWARD,
                    abs(speed)
            );
            lastSpeed = speed;
        }
    }
}

void Application::on_receive(etl::imessage_router &source, const IRControlMessage &msg) {
    auto code = msg.getCode();
    if (code != BTN_NONE) {
        Serial.print("handle remote: ");
        Serial.println(code);
    }

    if (code == Button::BTN_LEFT || code == Button::BTN_RIGHT) {
        controlServo(code);
    } else {
        controlMotor(code);
    }
}

void Application::on_receive_unknown(etl::imessage_router &source, const etl::imessage &msg) {
}

etl::imessage_bus &Application::getMessageBus() {
    return appMessageBus;
}

Registry &Application::getRegistry() {
    return *this;
}
