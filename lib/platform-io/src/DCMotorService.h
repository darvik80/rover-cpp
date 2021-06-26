//
// Created by Ivan Kishchenko on 26.06.2021.
//

#ifndef PLATFORM_IO_DCMOTORSERVICE_H
#define PLATFORM_IO_DCMOTORSERVICE_H

#include <device/controller/IRRemoteButton.h>
#include <control/IRControlMessage.h>
#include "Service.h"

#include "device/L293DMotorShield.h"

class DCMotorService : public BaseService, public etl::message_router<DCMotorService, IRControlMessage> {
    int _pos = 6;
    int _lastPos = 0;

    DCMotor *_motor{nullptr};

public:
    DCMotorService(Registry &registry, DCMotor *motor);

    void postConstruct() override;

    void on_receive(etl::imessage_router &source, const IRControlMessage &msg);

    void on_receive_unknown(etl::imessage_router &source, const etl::imessage &msg) {}
};


#endif //PLATFORM_IO_DCMOTORSERVICE_H
