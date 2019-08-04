//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-12.
//

#ifndef ROVER_LOGGERSUBSYSTEM_H
#define ROVER_LOGGER_H


#include "Subsystem.h"

#include <string>

#define PROP_LOG_LEVEL "log.level"
#define PROP_LOG_ENABLE_FILE "log.file"
#define PROP_LOG_ENABLE_CONSOLE "log.console"

class Logger : public Subsystem {
public:
    Logger();

    const char *name() const override;

    void postConstruct(Application &app) override;

    void preDestroy() override;

    void trace(const std::string &message);
    void debug(const std::string &message);
    void info(const std::string &message);
    void warning(const std::string &message);
    void error(const std::string &message);
    void fatal(const std::string &message);
};


#endif //ROVER_LOGGERSUBSYSTEM_H
