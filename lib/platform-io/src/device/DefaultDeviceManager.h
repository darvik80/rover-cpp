//
// Created by Ivan Kishchenko on 26.06.2021.
//

#ifndef PLATFORM_IO_DEFAULTDEVICEMANAGER_H
#define PLATFORM_IO_DEFAULTDEVICEMANAGER_H

#include <device/Device.h>
#include <CoreConfig.h>
#include <etl/vector.h>

#include "Service.h"

class DefaultDeviceManager : public DeviceManager {
    etl::vector<Device*, 5> _devices;
public:
    void registerDevice(Device* device) override;
    Device* getDevice(const char* name) const override;

    virtual ~DefaultDeviceManager();
};


#endif //PLATFORM_IO_DEFAULTDEVICEMANAGER_H
