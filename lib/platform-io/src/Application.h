//
// Created by Ivan Kishchenko on 01.05.2021.
//

#ifndef PLATFORM_IO_APPLICATION_H
#define PLATFORM_IO_APPLICATION_H

#include <CoreConfig.h>
#include "Service.h"
#include "serial/SerialMessage.h"
#include "control/IRControlMessage.h"

#include <etl/message_bus.h>
#include <etl/vector.h>
#include <device/Device.h>

class Application : public Service, public Registry
        , public etl::message_router<Application, SerialConnected, SerialDisconnected, IRControlMessage> {
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
    void on_receive(etl::imessage_router &source, const IRControlMessage &msg);

    void on_receive_unknown(etl::imessage_router &source, const etl::imessage &msg);

private:
    etl::vector<Service *, 4> _services{};
    DeviceManager* _deviceManager{nullptr};

private:
    void controlServo(Button code);
    void controlMotor(Button code);
};


#endif //PLATFORM_IO_APPLICATION_H
