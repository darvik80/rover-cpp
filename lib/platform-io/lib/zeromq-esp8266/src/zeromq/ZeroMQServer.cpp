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
    auto conn = new ZeroMQConnection(client);

    conn->onConnect();

    client->onError([conn](void* arg, AsyncClient* client, int8_t error) {
        conn->onError(error);
    }, conn);

    client->onTimeout([conn](void* arg, AsyncClient* client, uint32_t time) {
        conn->onTimeOut(time);
    }, conn);

    client->onAck([conn](void* arg, AsyncClient* client, size_t len, uint32_t time) {
        conn->onAck(len);
    }, conn);

    client->onPoll([conn](void* arg, AsyncClient* client) {
        conn->onPool();
    }, conn);

    client->onData([conn](void* arg, AsyncClient* client, void *data, size_t len) {
        conn->onData(data, len);
    }, conn);

    client->onDisconnect([conn](void* arg, AsyncClient* client) {
        conn->onDisconnect();
        delete conn;
    }, conn);
}

void ZeroMQServer::shutdown() {
    if (_server) {
        _server->end();
        delete _server;
    }
}
