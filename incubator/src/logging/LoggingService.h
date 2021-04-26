//
// Created by Ivan Kishchenko on 11.04.2021.
//

#ifndef ROVER_LOGGINGSERVICE_H
#define ROVER_LOGGINGSERVICE_H

#include "Logger.h"
#include "Service.h"

class LoggingService : public Logger, public Service, public std::enable_shared_from_this<LoggingService> {
public:
    void trace(std::string_view message) override;

    void debug(std::string_view message) override;

    void info(std::string_view message) override;

    void warning(std::string_view message) override;

    void error(std::string_view message) override;

    void fatal(std::string_view message) override;

public:
    const char *name() override {
        return "log";
    }

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry& registry) override;
};


#endif //ROVER_LOGGINGSERVICE_H
