//
// Created by Ivan Kishchenko on 26.04.2021.
//

#include "HttpService.h"
#include "HttpWorker.h"
#include "event/EventManager.h"
#include "event/ApplicationEvent.h"
#include "JsonRpcHandler.h"

void HttpService::addHandlers(Registry &registry) {
    auto jsonRpc = std::make_shared<JsonRpcHandler>();
    registry.getService<EventManager>().raiseEvent(JsonRpcRegisterEvent{*jsonRpc});
    registerHandler(http::verb::post, "/rpc", jsonRpc);
}

void HttpService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    const auto &props = registry.getProperties<HttpProperties>();
    auto &ioc = registry.getIoService();

    addHandlers(registry);
    auto worker = std::make_shared<HttpWorker>(registry.getIoService(), props.host, props.port, shared_from_this());

    std::function<bool(const ApplicationStartedEvent &event)> fnStart = [&props, worker, this](const ApplicationStartedEvent &event) -> bool {
        worker->start();
        info("started: " + props.host + ":" + std::to_string(props.port));
        return true;
    };
    registry.getService<EventManager>().subscribe(fnStart);

    std::function<bool(const ApplicationCloseEvent &event)> fnShutdown = [&props, worker, this](const ApplicationCloseEvent &event) -> bool {
        worker->shutdown();
        info("stopped: " + props.host + ":" + std::to_string(props.port));
        return true;
    };

    registry.getService<EventManager>().subscribe(fnShutdown);
}

void HttpService::process(const HttpRequest& req, HttpResponse& resp) {
    info(std::string("handle: ") + req.method_string().data() + ":" + req.target().data());
    HttpRequestHandlerManager::process(req, resp);
}

void HttpService::preDestroy(Registry &registry) {
    BaseService::preDestroy(registry);
}