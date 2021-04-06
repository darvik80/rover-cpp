//
// Created by Kishchenko, Ivan on 4/1/21.
//

#include "ArduinoMulticastMessage.h"

#ifdef SUPPORT_ETHERNET

void to_json(JsonDocument& doc, const MulticastMessage& message) {
    doc["id"] = message.id.c_str();
    doc["name"] = message.name.c_str();
    doc["type"] = message.type.c_str();
}

void from_json(const JsonDocument& doc, MulticastMessage& message) {
    message.id = (const char*)doc["id"];
    message.name = (const char*)doc["name"];
    message.type = (const char*)doc["type"];
}

#endif