//
// Created by Kishchenko, Ivan on 4/6/21.
//

#ifndef PLATFORM_IO_SERVICE_H
#define PLATFORM_IO_SERVICE_H

#include "CoreConfig.h"

class Service {
public:
    virtual int setup() = 0;
    virtual int loop() = 0;
};


#endif //PLATFORM_IO_SERVICE_H
