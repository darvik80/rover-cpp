//
// Created by Ivan Kishchenko on 03.04.2021.
//

#ifndef PLATFORM_IO_DEVICE_H
#define PLATFORM_IO_DEVICE_H

#include "CoreConfig.h"

class Device {
public:
    virtual const char* name() = 0;
};

class DeviceManager {
public:

public:
    virtual void registerDevice(unique_ptr<Device> device) = 0;

    virtual Device* getDevice(const char* name) const = 0;
};

#endif //PLATFORM_IO_DEVICE_H
