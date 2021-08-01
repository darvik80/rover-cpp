//
// Created by Ivan Kishchenko on 31.07.2021.
//

#include "MqttService.h"
#include "event/EventManagerService.h"


void MqttService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    _library.init();

    _client = _library.getFactory()->create(registry.getProperties<MqttProperties>());

    registry.getService<EventManagerService>().subscribe<xbox::Xbox380Event>(shared_from_this());

    const auto xboxProps = registry.getProperties<JoystickProperties>();
    _xboxPublisher = std::make_unique<mqtt::Publisher>(_client->publisher(xboxProps.mqttTopic, 0));
}

void MqttService::preDestroy(Registry &registry) {
    _library.shutdown();
    BaseService::preDestroy(registry);
}

void MqttService::onEvent(const xbox::Xbox380Event &event) {
    nlohmann::json json;
    nlohmann::to_json(json, event);
    _xboxPublisher->publish(json.dump());
};
