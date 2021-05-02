//
// Created by Ivan Kishchenko on 01.05.2021.
//

#include "Application.h"
#include "serial/SerialService.h"

etl::message_bus<1> appMessageBus;

Application::Application()
        : message_router(ROUTER_APP) {
}

void Application::postConstruct() {
    Serial.begin(115200);
    _services.emplace_back(new SerialService(getRegistry(), Serial));
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

void Application::on_receive(etl::imessage_router &source, const SerialConnected &msg) {
    msg.getService().send(0x03, "Hello World!");
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
