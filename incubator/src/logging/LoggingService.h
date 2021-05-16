//
// Created by Ivan Kishchenko on 11.04.2021.
//

#ifndef ROVER_LOGGINGSERVICE_H
#define ROVER_LOGGINGSERVICE_H

#include "Logger.h"
#include "Service.h"

class LoggingService : public Service, public std::enable_shared_from_this<LoggingService> {
public:
    const char *name() override {
        return "log";
    }

    int order() override {
        return INT32_MAX;
    }
    void postConstruct(Registry &registry) override;

    void preDestroy(Registry& registry) override;
};


#endif //ROVER_LOGGINGSERVICE_H
