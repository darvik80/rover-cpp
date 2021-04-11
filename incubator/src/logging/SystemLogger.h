//
// Created by Ivan Kishchenko on 11.04.2021.
//

#ifndef ROVER_SYSTEMLOGGER_H
#define ROVER_SYSTEMLOGGER_H

#include "Logger.h"

class SystemLogger : public Logger {
public:
    void trace(std::string_view message) override;

    void debug(std::string_view message) override;

    void info(std::string_view message) override;

    void warning(std::string_view message) override;

    void error(std::string_view message) override;

    void fatal(std::string_view message) override;

};


#endif //ROVER_SYSTEMLOGGER_H