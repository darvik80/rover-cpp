//
// Created by Ivan Kishchenko on 02.05.2021.
//

#include "SerialService.h"

void SerialService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    auto& props = registry.getProperties<SerialProperties>();

    _serial = std::make_unique<>(registry.getIoService(), props.port, props.baudRate);
}
