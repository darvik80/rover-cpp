//
// Created by Kishchenko, Ivan on 3/24/21.
//

#include "RegistryService.h"
#include "net/MulticastMessage.h"

#include "net/BoostMulticastFactory.h"

#include <iostream>
#include <net/BoostMulticastMessage.h>
#include <properties/NetworkProperties.h>

const char *RegistryService::name() {
    return "net_registry";
}

void RegistryService::postConstruct(Registry &registry) {
    auto server = std::make_shared<AsyncUdp>(registry.getIoService());

    auto &props = registry.getProperties<NetworkProperties>();

    server->listen(props.registryPort);

    server->onPacket([server](const Packet &packet) {
        std::cout << "srv: " << std::string((char *) packet.data, packet.size) << std::endl;
        server->send(packet.handler, packet.data, packet.size);
    });

    BoostMulticastFactory factory(registry.getIoService());
    _receiver = factory.createReceiver("239.255.0.1", 12345);

    _receiver->receive([](std::string_view message, const SenderAddress &address) {
        auto res = fromJson<MulticastMessage>(message);
        std::cout << "recv from: " << address.getHost() << ":" << address.getPort() << std::endl;
        std::cout << "data: " << message << std::endl;
        std::cout << "----" << std::endl;
    });
}

void RegistryService::run() {
}

void RegistryService::preDestroy() {
    _receiver.reset();
}

const char *RegistryServiceClient::name() {
    return "registry_client";
}

void RegistryServiceClient::postConstruct(Registry &registry) {
    BoostMulticastFactory factory(registry.getIoService());
    _sender = factory.createSender("239.255.0.1", 12345);

    _thread = std::make_unique<std::thread>([&registry, this]() {
        auto client = std::make_shared<AsyncUdp>(registry.getIoService());
        auto &props = registry.getProperties<NetworkProperties>();

        auto handler = client->connect(
                props.registryHost,
                props.registryPort
        );

        for (int i = 0; i < 100; i++) {
            MulticastMessage message{"1", "raspberry", "monitor"};

            _sender->multicast(toJson<MulticastMessage>(message));
            sleep(1);
        }
    });
}

void RegistryServiceClient::run() {

}

void RegistryServiceClient::preDestroy() {

}
