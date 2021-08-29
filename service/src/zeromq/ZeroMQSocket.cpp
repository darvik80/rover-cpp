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
    _timer.cancel();
    _status = ZMQ_Idle;
    _socket.close();
    _socket.async_connect(_endpoint, [this](const boost::system::error_code &err) {
        if (err) {
            zero_mq::log::warning("connect failed: {}", err.message());
            startConnect();
        } else {
            zero_mq::log::info("connected");
            startRecv();
        }
    });
}

void ZeroMQSocket::startRecv() {
    _socket.async_read_some(
            boost::asio::buffer(_incBuf),
            [this](const boost::system::error_code &err, std::size_t size) {
                if (!err) {
                    auto buf = std::make_unique<boost::asio::streambuf>();
                    zero_mq::log::info("recv: {}", size);
                    zero_mq::log::info("socker recv:\n{}", ZeroMQUtils::netDump((const uint8_t*)_incBuf.data(), size));
                    std::ostream out(buf.get());

                    switch (_status) {
                        case ZMQ_Idle: {
                            ZeroMQGreeting greeting;
                            out << greeting;
                            startSend(buf);
                            _status = ZMQ_Greeting;
                        }
                            break;
                        case ZMQ_Greeting: {
                            ZeroMQCommand cmd("READY");
                            cmd.setProperty("Identify", "joystick");
                            cmd.setProperty("Socket-Type", "PUB");

                            out << cmd;
                            startSend(buf);
                            _status = ZMQ_Ready;
                        }
                            break;
                        case ZMQ_Ready: {
                            _timer.schedule(boost::posix_time::seconds{5}, [this]() {
                                onIdle();
                            });
                            _status = ZMQ_Stream;
                        }
                            break;
                        case ZMQ_Stream: {

                            _status = ZMQ_Stream;
                        }
                            break;
                        default:
                            break;

                    }
                    startRecv();

                } else {
                    zero_mq::log::info("recv failed: {}", err.message());
                    startConnect();
                }
            }
    );
}

void ZeroMQSocket::startSend(std::unique_ptr<boost::asio::streambuf>& buf) {
    zero_mq::log::info("socker send:\n{}", ZeroMQUtils::netDump((const uint8_t*)buf->data().data(), buf->data().size()));
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
    auto buf = std::make_unique<boost::asio::streambuf>();
    std::ostream out(buf.get());

    ZeroMQMessage msg1;
    msg1 << "joystick";
    msg1 << "Control Message";
    out << msg1;
    ZeroMQMessage msg2;
    msg2 << "joystick1";
    msg2 << "Control";
    out << msg2;
    ZeroMQMessage msg3;
    msg3 << "joystick";
    msg3 << "Axis";
    out << msg3;
    startSend(buf);
    _timer.schedule(boost::posix_time::seconds{5}, [this]() {
        onIdle();
    });
}