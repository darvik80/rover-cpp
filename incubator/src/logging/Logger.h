//
// Created by Ivan Kishchenko on 11.04.2021.
//

#ifndef ROVER_LOGGER_H
#define ROVER_LOGGER_H

#include <string_view>

class Logger {
public:
    typedef std::shared_ptr<Logger> Ptr;

    virtual void trace(std::string_view message) = 0;

    virtual void debug(std::string_view message) = 0;

    virtual void info(std::string_view message) = 0;

    virtual void warning(std::string_view message) = 0;

    virtual void error(std::string_view message) = 0;

    virtual void fatal(std::string_view message) = 0;
};

#endif //ROVER_LOGGER_H
