//
// Created by Ivan Kishchenko on 01.09.2021.
//

#pragma once


#include <ESPAsyncTCP.h>
#include <cstdint>
#include <etl/list.h>

#include "ZeroMQProtocol.h"
#include "ZeroMQTopicEvent.h"

class ZeroMQDataMessage {
    std::unique_ptr<ZeroMQBuf<> > _buf;
    size_t _acked{};
    size_t _sent{};
public:
    explicit ZeroMQDataMessage(std::unique_ptr<ZeroMQBuf<>> &buf);

    size_t ack(size_t len);

    size_t send(AsyncClient *client);

    bool finished() { return _acked == _buf->size(); }

    bool sent() { return _sent == _buf->size(); }
};

enum class ZeroMQStatus {
    ZMQ_Idle,
    ZMQ_C_Wait_Header,
    ZMQ_C_Wait_Greeting,
    ZMQ_C_Wait_Ready,

    ZMQ_S_Wait_Header,
    ZMQ_S_Wait_Greeting,
    ZMQ_S_Wait_Ready,

    ZMQ_Stream,
};

class ZeroMQConnection {
    ZeroMQTopicEventHandler _topicEventHandler{nullptr};

    bool _serverMode{false};
    AsyncClient *_client;

    ZeroMQBufFix<256> _inc;
    ZeroMQStatus _state;

    etl::list<std::unique_ptr<ZeroMQDataMessage>, 32> _out;
private:
    void runQueue();

    void send(std::unique_ptr<ZeroMQBuf<>>& msg);

    void onCommand(ZeroMQCommand& cmd);
    void onMessage(ZeroMQMessage& msg);
public:
    explicit ZeroMQConnection(AsyncClient *client)
            : _serverMode{true}, _client(client), _state{ZeroMQStatus::ZMQ_Idle} {}

    void onTopicEvent(ZeroMQTopicEventHandler topicEventHandler) {
        _topicEventHandler = topicEventHandler;
    }

    void onConnect();

    void onError(int8_t error);

    void onData(void *data, size_t len);

    void onAck(size_t len);

    void onPool();

    void onDisconnect();

    void onTimeOut(uint32_t time);

    String getLocalAddress() {
        return _client->localIP().toString();
    }

    uint16_t getLocalPort() {
        return _client->localPort();
    }

    String getRemoteAddress() {
        return _client->remoteIP().toString();
    }

    uint16_t getRemotePort() {
        return _client->remotePort();
    }

    void close() {
        if (_client) {
            _client->close(true);
        }
    }
};
