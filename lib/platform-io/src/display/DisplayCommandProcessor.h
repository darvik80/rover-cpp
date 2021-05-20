//
// Created by Kishchenko, Ivan on 5/20/21.
//

#ifndef PLATFORM_IO_DISPLAYCOMMANDPROCESSOR_H
#define PLATFORM_IO_DISPLAYCOMMANDPROCESSOR_H


#include <command/Command.h>
#include <device/display/Display.h>

class DisplayCommandProcessor : public CommandProcessor {
    Display& _display;
public:
    enum Command {
        CMD_SET_CURSOR,
        CMD_SET_COLOR,
        CMD_PRINT
    };
public:
    DisplayCommandProcessor(Display &display) : _display(display) {}

    void process(const char *data, size_t size) override {
        if (!size) {
            return;
        }

        auto cmd = data[0];
        switch (cmd) {
            case CMD_SET_CURSOR:
                if (size >= 5) {
                    _display.setCursor(*(uint16_t*)data+1, *(uint16_t*)data+3);
                }
                break;
            case CMD_SET_COLOR:
                if (size >= 5) {
                    _display.setTextColor(*(uint16_t*)data+1, *(uint16_t*)data+3);
                }
                break;
            case CMD_PRINT:
                if (size >= 5) {
                    _display.print((const char*)data+1);
                }
                break;
            default:
                break;
        }
    }
};


#endif //PLATFORM_IO_DISPLAYCOMMANDPROCESSOR_H
