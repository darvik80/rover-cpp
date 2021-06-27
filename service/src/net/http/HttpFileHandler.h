//
// Created by Ivan Kishchenko on 27.04.2021.
//

#ifndef ROVER_HTTPFILEHANDLER_H
#define ROVER_HTTPFILEHANDLER_H

#include "HttpRequestHandler.h"

class HttpFileHandler : public HttpRequestHandler {
    std::string _root;
public:
    HttpFileHandler(std::string_view root);
    void handle(const HttpRequest &request, HttpResponse &response) override;
};


#endif //ROVER_HTTPFILEHANDLER_H
