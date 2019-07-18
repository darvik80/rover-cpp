//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-18.
//

#ifndef ROVER_MAINAPPLICATION_H
#define ROVER_MAINAPPLICATION_H


#include "subsystem/Application.h"

class MainApplication : public Application {
public:
    void postConstruct(Application &app) override;

    void run() override;

    int run(int argc, char **argv) override;

    void preDestroy() override;
};


#endif //ROVER_MAINAPPLICATION_H
