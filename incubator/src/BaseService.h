//
// Created by Ivan Kishchenko on 24.04.2021.
//

#ifndef ROVER_BASESERVICE_H
#define ROVER_BASESERVICE_H

#include "Service.h"
#include "Registry.h"
#include "logging/Logger.h"

class BaseService : public Service, protected Logger {
private:
    Logger::Ptr _log;
public:
    void postConstruct(Registry &registry) override;

    void preDestroy(Registry& registry) override;

protected:
    void trace(std::string_view message) override;

    void debug(std::string_view message) override;

    void info(std::string_view message) override;

    void warning(std::string_view message) override;

    void error(std::string_view message) override;

    void fatal(std::string_view message) override;
};


#endif //ROVER_BASESERVICE_H
