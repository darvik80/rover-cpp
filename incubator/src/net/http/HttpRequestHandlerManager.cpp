//
// Created by Ivan Kishchenko on 26.04.2021.
//

#include "HttpRequestHandlerManager.h"

void HttpRequestHandlerManager::process(const HttpRequest& req, HttpResponse& resp) {
   if (auto it = _handlers.find(Request{req.method(), req.target().data()}); it != _handlers.end()) {
       it->second(req, resp);
   } else {
       throw std::invalid_argument(req.target().data());
   }
}