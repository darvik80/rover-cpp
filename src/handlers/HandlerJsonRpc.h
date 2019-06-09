//
// Created by Ivan Kishchenko on 2019-06-08.
//

#ifndef ROVER_HANDLERJSONRPC_H
#define ROVER_HANDLERJSONRPC_H

#include <Poco/Net/HTTPRequestHandler.h>

#include <Poco/Optional.h>
#include <Poco/Dynamic/Var.h>
#include "codec/json/JsonBase.h"
#include <map>
#include <handlers/rpc/MethodRpc.h>

namespace handlers {
    struct MethodMetaData{
        std::shared_ptr<IMethodRpc> _method;
        std::shared_ptr<IUnMarshaller> _unMarshaller;

        MethodMetaData() = default;
        MethodMetaData(IMethodRpc* method, IUnMarshaller* unMarshaller)
            : _method(method), _unMarshaller(unMarshaller) {
        }
    };

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
    __DECLARE_DTO_FIELD(std::string, method)
    __DECLARE_DTO_FIELD(Poco::Dynamic::Var, params)
    __DECLARE_DTO_FIELD(Poco::Optional<std::string>, id)
    __DECLARE_DTO_FIELD(std::string, jsonrpc)

        BEGIN_JSON_UNMARSHAL
                ITEM_JSON_UNMARSHAL(method)
                ITEM_JSON_UNMARSHAL(id)
                ITEM_JSON_UNMARSHAL(params)
                ITEM_JSON_UNMARSHAL(jsonrpc)
        END_JSON_UNMARSHAL

        BEGIN_JSON_MARSHAL
        END_JSON_MARSHAL

    END_DECLARE_DTO

    /**
     * json-rpc Error
     */
    BEGIN_DECLARE_DTO(JsonRcpError)
    __DECLARE_DTO_FIELD(int, code)
    __DECLARE_DTO_FIELD(std::string, message)
    __DECLARE_DTO_FIELD(Poco::Dynamic::Var, data)

        BEGIN_JSON_UNMARSHAL
        END_JSON_UNMARSHAL

        BEGIN_JSON_MARSHAL
                ITEM_JSON_MARSHAL(code)
                ITEM_JSON_MARSHAL(message)
                ITEM_JSON_MARSHAL(data)
        END_JSON_MARSHAL
    END_DECLARE_DTO

    /**
     * json-rpc Response
     */
    BEGIN_DECLARE_DTO(JsonRpcResponse)
    __DECLARE_DTO_FIELD(Poco::Dynamic::Var, result)
    __DECLARE_DTO_FIELD(Poco::Optional<JsonRcpError>, error)
    __DECLARE_DTO_FIELD(Poco::Optional<std::string>, id)
    __DECLARE_DTO_FIELD(std::string, jsonrpc)

        BEGIN_JSON_UNMARSHAL
        END_JSON_UNMARSHAL

        BEGIN_JSON_MARSHAL
                ITEM_JSON_MARSHAL(result)
                ITEM_JSON_MARSHAL_OBJ_OPT(error)
                ITEM_JSON_MARSHAL(id)
                ITEM_JSON_MARSHAL(jsonrpc)
        END_JSON_MARSHAL

    END_DECLARE_DTO


    class HandlerJsonRpc : public Poco::Net::HTTPRequestHandler {
    private:
        std::map<std::string, MethodMetaData> _methods;

    private:
        static JsonRcpError error(int code, const std::string& message) {
            JsonRcpError err;
            err.code = code;
            err.message = message;

            return err;
        }

    public:
        HandlerJsonRpc();

    private:
        void registerMethod(const std::string& method, const MethodMetaData& pointer);

        void handleRequest(
                Poco::Net::HTTPServerRequest &request,
                Poco::Net::HTTPServerResponse &response) override;
    };


}

#endif //ROVER_HANDLERJSONRPC_H
