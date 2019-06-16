//
// Created by Ivan Kishchenko on 2019-06-16.
//

#ifndef ROVER_SPEC_H
#define ROVER_SPEC_H

#include <json/JsonBase.h>

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

#endif //ROVER_SPEC_H
