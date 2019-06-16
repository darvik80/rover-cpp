//
// Created by Ivan Kishchenko on 2019-06-08.
//

#ifndef ROVER_HANDLERJSONRPC_H
#define ROVER_HANDLERJSONRPC_H

#include <Poco/Net/HTTPRequestHandler.h>

#include <rpc/RpcMethod.h>
#include "json-rpc/spec.h"

#include <map>

namespace handlers {

    class HandlerJsonRpc : public Poco::Net::HTTPRequestHandler {

    private:
        static JsonRcpError error(int code, const std::string &message) {
            JsonRcpError err;
            err.code = code;
            err.message = message;

            return err;
        }

    public:
        void handle(
                JsonRpcRequest& request,
                JsonRpcResponse& response);

        void handleRequest(
                Poco::Net::HTTPServerRequest &request,
                Poco::Net::HTTPServerResponse &response) override;
    };


}

#endif //ROVER_HANDLERJSONRPC_H
