//
// Created by Ivan Kishchenko on 01.05.2021.
//

#include "Application.h"
#include "service/wifi/WifiService.h"
#include "service/mqtt/MqttService.h"
#include "service/serial/SerialService.h"

#ifdef ARDUINO_ARCH_AVR
#include "service/IRControllerService.h"
#include "service/DCMotorService.h"
#include "service/ServoMotorService.h"

#include "device/HX1838IRRemote.h"
#include "device/L293DMotorShield.h"
#include "device/MG90sServoMotor.h"
#endif


etl::message_bus<3> appMessageBus;

Application::Application()
        : message_router(ROUTER_APP) {
}

void Application::postConstruct() {
    Serial.begin(115200);

    _services.emplace_back(new MqttService(getRegistry()));
    _services.emplace_back(new WifiService(getRegistry()));
#ifdef ARDUINO_ARCH_AVR
    _services.emplace_back(new IRControllerService(getRegistry(), new HX1838IRRemote(11)));
    _services.emplace_back(new DCMotorService(getRegistry(), new L293DMotorShield()));
    _services.emplace_back(new ServoMotorService(getRegistry(), new MG90sServoMotor(10)));
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
