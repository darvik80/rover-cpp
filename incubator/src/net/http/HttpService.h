//
// Created by Ivan Kishchenko on 26.04.2021.
//

#ifndef ROVER_HTTPSERVICE_H
#define ROVER_HTTPSERVICE_H

#include "BaseService.h"
#include "properties/HttpProperties.h"
#include "HttpRequestHandlerManager.h"

class HttpService : public BaseService, public HttpRequestHandlerManager, public std::enable_shared_from_this<HttpService> {
public:
    const char *name() override {
        return "http";
    }

    void postConstruct(Registry &registry) override;
    void preDestroy(Registry &registry) override;

    void process(const HttpRequest& req, HttpResponse& resp) override;
private:
    void addHandlers(Registry &registry);
};


#endif //ROVER_HTTPSERVICE_H
