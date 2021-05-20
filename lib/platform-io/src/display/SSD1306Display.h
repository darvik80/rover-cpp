//
// Created by Kishchenko, Ivan on 5/20/21.
//

#ifndef PLATFORM_IO_SSD1306DISPLAY_H
#define PLATFORM_IO_SSD1306DISPLAY_H

#include "device/display/Display.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

class SSD1306Display : public Display {
    Adafruit_SSD1306 _display;
public:
    SSD1306Display();

    const char *name() override;

    void setCursor(uint16_t x, uint16_t y) override;

    void setTextColor(uint16_t c, uint16_t bg) override;

    void print(const char *text) override;
};


#endif //PLATFORM_IO_SSD1306DISPLAY_H
