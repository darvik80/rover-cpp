//
// Created by Ivan Kishchenko on 09.04.2021.
//

#ifndef ROVER_APPLICATION_H
#define ROVER_APPLICATION_H

#include "Service.h"
#include "Registry.h"
#include "Properties.h"
#include "event/ApplicationEvent.h"

class Application : public Service {
private:
public:
    const char *name() override {
        return "application";
    }

    int order() override {
        return 0;
    }

    void run(int argc, char **argv);

    void postConstruct(Registry &registry) override;

    void run(Registry &registry);

    void preDestroy(Registry &registry) override;
};


#endif //ROVER_APPLICATION_H
