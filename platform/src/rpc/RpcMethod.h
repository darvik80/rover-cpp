//
// Created by Ivan Kishchenko on 2019-06-09.
//

#ifndef ROVER_RPCMETHOD_H
#define ROVER_RPCMETHOD_H

#include <string>
#include <memory>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>

class RpcMethod {
public:
    typedef std::shared_ptr<RpcMethod> Ptr;
    typedef std::vector<Ptr> PtrVec;
public:
    virtual std::string name() const = 0;

    virtual boost::optional<boost::property_tree::ptree> handle(const boost::optional<boost::property_tree::ptree> &params) const = 0;

    virtual ~RpcMethod() = default;
};

template<typename T, typename R>
class RpcFunction : public RpcMethod {
public:
    boost::optional<boost::property_tree::ptree>
    handle(const boost::optional<boost::property_tree::ptree> &params) const override {
        T inc;
        if (params) {
            inc.unMarshal(params.value());
        }

        return exec(inc).marshal();
    };

    virtual R exec(const T &params) const = 0;
};

template<typename T>
class RpcConsumer : public RpcMethod {
public:
    boost::optional<boost::property_tree::ptree> handle(const boost::optional<boost::property_tree::ptree> &params) const override {
        T inc;
        if (params) {
            inc.unMarshal(params.value());
        }

        exec(inc);

        return boost::optional<boost::property_tree::ptree>();
    };

    virtual void exec(const T &params) const = 0;
};

template<typename R>
class RpcSupplier : public RpcMethod {
    boost::optional<boost::property_tree::ptree> handle(const boost::optional<boost::property_tree::ptree> &params) const override {
        return exec().marshal();
    };

    virtual R exec() const = 0;
};


#endif //ROVER_RPCMETHOD_H
