//
// Created by Ivan Kishchenko on 16.05.2021.
//

#ifndef ROVER_SERIALSERVICE_H
#define ROVER_SERIALSERVICE_H

#include <BaseService.h>

#include <serial/SerialPort.h>

class SerialService : public BaseService {
    std::unique_ptr<serial::SerialPort> _serial;
public:
    const char *name() override {
        return "serial";
    }

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry &registry) override {
        BaseService::preDestroy(registry);
    }
};


#endif //ROVER_SERIALSERVICE_H
