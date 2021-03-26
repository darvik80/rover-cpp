//
// Created by Kishchenko, Ivan on 3/24/21.
//

#ifndef ROVER_KERNEL_H
#define ROVER_KERNEL_H

#include "Module.h"
#include "ModuleRegistry.h"

class Kernel {
private:
    Content _content;
    ModuleRegistry _registry;
public:
    std::error_code create();

    std::error_code run();
};


#endif //ROVER_KERNEL_H
