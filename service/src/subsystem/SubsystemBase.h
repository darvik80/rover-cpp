//
// Created by Ivan Kishchenko on 2019-08-04.
//

#ifndef ROVER_SUBSYSTEMBASE_H
#define ROVER_SUBSYSTEMBASE_H


#include <Logger.h>
#include "Subsystem.h"
#include "Application.h"
#include <sstream>
#include <optional>

class SubsystemBase : public Subsystem, public Logger {
private:
    static Logger::Ptr devNullLogger;
    Logger::Ptr _logger = devNullLogger;
private:
    std::string message(std::string_view msg);

public:
    void trace(std::string_view msg) override;

    void debug(std::string_view msg) override;

    void info(std::string_view msg) override;

    void warning(std::string_view msg) override;

    void error(std::string_view msg) override;

    void fatal(std::string_view msg) override;

    void postConstruct(Application &app) override;

    void preDestroy() override;

    ~SubsystemBase() override = default;
};

#endif //ROVER_SUBSYSTEMBASE_H
