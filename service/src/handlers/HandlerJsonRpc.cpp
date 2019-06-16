//
// Created by Ivan Kishchenko on 2019-06-08.
//

#include "HandlerJsonRpc.h"
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <iostream>

namespace handlers {
    void HandlerJsonRpc::registerMethod(const std::shared_ptr<IRpcMethod>& method) {
        _methods[method->name()] = method;
    }

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
        auto method = _methods.find(request.method);
        if (method != _methods.end()) {
            try {
                response.result = method->second->handle(request.params);
            } catch (std::exception& ex) {
                response.error = error(InternalError, ex.what());
            }
        } else {
            response.error = error(MethodNotFound, "Method not found");
        }

    }


}// namespace handlers

