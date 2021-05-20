//
// Created by Kishchenko, Ivan on 5/20/21.
//

#ifndef PLATFORM_IO_DISPLAY_H
#define PLATFORM_IO_DISPLAY_H

#include "device/Device.h"

class Display : public Device {
public:
    Display() : Device(DEVICE_DISPLAY) {}

    virtual void setCursor(uint16_t x, uint16_t y) = 0;
    virtual void setTextColor(uint16_t c, uint16_t bg) = 0;
    virtual void print(const char* text) = 0;
};


#endif //PLATFORM_IO_DISPLAY_H
