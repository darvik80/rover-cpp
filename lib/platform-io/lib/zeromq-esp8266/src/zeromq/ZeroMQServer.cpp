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
    Serial.printf("Client connected: %s:%d\n", client->remoteIP().toString().c_str(), client->remotePort());

    auto conn = new ZeroMQConnection(client);
    if (_clients.full()) {
        Serial.printf("Client destroy: %s:%d\n", _clients.front()->getRemoteAddress().c_str(), _clients.front()->getRemotePort());
        _clients.front()->close();
        // prepare place for new connection
        Serial.println(1);
    }
    Serial.println(3);
    _clients.emplace_back(conn);

    Serial.println(4);

    conn->onTopicEvent(_topicEventHandler);

    Serial.println(5);

    conn->onConnect();

    client->setRxTimeout(0);
    client->onError([](void* arg, AsyncClient* client, int8_t error) {
        auto conn = (ZeroMQConnection*)arg;
        conn->onError(error);
        Serial.printf("Client error: %d, %s:%d\n", error, client->remoteIP().toString().c_str(), client->remotePort());
    }, conn);

    client->onTimeout([](void* arg, AsyncClient* client, uint32_t time) {
        Serial.printf("Client timeout: %s:%d\n", client->remoteIP().toString().c_str(), client->remotePort());
        auto conn = (ZeroMQConnection*)arg;
        conn->onTimeOut(time);
    }, conn);

    client->onAck([](void* arg, AsyncClient* client, size_t len, uint32_t time) {
        auto conn = (ZeroMQConnection*)arg;
        conn->onAck(len);
    }, conn);

    client->onPoll([this](void* arg, AsyncClient* client) {
//        auto conn = (ZeroMQConnection*)arg;
//        //Serial.printf("Client onPool: %s:%d\n", client->remoteIP().toString().c_str(), client->remotePort());
//        //conn->onPool();
//        bool handle = false;
//        for (const auto &item : _clients) {
//            if (item == conn) {
//                conn->onPool();
//                handle = true;
//            }
//        }
//        if (!handle) {
//            Serial.println("UNHANDLED ON POOL!!!");
//        }
    }, conn);

    client->onData([](void* arg, AsyncClient* client, void *data, size_t len) {
        auto conn = (ZeroMQConnection*)arg;
        conn->onData(data, len);
    }, conn);

    client->onDisconnect([this](void* arg, AsyncClient* client) {
        this->onDestroyClient((ZeroMQConnection*)arg, client);
    }, conn);
}

void ZeroMQServer::onDestroyClient(ZeroMQConnection* conn, AsyncClient* client) {
    Serial.printf("Client disconnected: %s:%d\n", client->remoteIP().toString().c_str(), client->remotePort());
    _clients.remove_if([conn](const ZeroMQConnection* value) -> bool {
        Serial.printf("Client for destroy: %d\n", value == conn);
        return value == conn;
    });
    conn->onDisconnect();
    delete conn;
    Serial.printf("conn destroyed\n");
    delete client;
    Serial.printf("Client destroyed\n");
}

void ZeroMQServer::shutdown() {
    if (_server) {
        _server->end();
        delete _server;
    }
}
