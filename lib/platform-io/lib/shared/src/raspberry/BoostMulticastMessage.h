//
// Created by Kishchenko, Ivan on 4/1/21.
//

#ifndef ROVER_BOOSTMULTICASTMESSAGE_H
#define ROVER_BOOSTMULTICASTMESSAGE_H

#ifdef PROFILE_RASPBERRY

#include <nlohmann/json.hpp>
#include <Multicast.h>

void to_json(nlohmann::json& j, const MulticastMessage& message);
void from_json(const nlohmann::json& j, MulticastMessage& message);

template<class C>
std::string toJson(const C &message) {
    nlohmann::json j;
    to_json(j, message);

    return j.dump();
}

template<class C>
C fromJson(std::string_view json) {
    nlohmann::json j = nlohmann::json::parse(json);
    return j.get<C>();
}

#endif

#endif //ROVER_BOOSTMULTICASTMESSAGE_H