//
// Created by Ivan Kishchenko on 04.07.2021.
//

#ifndef ROVER_GRPCSERVICE_H
#define ROVER_GRPCSERVICE_H

#include "BaseService.h"
#include <grpcpp/grpcpp.h>

class GrpcService : public BaseServiceShared<GrpcService> {
public:
    const char *name() override {
        return "grpc";
    }

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry &registry) override;

private:
    std::vector<std::shared_ptr<grpc::Service>> _services;
    std::unique_ptr<grpc::Server> _server;
};


#endif //ROVER_GRPCSERVICE_H
