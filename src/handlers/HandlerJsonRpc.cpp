//
// Created by Ivan Kishchenko on 2019-06-08.
//

#include "HandlerJsonRpc.h"
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <iostream>
#include <handlers/rpc/HelloMethod.h>


namespace handlers {
    HandlerJsonRpc::HandlerJsonRpc() {
        registerMethod(std::make_shared<HelloFunction>());
        registerMethod(std::make_shared<HelloConsumer>());
        registerMethod(std::make_shared<HelloSupplier>());
    }

    void HandlerJsonRpc::registerMethod(const std::shared_ptr<IRpcMethod>& method) {
        _methods[method->name()] = method;
    }

    void HandlerJsonRpc::handleRequest(
            Poco::Net::HTTPServerRequest &request,
            Poco::Net::HTTPServerResponse &response) {
        if (request.getMethod() != Poco::Net::HTTPRequest::HTTP_POST) {
            response.send() << "rpc support only post requests";
            response.setStatus(Poco::Net::HTTPServerResponse::HTTP_REASON_BAD_REQUEST);
            return;
        }

        if (request.getContentType().find("application/json") == std::string::npos) {
            response.send() << "rpc support only application/json content-type";
            response.setStatus(Poco::Net::HTTPServerResponse::HTTP_REASON_BAD_REQUEST);
            return;
        }

        JsonRpcRequest jsonRpcRequest;
        JsonDecoder(jsonRpcRequest.unMarshaller()).decode(request.stream());

        JsonRpcResponse jsonRpcResponse;
        jsonRpcResponse.id = jsonRpcRequest.id;
        jsonRpcResponse.jsonrpc = jsonRpcRequest.jsonrpc;
        auto method = _methods.find(jsonRpcRequest.method);
        if (method != _methods.end()) {
            try {
                jsonRpcResponse.result = method->second->handle(jsonRpcRequest.params);
            } catch (std::exception& ex) {
                jsonRpcResponse.error = error(InternalError, ex.what());
            }
        } else {
            jsonRpcResponse.error = error(MethodNotFound, "Method not found");
        }

        response.send() << JsonEncoder(jsonRpcResponse.marshaller()).encode();

        response.setStatus(Poco::Net::HTTPServerResponse::HTTP_OK);
    }


}// namespace handlers

