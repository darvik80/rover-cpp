//
// Created by Kishchenko, Ivan on 3/24/21.
//

#include "RegistryService.h"
#include "MulticastMessage.h"

#include "MulticastFactory.h"

#include <iostream>
#include <raspberry/BoostMulticastMessage.h>

std::error_code RegistryService::create(Content &content) {
    _server = std::make_shared<AsyncUdp>(
            content.service,
            content.config.network.registryPort
    );

    _server->onPacket([this](const Packet &packet) {
        std::cout << "srv: " << std::string((char *) packet.data, packet.size) << std::endl;
        _server->send(packet.handler, packet.data, packet.size);
    });

    Context context{content.service};
    _receiver = MulticastFactory::createReceiver(context, "239.255.0.1", 12345);

    _receiver->receive([](std::string_view message, const SenderAddress &address) {
        auto res = fromJson<MulticastMessage>(message);
        std::cout << "recv from: " << address.getHost() << ":" << address.getPort() << std::endl;
        std::cout << "data: " << message << std::endl;
        std::cout << "----" << std::endl;
    });

    return std::error_code();
}

std::error_code RegistryService::destroy() {
    _receiver.reset();
    return std::error_code();
}

ModuleStatus RegistryService::getStatus() {
    return DOWN;
}


std::error_code RegistryServiceClient::create(Content &content) {
    Context context{content.service};
    _sender = MulticastFactory::createSender(context, "239.255.0.1", 12345);

    _thread = std::make_unique<std::thread>([content, this]() {
        Transport::Ptr client = std::make_shared<AsyncUdp>(
                content.service,
                content.config.network.registryHost,
                content.config.network.registryPort
        );

        auto handler = client->connect();

        for (int i = 0; i < 100; i++) {
//            client->onPacket([](const Packet &packet) {
//                std::cout << "cln: " << std::string((const char *) packet.data, packet.size) << std::endl;
//            });
//            client->send(handler, "Hello World");

            MulticastMessage message{"1", "raspberry", "monitor"};

            _sender->multicast(toJson<MulticastMessage>(message));
            sleep(1);
        }
    });

    return std::error_code();
}

std::error_code RegistryServiceClient::destroy() {
    return std::error_code();
}

ModuleStatus RegistryServiceClient::getStatus() {
    return DOWN;
}