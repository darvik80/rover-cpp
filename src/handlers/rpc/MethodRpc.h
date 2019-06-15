//
// Created by Ivan Kishchenko on 2019-06-09.
//

#ifndef ROVER_METHODRPC_H
#define ROVER_METHODRPC_H

#include "codec/Interfaces.h"

class IMethodRpc {
public:
    virtual std::shared_ptr<IMarshaller> handle(const std::shared_ptr<IUnMarshaller>& params) = 0;

    virtual ~IMethodRpc() = default;
};

template <typename T, typename R>
class BaseMethodRpc : public IMethodRpc {
public:
    std::shared_ptr<IMarshaller> handle(const std::shared_ptr<IUnMarshaller>& params) override {
        T* p = dynamic_cast<T*>(params.get());
        if (p) {
            return exec(*p);
        }

        throw std::bad_exception();
    }

    virtual std::shared_ptr<R> exec(const T& params) = 0;
};

#endif //ROVER_METHODRPC_H
