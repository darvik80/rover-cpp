//
// Created by Ivan Kishchenko on 01.09.2021.
//

#pragma once

#include <ESPAsyncTCP.h>
#include <unordered_map>

#include "ZeroMQConnection.h"

class ZeroMQServer {
    AsyncServer* _server{};
public:
    explicit ZeroMQServer();

    void start(uint16_t port);
    void onNewClient(AsyncClient* client);
    void shutdown();
};
