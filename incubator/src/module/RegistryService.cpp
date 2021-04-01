//
// Created by Kishchenko, Ivan on 3/24/21.
//

#include "RegistryService.h"
#include "MulticastMessage.h"

#include "module/raspberry/BoostMulticast.h"

#include <iostream>

std::error_code RegistryService::create(Content &content) {
    _server = std::make_shared<AsyncUdp>(
            content.service,
            content.config.network.registryPort
    );

    _server->onPacket([this](const Packet &packet) {
        std::cout << "srv: " << std::string((char *) packet.data, packet.size) << std::endl;
        _server->send(packet.handler, packet.data, packet.size);
    });


    _receiver = std::make_shared<BoostMulticastReceiver>(*content.service, "0.0.0.0", "239.255.0.1", 12345);

    _receiver->receive([](std::string_view message, const SenderAddress &address) {
        auto res = from_json<MulticastMessage>(message);
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
    _sender = std::make_shared<BoostMulticastSender>(*content.service, "239.255.0.1", 12345);

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

            _sender->multicast(to_json<MulticastMessage>(message));
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