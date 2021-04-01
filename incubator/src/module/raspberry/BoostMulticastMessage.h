//
// Created by Kishchenko, Ivan on 4/1/21.
//

#ifndef ROVER_BOOSTMULTICASTMESSAGE_H
#define ROVER_BOOSTMULTICASTMESSAGE_H

#include "module/MulticastMessage.h"
#include <nlohmann/json.hpp>

void to_json(nlohmann::json& j, const MulticastMessage& message);
void from_json(const nlohmann::json& j, MulticastMessage& message);

template<class C>
std::string to_json(const C &message) {
    nlohmann::json j;
    to_json(j, message);

    return j.dump();
}

template<class C>
C from_json(std::string_view json) {
    nlohmann::json j = nlohmann::json::parse(json);
    return j.get<C>();
}


#endif //ROVER_BOOSTMULTICASTMESSAGE_H
