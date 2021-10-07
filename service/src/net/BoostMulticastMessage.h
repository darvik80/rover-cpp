//
// Created by Kishchenko, Ivan on 4/1/21.
//
#pragma once

#ifdef RASPBERRY_ARCH

#include <nlohmann/json.hpp>
#include "net/Multicast.h"
#include "net/MulticastMessage.h"

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
