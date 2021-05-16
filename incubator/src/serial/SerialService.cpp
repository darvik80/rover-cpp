//
// Created by Ivan Kishchenko on 16.05.2021.
//

#include "SerialService.h"
#include "BoostSerialPort.h"

void SerialService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    auto& props = registry.getProperties<SerialProperties>();

    _serial = std::make_unique<BoostSerialPort>(registry.getIoService(), props);
}
