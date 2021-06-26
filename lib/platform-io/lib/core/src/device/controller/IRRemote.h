//
// Created by Ivan Kishchenko on 26.06.2021.
//

#ifndef PLATFORM_IO_IRREMOTE_H
#define PLATFORM_IO_IRREMOTE_H

#ifdef IR_REMOTE

#include "device/Device.h"
#include "IRRemoteButton.h"

class IRRemote : public Device {
public:
    DeviceType type() override {
        return DeviceType::Remote;
    }

    virtual Button decode() = 0;
};

#endif

#endif //PLATFORM_IO_IRREMOTE_H
