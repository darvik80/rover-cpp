//
// Created by Ivan Kishchenko on 01.09.2021.
//

#include <HardwareSerial.h>
#include "ZeroMQServer.h"
#include "ZeroMQUtils.h"

ZeroMQServer::ZeroMQServer() {}

void ZeroMQServer::start(uint16_t port) {
    shutdown();
    _server = new AsyncServer(port);
    _server->onClient([this](void* arg, AsyncClient* client) {
        onNewClient(client);
    }, nullptr);
    _server->begin();

}

void ZeroMQServer::onNewClient(AsyncClient* client) {
    Serial.printf("%s: connected\n", client->remoteIP().toString().c_str());

    auto conn = new ZeroMQConnection(client);
    if (_clients.full()) {
        Serial.printf("%s destroy\n", _clients.front()->getRemoteAddress().c_str());
        _clients.front()->close();
    }
    _clients.emplace_back(conn);

    conn->onTopicEvent(_topicEventHandler);

    conn->onConnect();

    client->setRxTimeout(0);
    client->onError([](void* arg, AsyncClient* client, int8_t error) {
        auto conn = (ZeroMQConnection*)arg;
        conn->onError(error);
    }, conn);

    client->onTimeout([](void* arg, AsyncClient* client, uint32_t time) {
        auto conn = (ZeroMQConnection*)arg;
        conn->onTimeOut(time);
    }, conn);

    client->onAck([](void* arg, AsyncClient* client, size_t len, uint32_t time) {
        auto conn = (ZeroMQConnection*)arg;
        conn->onAck(len);
    }, conn);

//    client->onPoll([this](void* arg, AsyncClient* client) {
//    }, conn);

    client->onData([](void* arg, AsyncClient* client, void *data, size_t len) {
        auto conn = (ZeroMQConnection*)arg;
        conn->onData(data, len);
    }, conn);

    client->onDisconnect([this](void* arg, AsyncClient* client) {
        this->onDestroyClient((ZeroMQConnection*)arg, client);
    }, conn);
}

void ZeroMQServer::onDestroyClient(ZeroMQConnection* conn, AsyncClient* client) {
    Serial.printf("%s: disconnected\n", client->remoteIP().toString().c_str());
    _clients.remove_if([conn](const ZeroMQConnection* value) -> bool {
        return value == conn;
    });
    conn->onDisconnect();
    delete conn;
    delete client;
}

void ZeroMQServer::shutdown() {
    if (_server) {
        _server->end();
        delete _server;
    }
}
