//
// Created by Ivan Kishchenko on 01.09.2021.
//

#include "ZeroMQConnection.h"

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

ZeroMQDataMessage::ZeroMQDataMessage(std::unique_ptr<ZeroMQBuf<>> &buf)
        : _buf(std::move(buf)) {}


void ZeroMQConnection::onConnect() {
    Serial.printf("client %s connected \n", _client->remoteIP().toString().c_str());

    std::unique_ptr<ZeroMQBuf<>> buf(new ZeroMQBufFix<64>());
    std::ostream out(buf.get());
    ZeroMQGreeting reply(true);
    out << reply;

    send(buf);
}

void ZeroMQConnection::onError(int8_t error) {
    Serial.printf("connection error %s from client %s \n", _client->errorToString(error), _client->remoteIP().toString().c_str());
}

void ZeroMQConnection::onData(void *data, size_t len) {
    Serial.printf("recv: %s \n", ZeroMQUtils::netDump((const uint8_t *) data, len).c_str());

    _inc.compress();
    _inc.sputn((const char *) data, (std::streamsize)len);
    std::istream inc(&_inc);

    while (_inc.in_avail()) {
        switch (_state) {
            case ZeroMQStatus::ZMQ_Idle: {
                if (_inc.in_avail() < 11) {
                    return;
                }

                const uint8_t hdrs[3][11] = {
                        {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x03},
                        {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x7f, 0x03},
                        {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x7f, 0x03},
                };

                bool handle = false;
                for (auto hdr: hdrs) {
                    if (std::memcmp(_inc.data(), hdr, 11) == 0) {
                        handle = true;
                    }
                }
                if (!handle) {
                    _client->close(true);
                    return;
                }
                _state = ZeroMQStatus::ZMQ_S_Wait_Greeting;
            }
            case ZeroMQStatus::ZMQ_S_Wait_Greeting: {
                if (_inc.in_avail() < 64) {
                    return;
                }
                ZeroMQGreeting greeting{false};
                inc >> greeting;
//                if (!inc) {
//                    return;
//                }

                if (greeting.isIsServer()) {
                    //_client->close();
                    //return;
                }

                if (greeting.getMechanism() != "NULL") {
                    _client->close();
                    return;
                }

                ZeroMQCommand reply(ZERO_MQ_CMD_READY);
                reply.setProperty(ZERO_MQ_PROP_SOCKET_TYPE, "SUB");
                std::unique_ptr<ZeroMQBuf<>> buf(new ZeroMQBufFix<64>());
                std::ostream out(buf.get());
                out << reply;
                send(buf);

                _state = ZeroMQStatus::ZMQ_S_Wait_Ready;
                break;

            }
            case ZeroMQStatus::ZMQ_S_Wait_Ready: {
                inc.ignore(_inc.in_avail());
                std::unique_ptr<ZeroMQBuf<>> buf(new ZeroMQBufFix<64>());
                std::ostream out(buf.get());
                std::string topic = "joystick";
                out << (uint8_t)00 << (uint8_t)(topic.size() + 1) << (uint8_t)0x01 << topic;
                send(buf);

//                ZeroMQCommand cmd;
//                inc >> cmd;

//                if (inc) {
//                    ZeroMQCommand reply(ZERO_MQ_CMD_READY);
//                    reply.setProperty(ZERO_MQ_PROP_SOCKET_TYPE, "SUB");
//                    std::unique_ptr<ZeroMQBuf<>> buf(new ZeroMQBufFix<64>());
//                    std::ostream out(buf.get());
//                    out << reply;
//                    send(buf);
//                    _state = ZeroMQStatus::ZMQ_Stream;
//                }
                _state = ZeroMQStatus::ZMQ_Stream;
                break;
            }
            case ZeroMQStatus::ZMQ_Stream:
                if (_inc.sgetc() & flag_cmd) {
                    ZeroMQCommand cmd;
                    inc >> cmd;
                    if (inc) {
                        onCommand(cmd);
                    } else {
                        _client->close();
                        return;
                    }

                } else {
                    ZeroMQMessage msg;
                    inc >> msg;
                    if (inc) {
                        onMessage(msg);
                    } else {
                        _client->close();
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
    Serial.printf("client %s disconnected \n", _client->remoteIP().toString().c_str());
}

void ZeroMQConnection::onTimeOut(uint32_t time) {
    Serial.printf("client ACK timeout ip: %s \n", _client->remoteIP().toString().c_str());
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

void ZeroMQConnection::send(std::unique_ptr<ZeroMQBuf<>> &msg) {
    Serial.printf("send: %s \n", msg->dump().c_str());

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

void ZeroMQConnection::onCommand(ZeroMQCommand &cmd) {
    Serial.printf("Cmd %s\n", cmd.getName().c_str());
}

void ZeroMQConnection::onMessage(ZeroMQMessage &msg) {
    for (const auto &item: msg.getData()) {
        Serial.printf("Msg %s\n", item.c_str());
    }
}
