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
        registerMethod("hello", MethodMetaData(new HelloMethod(), new HelloParams()));
    }

    void HandlerJsonRpc::registerMethod(const std::string &method, const MethodMetaData& metaData) {
        _methods[method] = metaData;
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
            const auto meta = method->second;
            auto cloneable = dynamic_cast<ICloneable*>(meta._unMarshaller.get());
            if (cloneable) {
                auto params = std::shared_ptr<IUnMarshaller>((IUnMarshaller*)cloneable->clone());
                if (params) {
                    params->unMarshal(jsonRpcRequest.params);
                    std::shared_ptr<IMarshaller> result(meta._method.get()->handle(params));
                    jsonRpcResponse.result = result->marshal();
                } else {
                    jsonRpcResponse.error = error(InternalError, "Can't decode params");
                }
            } else {
                jsonRpcResponse.error = error(InternalError, "Can't find params");
            }
        } else {
            jsonRpcResponse.error = error(MethodNotFound, "Method not found");
        }

        response.send() << JsonEncoder(jsonRpcResponse.marshaller()).encode();

        response.setStatus(Poco::Net::HTTPServerResponse::HTTP_OK);
    }


}// namespace handlers

