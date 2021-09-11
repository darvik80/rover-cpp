//
// Created by Ivan Kishchenko on 01.09.2021.
//

#pragma once

#include <ESPAsyncTCP.h>
#include <unordered_map>

#include "ZeroMQConnection.h"
#include "ZeroMQTopicEvent.h"
#include <etl/list.h>

class ZeroMQServer {
    ZeroMQTopicEventHandler _topicEventHandler{nullptr};
    AsyncServer* _server{};

    etl::list<ZeroMQConnection*, 1> _clients;
public:
    explicit ZeroMQServer();
    void onTopicEvent(ZeroMQTopicEventHandler topicEventHandler) {
        _topicEventHandler = topicEventHandler;
    }

    void start(uint16_t port);
    void onNewClient(AsyncClient* client);
    void onDestroyClient(ZeroMQConnection* conn, AsyncClient* client);
    void shutdown();
};
