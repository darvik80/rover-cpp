#ifdef ARDUINO_ARCH_AVR

//
// Created by Ivan Kishchenko on 03.04.2021.
//

#include "Kernel.h"
#include "CoreConfig.h"
#include <net/SerialTransport.h>

SerialTransport transport(Serial, 115200);


void Kernel::setup() {
    //Serial.begin(115200);
    //while (!Serial) { delay(100); }

    //Serial.println("Setup Wifi network");
}

void Kernel::loop() {
    transport.loop();
}

#endif