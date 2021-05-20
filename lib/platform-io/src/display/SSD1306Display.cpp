//
// Created by Kishchenko, Ivan on 5/20/21.
//

#include "SSD1306Display.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32


SSD1306Display::SSD1306Display() : _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
    if(!_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
    }
}

const char *SSD1306Display::name() {
    return "ssd1306";
}

void SSD1306Display::setCursor(uint16_t x, uint16_t y) {
    _display.setCursor(x, y);
}

void SSD1306Display::setTextColor(uint16_t c, uint16_t bg) {
    _display.setTextColor(c, bg);
}

void SSD1306Display::print(const char *text) {
    _display.print(text);
}

