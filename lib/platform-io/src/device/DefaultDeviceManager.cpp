//
// Created by Ivan Kishchenko on 26.06.2021.
//

#include <device/motor/ServoMotor.h>
#include "DefaultDeviceManager.h"

void DefaultDeviceManager::registerDevice(Device* device) {
    _devices.emplace_back(device);
}

Device *DefaultDeviceManager::getDevice(const char *name) const {
    for (auto& item : _devices) {
        if (strcmp(item->name(), name) == 0) {
            return item;
        }
    }
    return nullptr;
}

DefaultDeviceManager::~DefaultDeviceManager() {
    for (auto& item : _devices) {
        delete item;
    }
}
