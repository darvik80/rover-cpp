//
// Created by Ivan Kishchenko on 2019-06-08.
//

#include "HandlerJsonRpc.h"
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <iostream>

#include <rpc/RpcRegistry.h>
#include "rpc/HealthRpcSupplier.h"

namespace handlers {
    void HandlerJsonRpc::handleRequest(
            Poco::Net::HTTPServerRequest &request,
            Poco::Net::HTTPServerResponse &response) {
        if (request.getMethod() != Poco::Net::HTTPRequest::HTTP_POST) {
            response.send() << "json-rpc support only post requests";
            response.setStatus(Poco::Net::HTTPServerResponse::HTTP_REASON_BAD_REQUEST);
            return;
        }

        if (request.getContentType().find("application/json") == std::string::npos) {
            response.send() << "json-rpc support only application/json content-type";
            response.setStatus(Poco::Net::HTTPServerResponse::HTTP_REASON_BAD_REQUEST);
            return;
        }

        JsonRpcRequest jsonRpcRequest;
        JsonRpcResponse jsonRpcResponse;

        JsonDecoder(jsonRpcRequest.unMarshaller()).decode(request.stream());

        handle(jsonRpcRequest, jsonRpcResponse);

        response.setStatus(Poco::Net::HTTPServerResponse::HTTP_OK);
        response.send() << JsonEncoder(jsonRpcResponse.marshaller()).encode();
    }

    void HandlerJsonRpc::handle(JsonRpcRequest& request, JsonRpcResponse& response) {
        response.id = request.id;
        response.jsonrpc = request.jsonrpc;
        auto method = RpcRegistry::instance().findMethod(request.method);
        if (method.isSpecified()) {
            try {
                response.result = method.value()->handle(request.params);
            } catch (std::exception& ex) {
                response.error = error(InternalError, ex.what());
            }
        } else {
            response.error = error(MethodNotFound, "Method not found");
        }

    }


}// namespace handlers

