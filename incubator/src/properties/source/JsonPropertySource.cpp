//
// Created by Ivan Kishchenko on 11.04.2021.
//

#include "JsonPropertySource.h"
#include "properties/NetworkProperties.h"

using namespace nlohmann;

JsonPropertySource::JsonPropertySource(std::string_view source) {
    _json = json::parse(source);
}

void JsonPropertySource::getProperties(NetworkProperties &props) {
    if (auto it = _json.find("network"); it != _json.end()) {
        it->at("registry_host").get_to(props.registryHost);
        it->at("registry_port").get_to(props.registryPort);
    }

    throw std::invalid_argument(std::string("The properties has not been found ") + typeid(props).name());
}
