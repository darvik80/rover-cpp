//
// Created by Ivan Kishchenko on 01.05.2021.
//

#ifndef PLATFORM_IO_APPLICATION_H
#define PLATFORM_IO_APPLICATION_H

#include <CoreConfig.h>
#include "Service.h"
#include "serial/SerialMessage.h"

#include <etl/message_bus.h>
#include <etl/vector.h>

enum {
    ROUTER_APP = 1
};

class Application : public Service, public Registry, public etl::message_router<Application, SerialConnected, SerialDisconnected> {
public:
    explicit Application();

    Registry &getRegistry() override;

    void postConstruct() override;

    void run() override;

    void preDestroy() override;

    etl::imessage_bus &getMessageBus() override;

    ~Application() override = default;

public:
    void on_receive(etl::imessage_router &source, const SerialConnected &msg);

    void on_receive(etl::imessage_router &source, const SerialDisconnected &msg);

    void on_receive_unknown(etl::imessage_router &source, const etl::imessage &msg);

private:
    etl::vector<Service *, 10> _services;
};


#endif //PLATFORM_IO_APPLICATION_H
