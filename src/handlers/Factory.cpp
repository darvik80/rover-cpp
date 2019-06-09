//
// Created by Ivan Kishchenko on 2019-05-22.
//

#include "Factory.h"
#include "Ping.h"
#include "HandlerJsonRpc.h"

#include "Route.h"
#include <Poco/Net/HTTPServerRequest.h>

namespace handlers {

    Poco::Net::HTTPRequestHandler *Factory::createRequestHandler(
            const Poco::Net::HTTPServerRequest &request) {

        if (request.getURI() == ROUTE_JSON_RPC)
            return new HandlerJsonRpc();

        if (request.getURI() == ROUTE_PING)
            return new HandlerJsonRpc();

        return nullptr;
    }
}