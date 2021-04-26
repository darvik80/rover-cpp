//
// Created by Kishchenko, Ivan on 3/24/21.
//

#ifndef ROVER_REGISTRYSERVICE_H
#define ROVER_REGISTRYSERVICE_H

#include "module/raspberry/AsyncUdp.h"
#include "Service.h"
#include "Transport.h"
#include "net/Multicast.h"

class RegistryService : public Service {
public:
    const char *name() override;

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry& registry) override;

private:
    Transport::Ptr _server;

    std::shared_ptr<MulticastReceiver> _receiver;
};

class RegistryServiceClient : public Service {
public:
    const char *name() override;

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry& registry) override;

private:
    std::unique_ptr<std::thread> _thread;

    std::shared_ptr<MulticastSender> _sender;
};


#endif //ROVER_REGISTRYSERVICE_H
