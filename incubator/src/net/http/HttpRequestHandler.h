//
// Created by Ivan Kishchenko on 26.04.2021.
//

#ifndef ROVER_HTTPREQUESTHANDLER_H
#define ROVER_HTTPREQUESTHANDLER_H

#include <boost/beast/http.hpp>
#include <functional>
#include <string>

namespace http = boost::beast::http;

typedef http::request<http::string_body> HttpRequest;
typedef http::response<http::string_body> HttpResponse;
typedef std::function<void(const HttpRequest &, HttpResponse &)> HttpRequestHandlerFn;

class HttpRequestHandler {
public:
    typedef std::shared_ptr<HttpRequestHandler> Ptr;
public:
    virtual void handle(const HttpRequest &, HttpResponse &) = 0;
};

#endif //ROVER_HTTPREQUESTHANDLER_H
