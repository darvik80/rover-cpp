//
// Created by Ivan Kishchenko on 01.05.2021.
//

#ifndef PLATFORM_IO_APPLICATION_H
#define PLATFORM_IO_APPLICATION_H

#include "service/Service.h"
#include "serial/SerialMessage.h"

#include <etl/message_bus.h>
#include <etl/vector.h>
#include <device/Device.h>

class Application : public Service, public Registry
        , public etl::message_router<Application, SerialConnected, SerialDisconnected> {
public:
    explicit Application();

    Registry &getRegistry() override;

    void postConstruct() override;

    void run() override;

    void preDestroy() override;

    etl::imessage_bus &getMessageBus() override;

    ~Application() override = default;

public:
    void on_receive(const SerialConnected &msg);

    void on_receive(const SerialDisconnected &msg);

    void on_receive_unknown(const etl::imessage &msg);

private:
    etl::vector<Service *, 3> _services{};
};


#endif //PLATFORM_IO_APPLICATION_H
