//
// Created by Kishchenko, Ivan on 4/1/21.
//

#include "BoostMulticastMessage.h"

void to_json(nlohmann::json& j, const MulticastMessage& message) {
    j = nlohmann::json{{"id", message.id}, {"name", message.name}, {"type", message.type}};
}

void from_json(const nlohmann::json& j, MulticastMessage& message) {
    j.at("id").get_to(message.id);
    j.at("name").get_to(message.name);
    j.at("type").get_to(message.type);
}

