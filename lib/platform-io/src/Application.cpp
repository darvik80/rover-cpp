//
// Created by Ivan Kishchenko on 01.05.2021.
//

#include <service/zeromq/ZeroMQService.h>
#include "Application.h"
#include "service/wifi/WifiService.h"
#include "service/mqtt/MqttService.h"
#include "service/serial/SerialService.h"

#ifdef ARDUINO_ARCH_AVR
#include "service/IRControllerService.h"
#endif

#ifdef L293D_DC_MOTOR_SHIELD

#include "service/DCMotorService.h"
#include "device/L293DMotorShield.h"

#endif

#ifdef MG90S_SERVO_MOTOR

#include "device/MG90sServoMotor.h"
#include "service/ServoMotorService.h"

#endif

etl::message_bus<4> appMessageBus;

Application::Application()
        : message_router(ROUTER_APP) {
}

void Application::postConstruct() {
    Serial.begin(115200);
#if defined ESP8266 || ESP32
    _services.emplace_back(new WifiService(getRegistry()));
    _services.emplace_back(new ZeroMQService(getRegistry()));
    //_services.emplace_back(new MqttService(getRegistry()));
#endif

#ifdef ARDUINO_ARCH_AVR
    _services.emplace_back(new IRControllerService(getRegistry(), new HX1838IRRemote(11)));
#endif

#ifdef L293D_DC_MOTOR_SHIELD
    _services.emplace_back(new DCMotorService(getRegistry(), new L293DMotorShield()));
#endif

#ifdef MG90S_SERVO_MOTOR
    _services.emplace_back(new ServoMotorService(getRegistry(), new MG90sServoMotor(D8)));
#endif
    appMessageBus.subscribe(*this);

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

void Application::on_receive(const SerialConnected &msg) {
}

void Application::on_receive(const SerialDisconnected &msg) {

}

void Application::on_receive_unknown(const etl::imessage &msg) {

}

etl::imessage_bus &Application::getMessageBus() {
    return appMessageBus;
}

Registry &Application::getRegistry() {
    return *this;
}
