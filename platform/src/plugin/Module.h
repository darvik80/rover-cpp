//
// Created by Ivan Kishchenko on 2019-06-16.
//

#ifndef ROVER_MODULE_H
#define ROVER_MODULE_H

#include <boost/config.hpp>

#include <string>
#include <map>
#include <memory>
#include <rpc/RpcMethod.h>

#define FN_CREATE_MODULE "createModule"

class BOOST_SYMBOL_VISIBLE Module {
public:
    typedef std::shared_ptr<Module> Ptr;
    typedef std::vector<Ptr> PtrVec;

    virtual std::string name() const = 0;

    virtual std::map<std::string, RpcMethod::Ptr> getRpcMethods() const = 0;

    virtual ~Module() = default;
};


#endif //ROVER_MODULE_H
