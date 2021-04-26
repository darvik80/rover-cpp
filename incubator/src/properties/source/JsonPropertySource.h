//
// Created by Ivan Kishchenko on 11.04.2021.
//

#ifndef ROVER_JSONPROPERTYSOURCE_H
#define ROVER_JSONPROPERTYSOURCE_H

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
};


#endif //ROVER_JSONPROPERTYSOURCE_H
