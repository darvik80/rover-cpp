//
// Created by Ivan Kishchenko on 2019-06-09.
//

#ifndef ROVER_RPCMETHOD_H
#define ROVER_RPCMETHOD_H

#include <string>
#include <vector>
#include <memory>
#include <boost/optional.hpp>
#include <nlohmann/json.hpp>
#include "json/Helper.h"
#include "json/Spec.h"

class RpcMethod {
public:
    typedef std::shared_ptr<RpcMethod> Ptr;
    typedef std::vector<Ptr> PtrVec;
public:
    [[nodiscard]] virtual std::string name() const = 0;

    [[nodiscard]] virtual boost::optional<nlohmann::json> handle(const boost::optional<nlohmann::json> &params) const = 0;

    virtual ~RpcMethod() = default;
};

template<typename T, typename R>
class RpcFunction : public RpcMethod {
public:
    [[nodiscard]] boost::optional<nlohmann::json> handle(const boost::optional<nlohmann::json> &params) const override {
        T inc;
        if (params) {
            inc.unMarshal(params.value());
        }

        return marshal(exec(inc));
    };

    virtual R exec(const T &params) const = 0;
};

template<typename T>
class RpcConsumer : public RpcMethod {
public:
    [[nodiscard]] boost::optional<nlohmann::json> handle(const boost::optional<nlohmann::json> &params) const override {
        T inc;
        if (params) {
            inc = unMarshal<T>(params.value());
        }

        exec(inc);

        return boost::optional<nlohmann::json>();
    };

    virtual void exec(const T &params) const = 0;
};

template<typename R>
class RpcSupplier : public RpcMethod {
    [[nodiscard]] boost::optional<nlohmann::json> handle(const boost::optional<nlohmann::json> &params) const override {
        nlohmann::json res;

        return marshal(exec());
    };

    virtual R exec() const = 0;
};

class RpcExec : public RpcMethod {
    [[nodiscard]] boost::optional<nlohmann::json> handle(const boost::optional<nlohmann::json> &params) const override {
        nlohmann::json res;

        exec();

        return boost::optional<nlohmann::json>();
    };

    virtual void exec() const = 0;
};


#endif //ROVER_RPCMETHOD_H
