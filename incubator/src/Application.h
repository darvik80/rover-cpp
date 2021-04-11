//
// Created by Ivan Kishchenko on 09.04.2021.
//

#ifndef ROVER_APPLICATION_H
#define ROVER_APPLICATION_H

#include "Service.h"
#include "Registry.h"
#include "Properties.h"

class Application : public Service, public Registry {
public:
    const char* name() override {
        return "application";
    }
public:
    void run(int argc, char **argv);

    void postConstruct(Registry &registry) override;

    void run() override;
    void preDestroy() override;

};


#endif //ROVER_APPLICATION_H
