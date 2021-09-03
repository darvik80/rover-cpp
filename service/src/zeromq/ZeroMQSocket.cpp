//
// Created by Ivan Kishchenko on 29.08.2021.
//

#include "ZeroMQSocket.h"
#include "ZeroMQLogger.h"
#include <boost/lexical_cast.hpp>

using namespace boost;
using namespace boost::asio;

ZeroMQSocket::ZeroMQSocket(boost::asio::io_service &service, std::string_view address, int port)
        : _socket(service), _timer(service) {
    ip::tcp::resolver::query resolver_query(address.data(), lexical_cast<std::string>(port));
    boost::asio::ip::tcp::resolver resolver(service);
    auto iterator = resolver.resolve(resolver_query);
    _endpoint = *iterator;
    startConnect();
}

void ZeroMQSocket::startConnect() {
    _inc.consume(_inc.in_avail());
    _timer.cancel();
    _status = ZeroMQStatus::ZMQ_Idle;
    _socket.close();
    _socket.async_connect(_endpoint, [this](const boost::system::error_code &err) {
        if (err) {
            zero_mq::log::warning("connect failed: {}", err.message());
            _timer.schedule(boost::posix_time::seconds{5}, [this]() {
                startConnect();
            });
        } else {
            zero_mq::log::info("connected");

            auto buf = std::make_unique<boost::asio::streambuf>();
            std::ostream out(buf.get());
            ZeroMQGreeting greeting(false);
            out << greeting;
            startSend(buf);
            _status = ZeroMQStatus::ZMQ_C_Wait_Header;

            startRecv();
        }
    });
}

void ZeroMQSocket::onCommand(ZeroMQCommand &cmd) {
    zero_mq::log::info("handle cmd: {}", cmd.getName());
    if (cmd.getName() == ZERO_MQ_CMD_SUBSCRIBE) {
        zero_mq::log::info("subscribe: {}", cmd.getProperty(ZERO_MQ_PROP_SUBSCRIPTION));
        _topics.emplace(cmd.getProperty(ZERO_MQ_PROP_SUBSCRIPTION));
    } else if (cmd.getName() == ZERO_MQ_CMD_CANCEL) {
        zero_mq::log::info("cancel: {}", cmd.getProperty(ZERO_MQ_PROP_SUBSCRIPTION));
        _topics.erase(cmd.getProperty(ZERO_MQ_PROP_SUBSCRIPTION));
    }
}

void ZeroMQSocket::handleData(boost::asio::streambuf &incBuf) {
    std::istream inc(&incBuf);
    while (incBuf.size()) {
        zero_mq::log::info("iter size: {}", incBuf.size());
        auto buf = std::make_unique<boost::asio::streambuf>();
        std::ostream out(buf.get());

        switch (_status) {
            case ZeroMQStatus::ZMQ_C_Wait_Header: {
                if (_inc.size() < 10) {
                    return;
                }
                const uint8_t hdr[] = {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x7f};
                if (std::memcmp(_inc.data().data(), hdr, 9) != 0) {
                    startConnect();
                    return;
                }
                _status = ZeroMQStatus::ZMQ_C_Wait_Greeting;
            }
            case ZeroMQStatus::ZMQ_C_Wait_Greeting: {
                if (_inc.size() < 64) {
                    return;
                }
                ZeroMQGreeting greeting(false);
                inc >> greeting;
                if (!inc) {
                    startConnect();
                    return;
                }

                ZeroMQCommand cmd(ZERO_MQ_CMD_READY);
                cmd.setProperty(ZERO_MQ_PROP_IDENTIFY, "joystick");
                cmd.setProperty(ZERO_MQ_PROP_SOCKET_TYPE, "PUB");
                out << cmd;
                startSend(buf);
                _status = ZeroMQStatus::ZMQ_C_Wait_Ready;
            }
                break;
            case ZeroMQStatus::ZMQ_C_Wait_Ready: {
                ZeroMQCommand cmd;
                inc >> cmd;
                if (inc) {
                    _status = ZeroMQStatus::ZMQ_Stream;
                    _timer.schedule(boost::posix_time::seconds{5}, [this]() {
                        onIdle();
                    });
                }
            }
                break;
            case ZeroMQStatus::ZMQ_Stream:
                if (_inc.sgetc() & flag_cmd) {
                    ZeroMQCommand cmd;
                    inc >> cmd;
                    if (inc) {
                        onCommand(cmd);
                    }

                }
                return;
            default:
                startConnect();
                break;
        }
    }
}

void ZeroMQSocket::startRecv() {
    _socket.async_read_some(
            boost::asio::buffer(_incBuf),
            [this](const boost::system::error_code &err, std::size_t size) {
                if (!err) {
                    zero_mq::log::info("socker recv:\n{}", ZeroMQUtils::netDump((const uint8_t *) _incBuf.data(), size));

                    _inc.sputn(_incBuf.data(), size);
                    handleData(_inc);
                    startRecv();
                } else {
                    zero_mq::log::info("recv failed: {}", err.message());
                    startConnect();
                }
            }
    );
}

void ZeroMQSocket::startSend(std::unique_ptr<boost::asio::streambuf> &buf) {
    zero_mq::log::info("socket send:\n{}", ZeroMQUtils::netDump((const uint8_t *) buf->data().data(), buf->data().size()));
    async_write(_socket, *buf, [this, buf = std::move(buf)](const boost::system::error_code &err, std::size_t size) {
        if (err) {
            zero_mq::log::info("send failed: {}", err.message());
            startConnect();
        } else {
            zero_mq::log::info("sent: {}", size);
        }
    });
}

void ZeroMQSocket::onIdle() {
    publish("joystick", "Control Message");
    publish("joystick1", "skip");
    publish("joystick", "Axis");
    _timer.schedule(boost::posix_time::seconds{5}, [this]() {
        onIdle();
    });
}

void ZeroMQSocket::publish(std::string_view topic, std::string_view data) {
    if (_topics.find(topic.data()) != _topics.end()) {
        auto buf = std::make_unique<boost::asio::streambuf>();
        std::ostream out(buf.get());

        ZeroMQMessage msg;
        msg << topic << data;
        out << msg;
        if (out) {
            startSend(buf);
        }
    }
}