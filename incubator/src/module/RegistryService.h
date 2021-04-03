//
// Created by Kishchenko, Ivan on 3/24/21.
//

#ifndef ROVER_REGISTRYSERVICE_H
#define ROVER_REGISTRYSERVICE_H

#include "module/raspberry/AsyncUdp.h"
#include "Module.h"
#include "Transport.h"
#include "net/Multicast.h"

class RegistryService : public Module {
public:
    std::error_code create(Content &content) override;

    std::error_code destroy() override;

    const char *getName() override {
        return "registry";
    }

    ModuleStatus getStatus() override;

private:
    Transport::Ptr _server;

    std::shared_ptr<MulticastReceiver> _receiver;
};

class RegistryServiceClient : public Module {
public:
    std::error_code create(Content &content) override;

    std::error_code destroy() override;

    const char *getName() override {
        return "registry_client";
    }

    ModuleStatus getStatus() override;

private:
    std::unique_ptr<std::thread> _thread;

    std::shared_ptr<MulticastSender> _sender;
};


#endif //ROVER_REGISTRYSERVICE_H
