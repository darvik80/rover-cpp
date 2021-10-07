//
// Created by Ivan Kishchenko on 11.04.2021.
//

#pragma once

#include "PropertySource.h"
#include <nlohmann/json.hpp>
#include <fstream>

class JsonPropertySource : public PropertySource {
    nlohmann::json _json;
public:
    explicit JsonPropertySource(std::string_view source);
    explicit JsonPropertySource(std::ifstream& stream) {
        stream >> _json;
    }

    void getProperties(NetworkProperties& props) override;
    void getProperties(LoggingProperties &props) override;
    void getProperties(HttpProperties &props) override;
    void getProperties(GrpcProperties &props) override;
    void getProperties(SerialProperties &props) override;
    void getProperties(MqttProperties &props) override;
    void getProperties(JoystickProperties &props) override;
};
