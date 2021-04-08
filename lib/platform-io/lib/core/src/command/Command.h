//
// Created by Ivan Kishchenko on 03.04.2021.
//

#ifndef PLATFORM_IO_COMMAND_H
#define PLATFORM_IO_COMMAND_H

#include <stddef.h>

class Command {
public:
    virtual int execute() = 0;
};

class CommandProcessor {
public:
    virtual void process(const char* data, size_t size) = 0;

};

#endif //PLATFORM_IO_COMMAND_H
