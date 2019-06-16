//
// Created by Ivan Kishchenko on 2019-05-22.
//

#include "Factory.h"

#include "Route.h"

#include "JsonRpcHandler.h"
#include "ServletRequestHandler.h"
#include <Poco/Net/HTTPServerRequest.h>

namespace handlers {

    Poco::Net::HTTPRequestHandler *Factory::createRequestHandler(
            const Poco::Net::HTTPServerRequest &request) {

        if (request.getURI() == ROUTE_JSON_RPC)
            return new JsonRpcHandler();

        return new ServletRequestHandler();
    }
}