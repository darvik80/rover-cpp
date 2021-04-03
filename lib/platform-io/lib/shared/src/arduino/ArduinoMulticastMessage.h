//
// Created by Kishchenko, Ivan on 4/1/21.
//

#ifndef ROVER_BOOSTMULTICASTMESSAGE_H
#define ROVER_BOOSTMULTICASTMESSAGE_H

#include <Multicast.h>
#undef ARDUINOJSON_ENABLE_STD_STRING
#define ARDUINOJSON_ENABLE_STD_STRING 1
#include <ArduinoJson.h>

void to_json(JsonDocument& doc, const MulticastMessage& message);
void from_json(const JsonDocument& doc, MulticastMessage& message);

template<class C>
void toJson(const C &message, std::string& res) {
    DynamicJsonDocument doc(1024);
    to_json(doc, message);
    serializeJson(doc, res);
}

template<class C>
bool fromJson(const char* json, C& res) {
    DynamicJsonDocument doc(1024);
    auto err = deserializeJson(doc, json);
    if (err) {
        return false;
    }

    from_json(doc, res);
    return true;
}

#endif //ROVER_BOOSTMULTICASTMESSAGE_H