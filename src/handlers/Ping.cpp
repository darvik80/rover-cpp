#include "Ping.h"

#include <Poco/Net/HTTPServerResponse.h>
#include <iostream>

namespace handlers
{

    void Ping::handleRequest(
            Poco::Net::HTTPServerRequest& request,
            Poco::Net::HTTPServerResponse& response)
    {
        response.send().flush();
        response.setStatus(Poco::Net::HTTPServerResponse::HTTP_OK);
    }


    Ping::~Ping() {

    }
} // namespace handlers
