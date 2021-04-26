//
// Created by Ivan Kishchenko on 03.04.2021.
//

#ifndef PLATFORM_IO_ESP8266KERNEL_H
#define PLATFORM_IO_ESP8266KERNEL_H

#include <net/MulticastFactory.h>

class Kernel {
protected:
    static MulticastFactory& multicastFactory();
public:
    void setup();
    void loop();
};


#endif //PLATFORM_IO_ESP8266KERNEL_H
