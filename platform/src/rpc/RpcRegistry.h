//
// Created by Ivan Kishchenko on 2019-06-16.
//

#ifndef ROVER_RPCREGISTRY_H
#define ROVER_RPCREGISTRY_H


#include <Poco/SingletonHolder.h>
#include <Poco/HashMap.h>
#include "RpcMethod.h"

class RpcRegistry {
private:
    Poco::HashMap<std::string, RpcMethod::Ptr> _methods;

public:
    static RpcRegistry& instance() {
        static Poco::SingletonHolder<RpcRegistry> sh;
        return *sh.get();
    }

    void addMethod(RpcMethod* method) {
        _methods[method->name()] = RpcMethod::Ptr(method);
    }

    Poco::Optional<RpcMethod::Ptr> findMethod(const std::string& method) {
        Poco::Optional<RpcMethod::Ptr> result;
        auto iter = _methods.find(method);
        if (iter != _methods.end()) {
            result = iter->second;
        }

        return result;
    }
};

#endif //ROVER_RPCREGISTRY_H
