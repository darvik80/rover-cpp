//
// Created by Ivan Kishchenko on 26.04.2021.
//

#ifndef ROVER_JSONRPCHANDLER_H
#define ROVER_JSONRPCHANDLER_H

#include "HttpRequestHandler.h"
#include "rpc/RpcRegistry.h"
#include "rpc/RpcMethod.h"
#include "event/EventHandler.h"

class JsonRpcHandler : public HttpRequestHandler, public RpcRegistry {
public:
    void handle(const HttpRequest &request, HttpResponse &response) override;
private:
    void handle(const JsonRpcRequest &request, JsonRpcResponse &response);
};

class JsonRpcRegisterEvent : public Event {
    RpcRegistry& _jsonRpc;
public:
    explicit JsonRpcRegisterEvent(RpcRegistry &jsonRpc)
            : _jsonRpc(jsonRpc) {}

    [[nodiscard]] RpcRegistry &getJsonRpc() const {
        return _jsonRpc;
    }
};

#endif //ROVER_JSONRPCHANDLER_H
