//
// Created by Ivan Kishchenko on 03.04.2021.
//

#ifndef PLATFORM_IO_DEVICE_H
#define PLATFORM_IO_DEVICE_H

enum DeviceType {
    DC_Motor,
    Servo_Motor,
    Remote,
};

class Device {
public:
    virtual DeviceType type() = 0;
    virtual const char* name() = 0;

    virtual ~Device() = default;
};

class DeviceManager {
public:

public:
    virtual void registerDevice(Device* device) = 0;

    virtual Device* getDevice(const char* name) const = 0;
};

#endif //PLATFORM_IO_DEVICE_H
