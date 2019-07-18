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

class BOOST_SYMBOL_VISIBLE Module {
public:
    typedef std::shared_ptr<Module> Ptr;

    virtual std::string name() const = 0;

    virtual std::map<std::string, RpcMethod::Ptr> getRpcMethods() const = 0;

    virtual ~Module() = default;
};


#endif //ROVER_MODULE_H
