//
// Created by Ivan Kishchenko on 02.05.2021.
//

#ifndef ROVER_SERIALSERVICE_H
#define ROVER_SERIALSERVICE_H

#include "BaseService.h"
#include "BoostSerialPort.h"

class SerialService : public BaseService {
    bool _connected{false};
    std::unique_ptr<BoostSerialPort> _serial;
public:
    const char *name() override {
        return "serial";
    }

    void postConstruct(Registry &registry) override;
};


#endif //ROVER_SERIALSERVICE_H
