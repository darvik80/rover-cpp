//
// Created by Ivan Kishchenko on 2019-05-22.
//

#ifndef ROVER_PING_H
#define ROVER_PING_H


#include <Poco/Net/HTTPRequestHandler.h>

namespace handlers {

    class Ping : public Poco::Net::HTTPRequestHandler {
    private:
        void handleRequest(
                Poco::Net::HTTPServerRequest &request,
                Poco::Net::HTTPServerResponse &response) override;

        virtual ~Ping();
    };
}

#endif //ROVER_PING_H
