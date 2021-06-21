//
// Created by Ivan Kishchenko on 01.05.2021.
//

#include "Application.h"
#include "serial/SerialService.h"

#include "device/MG90sServoMotor.h"
#include <MotorDriver.h>
#include <device/L293DMotorShield.h>

etl::message_bus<1> appMessageBus;

std::unique_ptr<ServoMotor> servoMotor;

MotorDriver m;
L293DMotorShield motorShield;

Application::Application()
        : message_router(ROUTER_APP) {
}

void Application::postConstruct() {
    servoMotor.reset(new SG90ServoMotor(10));
    Serial.begin(115200);
    _services.emplace_back(new SerialService(getRegistry(), Serial));
    appMessageBus.subscribe(*this);
    for (const auto &service : _services) {
        service->postConstruct();
    }

//    motor.move(90);
//    delay(5000);
//    motor.moveMicroseconds(544);
}

void Application::run() {
    for (const auto &service : _services) {
        service->run();
    }


    //motor.moveMicroseconds(544);
    servoMotor->move(45);
    //Serial.println(motor.read());
    delay(2000);
    //motor.moveMicroseconds(2400);
    servoMotor->move(135);
    //Serial.println(motor.status());
    delay(2000);
    m.motor(4, FORWARD,1024);

//    for (int pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
//        // in steps of 1 degree
//        motor.move(pos);              // tell servo to go to position in variable 'pos'
//        delay(15);                       // waits 15 ms for the servo to reach the position
//    }
//    for (int pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//        motor.move(pos);              // tell servo to go to position in variable 'pos'
//        delay(15);                       // waits 15 ms for the servo to reach the position
//    }
}

void Application::preDestroy() {
    for (const auto &service : _services) {
        service->preDestroy();
    }
    appMessageBus.unsubscribe(*this);
}

void Application::on_receive(etl::imessage_router &source, const SerialConnected &msg) {
    //msg.getService().send(0x03, "Hello World!");
}

void Application::on_receive(etl::imessage_router &source, const SerialDisconnected &msg) {

}

void Application::on_receive_unknown(etl::imessage_router &source, const etl::imessage &msg) {

}

etl::imessage_bus &Application::getMessageBus() {
    return appMessageBus;
}

Registry &Application::getRegistry() {
    return *this;
}
