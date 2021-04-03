//
// Created by Kishchenko, Ivan on 3/24/21.
//

#include "RegistryService.h"

#include <iostream>

std::error_code RegistryService::create(Content &content) {
    auto server = std::make_shared<AsyncUdp>(content.service);

    server->listen(content.config.network.registryPort);

    server->onPacket([server](const Packet &packet) {
        std::cout << "srv: " << std::string((char *) packet.data, packet.size) << std::endl;
        server->send(packet.handler, packet.data, packet.size);
    });

    return std::error_code();
}

std::error_code RegistryService::destroy() {
    return std::error_code();
}

ModuleStatus RegistryService::getStatus() {
    return DOWN;
}


std::error_code RegistryServiceClient::create(Content &content) {
    _thread = std::make_unique<std::thread>([content]() {
        auto client = std::make_shared<AsyncUdp>(content.service);

        auto handler = client->connect(
                content.config.network.registryHost,
                content.config.network.registryPort
        );

        while (true) {
            client->onPacket([](const Packet &packet) {
                std::cout << "cln: " << std::string((const char *) packet.data, packet.size) << std::endl;
            });
            const char* msg = "Hello World";
            client->send(handler, (const uint8_t* )msg, strlen(msg));
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