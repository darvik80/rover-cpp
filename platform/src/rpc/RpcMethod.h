//
// Created by Ivan Kishchenko on 2019-06-09.
//

#ifndef ROVER_RPCMETHOD_H
#define ROVER_RPCMETHOD_H

#include <string>
#include "json/Interfaces.h"

class RpcMethod : Poco::ReferenceCounter {
public:
    typedef Poco::SharedPtr<RpcMethod> Ptr;
public:
    virtual std::string name() const = 0;
    virtual Poco::Optional<Poco::Dynamic::Var> handle(const Poco::Optional<Poco::Dynamic::Var>& params) const = 0;

    virtual ~RpcMethod() = default;
};

template <typename T, typename R>
class RpcFunction : public RpcMethod {
public:
    Poco::Optional<Poco::Dynamic::Var> handle(const Poco::Optional<Poco::Dynamic::Var>& params) const override {
        T inc;
        if (params.isSpecified()) {
            inc.unMarshal(params.value());
        }

        return this->exec(inc).marshaller()->marshal();
    };

    virtual R exec(const T& params)const = 0;
};

template <typename T>
class RpcConsumer : public RpcMethod {
public:
    Poco::Optional<Poco::Dynamic::Var> handle(const Poco::Optional<Poco::Dynamic::Var>& params) const override {
        T inc;
        if (params.isSpecified()) {
            inc.unMarshal(params.value());
        }

        this->exec(inc);

        return Poco::Optional<Poco::Dynamic::Var>();
    };

    virtual void exec(const T& params) const = 0;
};

template <typename R>
class RpcSupplier : public RpcMethod {
    Poco::Optional<Poco::Dynamic::Var> handle(const Poco::Optional<Poco::Dynamic::Var>& params) const override {
        return this->exec().marshaller()->marshal();
    };

    virtual R exec() const = 0;
};


#endif //ROVER_RPCMETHOD_H
