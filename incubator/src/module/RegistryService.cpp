//
// Created by Kishchenko, Ivan on 3/24/21.
//

#include "RegistryService.h"

#include <iostream>

std::error_code RegistryService::create(Content &content) {
    _server = std::make_shared<AsyncUdp>(
            content.service,
            content.config.network.registryPort
    );

    _server->onPacket([this](const Packet &packet) {
        std::cout << "srv: " << std::string((char*)packet.data, packet.size) << std::endl;
       _server->send(packet.handler, packet.data, packet.size);
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
        Transport::Ptr client = std::make_shared<AsyncUdp>(
                content.service,
                content.config.network.registryHost,
                content.config.network.registryPort
        );

        auto handler = client->connect();

        while (true) {
            client->onPacket([](const Packet &packet) {
                std::cout << "cln: " << std::string((const char *) packet.data, packet.size) << std::endl;
            });
            client->send(handler, "Hello World");
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