//
// Created by Ivan Kishchenko on 31.07.2021.
//

#include "MqttService.h"


void MqttService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    _library.init();

    _client = _library.getFactory()->create(registry.getProperties<MqttProperties>());
}

void MqttService::preDestroy(Registry &registry) {
    _library.shutdown();
    BaseService::preDestroy(registry);
}
