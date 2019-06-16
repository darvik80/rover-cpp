//
// Created by Ivan Kishchenko on 2019-06-16.
//

#ifndef ROVER_SERVLETREQUESTHANDLER_H
#define ROVER_SERVLETREQUESTHANDLER_H


#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPResponse.h>

namespace handlers {

    class ServletRequestHandler : public Poco::Net::HTTPRequestHandler {
    public:
        void handleRequest(
                Poco::Net::HTTPServerRequest &request,
                Poco::Net::HTTPServerResponse &response) override;
    private:
        void handleFile(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &response);
        void sendError(const std::string &text, Poco::Net::HTTPResponse::HTTPStatus status = Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);

    private:
        Poco::Net::HTTPServerRequest* _ptrRequest;
        Poco::Net::HTTPServerResponse *_ptrResponse;
    };

}


#endif //ROVER_SERVLETREQUESTHANDLER_H
