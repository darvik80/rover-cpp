//
// Created by Ivan Kishchenko on 03.04.2021.
//

#include "Kernel.h"

#ifdef ARDUINO_ARCH_ESP8266

#include <ESP8266WiFi.h>

#include "net/Context.h"
#include "net/Multicast.h"
#include "net/MulticastFactory.h"
#include "net/arduino/ArduinoMulticastMessage.h"

const char* ssid = "plc";
const char* password = "1234554321";

Context context;
std::shared_ptr<MulticastReceiver> receiver;

void Kernel::setup() {
    Serial.begin(115200);
    //while (!Serial) { delay(100); }

    Serial.println("Setup Wifi network");
    if (WiFi.softAP(ssid, password)) {
        Serial.println("Wifi configured");
    }

    context.macAddress = WiFi.macAddress().c_str();

    receiver = MulticastFactory::createReceiver(context, "239.255.0.1", 12345);
    Serial.println("Multicast receiver created");

    receiver->receive([](const std::string& message, const SenderAddress& address) {
        MulticastMessage msg;
        fromJson<MulticastMessage>(message.c_str(), msg);

        std::string test;
        toJson<MulticastMessage>(msg, test);
        Serial.println(test.c_str());

        Serial.printf("receive from: %s:%d\r\n", address.getHost().c_str(), address.getPort());
        Serial.printf("id: %s, name: %s, type: %s\r\n", msg.id.c_str(), msg.name.c_str(), msg.type.c_str());
        Serial.println("----");
    });
}

void Kernel::loop() {

}

#endif