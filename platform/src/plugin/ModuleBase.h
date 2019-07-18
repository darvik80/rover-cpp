//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#ifndef ROVER_MODULEBASE_H
#define ROVER_MODULEBASE_H


#include <rpc/RpcRegistry.h>
#include "Module.h"

class ModuleBase : public Module, protected RpcRegistry {
public:
    std::map<std::string, RpcMethod::Ptr> getRpcMethods() const override;
};


#endif //ROVER_MODULEBASE_H
