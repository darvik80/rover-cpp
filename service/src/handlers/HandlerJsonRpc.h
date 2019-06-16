//
// Created by Ivan Kishchenko on 2019-06-08.
//

#ifndef ROVER_HANDLERJSONRPC_H
#define ROVER_HANDLERJSONRPC_H

#include <Poco/Net/HTTPRequestHandler.h>

#include <rpc/RpcMethod.h>
#include <json/JsonBase.h>

#include <map>

namespace handlers {
    enum Code {
        ParseError = -32700,    // Parse error	Invalid JSON was received by the server. An error occurred on the server while parsing the JSON text.
        InvalideRequest = -32600,    // Invalid Request	The JSON sent is not a valid Request object.
        MethodNotFound = -32601,    // Method not found	The method does not exist / is not available.
        InvalidParams = -32602,    // Invalid params	Invalid method parameter(s).
        InternalError = -32603,    // Internal error	Internal JSON-RPC error.
        ServerError = -32000, // to -32099	Server error	Reserved for implementation-defined server-errors.
    };

    /**
     * json-rpc Request
     */
    BEGIN_DECLARE_DTO(JsonRpcRequest)

    __DECLARE_DTO_FIELDS4(
            std::string, method,
            Poco::Optional<Poco::Dynamic::Var>, params,
            Poco::Optional<std::string>, id,
            std::string, jsonrpc
    )
    END_DECLARE_DTO

    /**
     * json-rpc Error
     */
    BEGIN_DECLARE_DTO(JsonRcpError)

    __DECLARE_DTO_FIELDS3(
            int, code,
            std::string, message,
            Poco::Optional<Poco::Dynamic::Var>, data
    )

    END_DECLARE_DTO

    /**
     * json-rpc Response
     */
    BEGIN_DECLARE_DTO(JsonRpcResponse)

    __DECLARE_DTO_FIELDS4(
            Poco::Optional<Poco::Dynamic::Var>, result,
            Poco::Optional<JsonRcpError>, error,
            Poco::Optional<std::string>, id,
            std::string, jsonrpc
    )
    END_DECLARE_DTO


    class HandlerJsonRpc : public Poco::Net::HTTPRequestHandler {
    private:
        std::map<std::string, std::shared_ptr<IRpcMethod> > _methods;

    private:
        static JsonRcpError error(int code, const std::string &message) {
            JsonRcpError err;
            err.code = code;
            err.message = message;

            return err;
        }

    public:
        HandlerJsonRpc();

    private:
        void registerMethod(const std::shared_ptr<IRpcMethod> &pointer);

        void handleRequest(
                Poco::Net::HTTPServerRequest &request,
                Poco::Net::HTTPServerResponse &response) override;
    };


}

#endif //ROVER_HANDLERJSONRPC_H
