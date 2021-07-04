//
// Created by Ivan Kishchenko on 01.05.2021.
//

#ifndef PLATFORM_IO_SERVICE_H
#define PLATFORM_IO_SERVICE_H

#include "Registry.h"

class Service {
public:
    virtual Registry &getRegistry() = 0;

    virtual void postConstruct() = 0;

    virtual void run() = 0;

    virtual void preDestroy() = 0;
};

class BaseService : public Service {
private:
    Registry &_registry;
public:
    explicit BaseService(Registry &registry)
            : _registry(registry) {

    }

    void postConstruct() override {}

    void run() override {}

    void preDestroy() override {}


    Registry &getRegistry() override {
        return _registry;
    }
};


#endif //PLATFORM_IO_SERVICE_H
