//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-07.
//

#include <rpc/RpcMonitorSupplier.h>
#include "SystemModule.h"
#include <iostream>

namespace plugin {

    SystemModule::SystemModule() {
        std::cout << "Create SystemModule" << std::endl;
        addMethod(std::make_shared<RpcMonitorSupplier>());

    }

    std::string SystemModule::name() const {
        return "system";
    }

    SystemModule::~SystemModule() {
        std::cout << "Destroy SystemModule" << std::endl;
    }
}