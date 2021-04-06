//
// Created by Ivan Kishchenko on 03.04.2021.
//

#include "Kernel.h"

#ifdef ARDUINO_ARCH_ESP8266

#include <ESP8266WiFi.h>
#include "SSD1306Wire.h"
SSD1306Wire  display(0x3c, SDA, SCL);

#include "net/Context.h"
#include "net/Multicast.h"
#include "net/MulticastFactory.h"
#include "net/arduino/ArduinoMulticastMessage.h"

const char* ssid = "plc";
const char* password = "1234554321";

Context context;
std::shared_ptr<MulticastReceiver> receiver;

// Adapted from Adafruit_SSD1306
void drawLines() {
    for (int16_t i=0; i<display.getWidth(); i+=4) {
        display.drawLine(0, 0, i, display.getHeight()-1);
        display.display();
        delay(10);
    }
    for (int16_t i=0; i<display.getHeight(); i+=4) {
        display.drawLine(0, 0, display.getWidth()-1, i);
        display.display();
        delay(10);
    }
    delay(250);

    display.clear();
    for (int16_t i=0; i<display.getWidth(); i+=4) {
        display.drawLine(0, display.getHeight()-1, i, 0);
        display.display();
        delay(10);
    }
    for (int16_t i=display.getHeight()-1; i>=0; i-=4) {
        display.drawLine(0, display.getHeight()-1, display.getWidth()-1, i);
        display.display();
        delay(10);
    }
    delay(250);

    display.clear();
    for (int16_t i=display.getWidth()-1; i>=0; i-=4) {
        display.drawLine(display.getWidth()-1, display.getHeight()-1, i, 0);
        display.display();
        delay(10);
    }
    for (int16_t i=display.getHeight()-1; i>=0; i-=4) {
        display.drawLine(display.getWidth()-1, display.getHeight()-1, 0, i);
        display.display();
        delay(10);
    }
    delay(250);
    display.clear();
    for (int16_t i=0; i<display.getHeight(); i+=4) {
        display.drawLine(display.getWidth()-1, 0, 0, i);
        display.display();
        delay(10);
    }
    for (int16_t i=0; i<display.getWidth(); i+=4) {
        display.drawLine(display.getWidth()-1, 0, i, display.getHeight()-1);
        display.display();
        delay(10);
    }
    delay(250);
}

// Adapted from Adafruit_SSD1306
void drawRect(void) {
    for (int16_t i=0; i<display.getHeight()/2; i+=2) {
        display.drawRect(i, i, display.getWidth()-2*i, display.getHeight()-2*i);
        display.display();
        delay(10);
    }
}

// Adapted from Adafruit_SSD1306
void fillRect(void) {
    uint8_t color = 1;
    for (int16_t i=0; i<display.getHeight()/2; i+=3) {
        display.setColor((color % 2 == 0) ? BLACK : WHITE); // alternate colors
        display.fillRect(i, i, display.getWidth() - i*2, display.getHeight() - i*2);
        display.display();
        delay(10);
        color++;
    }
    // Reset back to WHITE
    display.setColor(WHITE);
}

// Adapted from Adafruit_SSD1306
void drawCircle(void) {
    for (int16_t i=0; i<display.getHeight(); i+=2) {
        display.drawCircle(display.getWidth()/2, display.getHeight()/2, i);
        display.display();
        delay(10);
    }
    delay(1000);
    display.clear();

    // This will draw the part of the circel in quadrant 1
    // Quadrants are numberd like this:
    //   0010 | 0001
    //  ------|-----
    //   0100 | 1000
    //
    display.drawCircleQuads(display.getWidth()/2, display.getHeight()/2, display.getHeight()/4, 0b00000001);
    display.display();
    delay(200);
    display.drawCircleQuads(display.getWidth()/2, display.getHeight()/2, display.getHeight()/4, 0b00000011);
    display.display();
    delay(200);
    display.drawCircleQuads(display.getWidth()/2, display.getHeight()/2, display.getHeight()/4, 0b00000111);
    display.display();
    delay(200);
    display.drawCircleQuads(display.getWidth()/2, display.getHeight()/2, display.getHeight()/4, 0b00001111);
    display.display();
}

void printBuffer(void) {
    // Initialize the log buffer
    // allocate memory to store 8 lines of text and 30 chars per line.
    display.setLogBuffer(5, 30);

    // Some test data
    const char* test[] = {
            "Hello",
            "World" ,
            "----",
            "Show off",
            "how",
            "the log buffer",
            "is",
            "working.",
            "Even",
            "scrolling is",
            "working"
    };

    for (uint8_t i = 0; i < 11; i++) {
        display.clear();
        // Print to the screen
        display.println(test[i]);
        // Draw it to the internal screen buffer
        display.drawLogBuffer(0, 0);
        // Display it on the screen
        display.display();
        delay(500);
    }
}

void Kernel::setup() {
    Serial.begin(115200);
    //while (!Serial) { delay(100); }

    Serial.println("Setup Wifi network");
    if (WiFi.softAP(ssid, password)) {
        Serial.println("Wifi configured");
    }

    display.init();

    display.flipScreenVertically();

    display.setContrast(255);

    drawLines();
    delay(1000);
    display.clear();

    drawRect();
    delay(1000);
    display.clear();

    fillRect();
    delay(1000);
    display.clear();

    drawCircle();
    delay(1000);
    display.clear();

    printBuffer();
    delay(1000);
    display.clear();

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