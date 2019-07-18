//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#include "ModuleBase.h"

std::map<std::string, RpcMethod::Ptr> ModuleBase::getRpcMethods() const {
    return getMethods();
}
