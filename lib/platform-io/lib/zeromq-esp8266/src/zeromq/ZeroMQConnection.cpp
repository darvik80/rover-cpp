//
// Created by Ivan Kishchenko on 01.09.2021.
//

#include "ZeroMQConnection.h"
#include "ZeroMQReader.h"

#include <HardwareSerial.h>

size_t ZeroMQDataMessage::ack(size_t len) {
    // If the whole message is now acked...
    if (_acked + len > _buf->size()) {
        // Return the number of extra bytes acked (they will be carried on to the next message)
        const size_t extra = _acked + len - _buf->size();
        _acked = _buf->size();
        return extra;
    }
    // Return that no extra bytes left.
    _acked += len;
    return 0;
}

size_t ZeroMQDataMessage::send(AsyncClient *client) {
    const size_t len = _buf->size() - _sent;
    if (client->space() < len) {
        return 0;
    }
    size_t sent = client->add((const char *) _buf->data(), len);
    if (client->canSend()) {
        client->send();
    }

    _sent += sent;
    return sent;
}

ZeroMQDataMessage::ZeroMQDataMessage(std::shared_ptr<ZeroMQCharBuf> &buf)
        : _buf(std::move(buf)) {
}

ZeroMQConnection::ZeroMQConnection(AsyncClient *client)
        : _serverMode{true}, _client(client), _state{ZeroMQStatus::ZMQ_Idle} {
    _dec.onCommand([this](const ZeroMQCommand &cmd) {
        onCommand(cmd);
    });

    _dec.onMessage([this](const ZeroMQMessage &msg) {
        onMessage(msg);
    });

    _topics.emplace_back("joystick");
}

void ZeroMQConnection::onConnect() {
    _state = ZeroMQStatus::ZMQ_Idle;
    std::shared_ptr<ZeroMQCharBuf> buf(new ZeroMQBufFix<64>());
    std::ostream out(buf.get());
    ZeroMQGreeting reply(true);
    out << reply;

    send(buf);
}

void ZeroMQConnection::onError(int8_t error) {
    Serial.printf("%s: conn error: %s \n", _client->remoteIP().toString().c_str(), _client->errorToString(error));
}

void ZeroMQConnection::onData(void *data, size_t len) {
//    Serial.printf("%s: recv: %d\n", _client->remoteIP().toString().c_str(), len);
//    Serial.println(ZeroMQUtils::netDump((uint8_t *) data, len).c_str());

    _inc.sputn((const char *) data, (std::streamsize) len);
    onData(_inc);
}

void ZeroMQConnection::onData(ZeroMQCharBuf &incBuf) {
    while (incBuf.in_avail()) {
        switch (_state) {
            case ZeroMQStatus::ZMQ_Idle:
            case ZeroMQStatus::ZMQ_S_Wait_Greeting: {
                if (_inc.in_avail() < 64) {
                    return;
                }
                ZeroMQReader inc(incBuf);
                std::array<uint8_t, 64> greeting{};
                inc.readBinary<uint8_t, 64>(greeting);
                _inc.consume(64);
                _version.major = greeting[10];
                _version.minor = greeting[11];
                if (_version.major != 0x03) {
                    Serial.printf("%s: unsupported version: %d:%d\n", getRemoteAddress().c_str(), _version.major, _version.minor);
                    _client->close();
                    return;
                }
                Serial.printf("%s: version: %d:%d\n", getRemoteAddress().c_str(), _version.major, _version.minor);

                ZeroMQCommand reply(ZERO_MQ_CMD_READY);
                reply.props.emplace(ZERO_MQ_PROP_SOCKET_TYPE, "SUB");

                std::shared_ptr<ZeroMQCharBuf> out(new ZeroMQBufFix<64>());
                _enc.write(*out, reply);
                send(out);

                _state = ZeroMQStatus::ZMQ_Stream;
                break;
            }
            case ZeroMQStatus::ZMQ_Stream: {
                if (_dec.read(incBuf)) {
                    return;
                }
            }
                break;
            default:
                break;
        }
    }
}

void ZeroMQConnection::onDisconnect() {
}

void ZeroMQConnection::onTimeOut(uint32_t time) {
}

void ZeroMQConnection::onPool() {
    runQueue();
}

void ZeroMQConnection::onAck(size_t len) {
    if (!_out.empty()) {
        _out.front()->ack(len);
    }

    onPool();
}

void ZeroMQConnection::send(std::shared_ptr<ZeroMQCharBuf> &msg) {
//    Serial.printf("send: %d\n", msg->size());
//    Serial.println(msg->dump().c_str());

    _out.emplace_back(new ZeroMQDataMessage(msg));
    if (_client->canSend()) {
        runQueue();
    }
}

void ZeroMQConnection::runQueue() {
    while (!_out.empty() && _out.front()->finished()) {
        _out.remove(_out.front());
    }

    for (auto &buf: _out) {
        if (!buf->sent()) {
            buf->send(_client);
        }
    }
}

void ZeroMQConnection::onCommand(const ZeroMQCommand &cmd) {
    Serial.printf("%s: cmd %s\n", getRemoteAddress().c_str(), cmd.getName().c_str());
    if (cmd.getName() == ZERO_MQ_CMD_READY) {
        // subscribe = %x00 short-size %d1 subscription
        for (auto &topic: _topics) {
            std::shared_ptr<ZeroMQCharBuf> out(new ZeroMQBufFix<64>());
            if (_version.minor == 0x00) {
                std::ostream str(out.get());
                str << (uint8_t) 0x00 << (uint8_t) (1 + 8) << (uint8_t) 0x01 << topic;
            } else if (_version.minor == 0x01) {
                ZeroMQCommand sub(ZERO_MQ_CMD_SUBSCRIBE);
                sub.props.emplace(ZERO_MQ_PROP_SUBSCRIPTION, topic);
                _enc.write(*out, sub);
            }
            send(out);
        }
    }
}

void ZeroMQConnection::onMessage(const ZeroMQMessage &msg) {
    if (msg.data.size() == 2 && _topicEventHandler) {
        Serial.printf("%s: sub: %s:%s\n", getRemoteAddress().c_str(), msg.data.front().c_str(), msg.data.back().c_str());
        _topicEventHandler(ZeroMQTopicEvent{msg.data.front(), msg.data.back()});
        return;
    }
    for (const auto &item: msg.data) {
        Serial.printf("%s: msg %s\n", getRemoteAddress().c_str(), item.c_str());
    }
}
