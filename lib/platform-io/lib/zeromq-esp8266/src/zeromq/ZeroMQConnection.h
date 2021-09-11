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
    std::shared_ptr<ZeroMQCharBuf> _buf;
    size_t _acked{};
    size_t _sent{};
public:
    explicit ZeroMQDataMessage(std::shared_ptr<ZeroMQCharBuf> &buf);

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
    ZeroMQVersion _version{};
    ZeroMQTopicEventHandler _topicEventHandler{nullptr};

    bool _serverMode{false};
    AsyncClient *_client;

    ZeroMQBufFix<1024> _inc;
    ZeroMQStatus _state;

    etl::list<std::unique_ptr<ZeroMQDataMessage>, 32> _out;

    ZeroMQEncoder _enc;
    ZeroMQDecoder _dec;

    etl::vector<std::string, 3> _topics;
private:
    void runQueue();

    void send(std::shared_ptr<ZeroMQCharBuf>& msg);

    void onData(ZeroMQCharBuf& inc);
    void onCommand(const ZeroMQCommand& cmd);
    void onMessage(const ZeroMQMessage& msg);
public:
    explicit ZeroMQConnection(AsyncClient *client);

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
