//
// Created by Ivan Kishchenko on 29.08.2021.
//

#ifdef RASPBERRY_ARCH

#include "ZeroMQSocket.h"
#include "ZeroMQLogger.h"
#include "zeromq/ZeroMQWriter.h"
#include "zeromq/ZeroMQReader.h"
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
    _inc.consume(_inc.size());
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

            std::unique_ptr<ZeroMQBuf<>> buf(new ZeroMQBufFix<64>());
            std::ostream out(buf.get());
            ZeroMQGreeting greeting(false);
            out << greeting;
            startSend(buf);
            _status = ZeroMQStatus::ZMQ_C_Wait_Header;

            startRecv();
        }
    });
}

void ZeroMQSocket::onCommand(const ZeroMQCommand &cmd) {
    zero_mq::log::info("handle cmd: {}", cmd.getName());
    if (cmd.getName() == ZERO_MQ_CMD_READY) {
        ZeroMQEncoder enc;

        std::unique_ptr<ZeroMQBuf<>> out(new ZeroMQBufFix<64>());

        ZeroMQCommand reply(ZERO_MQ_CMD_READY);
        reply.props.emplace(ZERO_MQ_PROP_IDENTIFY, "joystick");
        reply.props.emplace(ZERO_MQ_PROP_SOCKET_TYPE, "PUB");
        enc.write(*out, reply);
        startSend(out);
        _timer.schedule(boost::posix_time::seconds{5}, [this]() {
            onIdle();
        });
        publish("joystick", "ready");
    } else if (cmd.getName() == ZERO_MQ_CMD_SUBSCRIBE) {
        zero_mq::log::info("subscribe: {}", cmd.props.at(ZERO_MQ_PROP_SUBSCRIPTION));
        _topics.emplace(cmd.props.at(ZERO_MQ_PROP_SUBSCRIPTION));
    } else if (cmd.getName() == ZERO_MQ_CMD_CANCEL) {
        zero_mq::log::info("cancel: {}", cmd.props.at(ZERO_MQ_PROP_SUBSCRIPTION));
        _topics.erase(cmd.props.at(ZERO_MQ_PROP_SUBSCRIPTION));
    }
}

void ZeroMQSocket::onMessage(const ZeroMQMessage &msg) {
    for (auto item: msg.data) {
        zero_mq::log::info("msg: {}", item);
    }
}

void ZeroMQSocket::handleData(ZeroMQCharBuf &incBuf) {
    while (incBuf.size()) {
        zero_mq::log::info("inc size: {}:{}", incBuf.size(), incBuf.in_avail());
        switch (_status) {
            case ZeroMQStatus::ZMQ_C_Wait_Header: {
                if (incBuf.size() < 10) {
                    return;
                }
                const uint8_t hdr[] = {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x7f};
                if (std::memcmp(_inc.data(), hdr, 9) != 0) {
                    startConnect();
                    return;
                }
                _status = ZeroMQStatus::ZMQ_C_Wait_Greeting;
            }
            case ZeroMQStatus::ZMQ_C_Wait_Greeting: {
                if (incBuf.size() < 64) {
                    return;
                }
                ZeroMQReader inc(incBuf);
                std::array<uint8_t, 64> greeting{};
                inc.readBinary<uint8_t, 64>(greeting);
                incBuf.consume(64);

                _status = ZeroMQStatus::ZMQ_Stream;
            }
                break;
            case ZeroMQStatus::ZMQ_Stream: {
                ZeroMQDecoder dec;
                dec.onCommand([this](const ZeroMQCommand &cmd) {
                    onCommand(cmd);
                });
                dec.onMessage([this](const ZeroMQMessage &msg) {
                    onMessage(msg);
                });
                while (dec.read(incBuf)) {}
            }
                return;
            default:
                startConnect();
                break;
        }
    }
}

void ZeroMQSocket::startRecv() {
    _socket.async_receive(
            boost::asio::buffer(_incBuf.data(), _incBuf.size()),
            [this](const boost::system::error_code &err, std::size_t size) {
                if (!err) {
                    zero_mq::log::info("socker recv: {}\n{}", size, ZeroMQUtils::netDump((const uint8_t *) _incBuf.data(), size));

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

void ZeroMQSocket::startSend(std::unique_ptr<ZeroMQCharBuf> &buf) {
    zero_mq::log::info("socket send:\n{}", ZeroMQUtils::netDump((const uint8_t *) buf->data(), buf->size()));
    async_write(_socket, boost::asio::buffer(buf->data(), buf->size()), [this, buf = std::move(buf)](const boost::system::error_code &err, std::size_t size) {
        if (err) {
            zero_mq::log::info("send failed: {}", err.message());
            startConnect();
        } else {
            zero_mq::log::info("sent: {}:{}", size, buf->size());
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
        ZeroMQEncoder enc;
        std::unique_ptr<ZeroMQBuf<>> buf(new ZeroMQBufFix<256>());

        ZeroMQMessage msg;
        msg << topic.data() << data.data();

        if (!enc.write(*buf, msg)) {
            startSend(buf);
        }
    }
}

#endif