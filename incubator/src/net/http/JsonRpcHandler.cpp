//
// Created by Ivan Kishchenko on 26.04.2021.
//

#include "JsonRpcHandler.h"

void JsonRpcHandler::handle(const HttpRequest &request, HttpResponse &response) {
    response.keep_alive(false);
    response.set(http::field::server, "Beast");

    if (request.method() != http::verb::post) {
        response.result(http::status::bad_request);
        response.set(http::field::content_type, "text/plain");
        response.body() = "json-rpc support only post requests";
    } else if (request.at(http::field::content_type).find("application/json") == std::string::npos) {
        response.result(http::status::bad_request);
        response.set(http::field::content_type, "text/plain");
        response.body() = "json-rpc support only application/json content-type";
    } else {
        std::shared_ptr<JsonRpcRequest> jsonRequest = std::make_shared<JsonRpcRequest>();
        std::shared_ptr<JsonRpcResponse> jsonResponse = std::make_shared<JsonRpcResponse>();
        try {
            JsonDecoder(jsonRequest).decode(request.body());

            handle(*jsonRequest, *jsonResponse);
        } catch (std::exception &ex) {
            JsonRcpError error;
            error.code = InternalError;
            error.message = ex.what();
            jsonResponse->error = error;
        }

        response.body() = JsonEncoder(jsonResponse).encode();
    }

    response.prepare_payload();
}

void JsonRpcHandler::handle(const JsonRpcRequest &request, JsonRpcResponse &response) {
    try {
        response.id = request.id;
        response.jsonrpc = request.jsonrpc;

        const auto method = findMethod(request.method);

        if (method) {
            response.result = method.get()->handle(request.params);
        } else {
            JsonRcpError error;
            error.code = MethodNotFound;
            error.message = "Method not found";
            response.error = error;
        }

    } catch (std::exception &ex) {
        response.jsonrpc = "2.0";
        JsonRcpError error;
        error.code = InternalError;
        error.message = ex.what();
        response.error = error;
    }
}
