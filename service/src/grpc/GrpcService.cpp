//
// Created by Ivan Kishchenko on 04.07.2021.
//

#include "GrpcService.h"
#include "GreeterService.h"

void GrpcService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);
    try {
        const auto &props = registry.getProperties<GrpcProperties>();
        std::string serverAddress(props.host + ":" + std::to_string(props.port));

        _services.emplace_back(new GreeterService());

        grpc::EnableDefaultHealthCheckService(true);
        ServerBuilder builder;
        builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());

        for (auto &service : _services) {
            builder.RegisterService(service.get());
        }
        _server = builder.BuildAndStart();

        logger::info("[grpc] server started: {}", serverAddress);
    } catch (std::exception& ex) {
        logger::info("[grpc] server failed: {}", ex.what());
    }
}

void GrpcService::preDestroy(Registry &registry) {
    _server->Shutdown();
    BaseService::preDestroy(registry);

    logger::info("[grpc] server stopped");
}
