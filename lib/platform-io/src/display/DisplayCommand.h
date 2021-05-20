//
// Created by Kishchenko, Ivan on 5/20/21.
//

#ifndef PLATFORM_IO_DISPLAYCOMMAND_H
#define PLATFORM_IO_DISPLAYCOMMAND_H

#include <command/Command.h>
#include <device/display/Display.h>

class DisplayCommandSetCursor : public Command {
    Display& _display;
    uint16_t _x;
    uint16_t _y;
public:
    DisplayCommandSetCursor(Display& display, uint16_t x, uint16_t y) : _display(display), _x(x), _y(y) {}

    int execute() override {
        _display.setCursor(_x, _y);
    }
};

class DisplayCommandSetColor : public Command {
    Display& _display;
    uint16_t _color;
    uint16_t _background;
public:
    DisplayCommandSetColor(Display& display, uint16_t color, uint16_t background) : _display(display), _color(color), _background(background) {}

    int execute() override {
        _display.setTextColor(_color, _background);
    }
};

class DisplayCommandPrint : public Command {
    Display& _display;
    const char* _text;
public:
    DisplayCommandPrint(Display& display, const char* text) : _display(display), _text(text) {}

    int execute() override {
        _display.print(_text);
    }
};

#endif //PLATFORM_IO_DISPLAYCOMMAND_H
