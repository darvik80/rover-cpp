#ifdef ARDUINO_ARCH_ESP32

//
// Created by Ivan Kishchenko on 03.04.2021.
//

#include "Kernel.h"


#include <WiFi.h>

#include "CoreContext.h"

const char* ssid = "plc";
const char* password = "1234554321";

CoreContext context;

void Kernel::setup() {
    Serial.begin(115200);

    Serial.println("Setup Wifi network");
    if (WiFi.softAP(ssid, password)) {
        Serial.println("Wifi configured");
    }

    context.macAddress = WiFi.macAddress().c_str();
}

void Kernel::loop() {

}

#endif