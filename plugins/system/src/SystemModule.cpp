//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-07.
//

#include <rpc/RpcMonitorSupplier.h>
#include "SystemModule.h"

namespace plugin {

    SystemModule::SystemModule() {
        //addMethod(std::make_shared<RpcMonitorSupplier>());
    }

    std::string SystemModule::name() const {
        return "system";
    }

}