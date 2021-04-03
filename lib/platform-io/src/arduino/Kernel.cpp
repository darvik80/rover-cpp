//
// Created by Ivan Kishchenko on 03.04.2021.
//

#include "Kernel.h"

#ifdef ARDUINO_ARCH_AVR

void Kernel::setup() {
    Serial.begin(115200);
    //while (!Serial) { delay(100); }

    Serial.println("Setup Wifi network");
}

void Kernel::loop() {

}

#endif