//
// Created by Ivan Kishchenko on 19.09.2020.
//

#include "Connection.h"

#include <utility>
#include "logging/MqttLogger.h"
#include <boost/lexical_cast.hpp>

#include "event/EventChannel.h"

using namespace boost;
using namespace boost::asio;
using namespace mqtt::message;

namespace mqtt {

    DefaultConnection::DefaultConnection(const MqttProperties &props)
            : Component(props.address)
            , _socket(IoServiceHolder::get_mutable_instance())
            , _incBuf(512)
            , _props(props) {
        // TODO: make it async
        ip::tcp::resolver::query resolver_query(props.address, lexical_cast<std::string>(props.port));
        boost::asio::ip::tcp::resolver _resolver(IoServiceHolder::get_mutable_instance());
        auto iterator = _resolver.resolve(resolver_query);
        _endpoint = *iterator;
    }

    void DefaultConnection::start() {
        _shutdown = false;
        startConnect();
    }

    void DefaultConnection::stop() {
        asio::dispatch(IoServiceHolder::get_mutable_instance(), [this]() {
            _shutdown = true;
            _socket.close();
        });
    }

    void DefaultConnection::startConnect() {
        if (_shutdown) {
            return;
        }
        _socket.async_connect(_endpoint, [this](const boost::system::error_code &err) {
            if (!_socket.is_open() || err) {
                if (!_socket.is_open()) {
                    log::warning("[{}] connect timed out", name());
                } else {
                    log::error("[{}] connect failed", name());
                    _socket.close();
                }

                _timer = Timer::create([this]() { this->startConnect(); }, PosixSeconds{10});
            } else {
                log::debug("[{}] connected", name());
                _status = ACTIVE;

                startRead();
                this->channelActive();
            }

        });
    }

    void DefaultConnection::startRead() {
        _socket.async_read_some(
                boost::asio::buffer(_incBuf),
                [this](const boost::system::error_code &err, std::size_t size) {
                    this->channelReadComplete(err, size);
                }
        );
    }

    void DefaultConnection::onMessage(const message::Message::Ptr &msg) {
        _lastUpdate = boost::posix_time::second_clock::local_time();
        log::debug("[{}] recv: {}", name(), msg->getTypeStr());
        raiseEvent(EventChannelMessage{*this, msg});
        switch (msg->getType()) {
            case MQTT_MSG_CONNACK: {
                auto ack = (ConnAckMessage *) msg.get();
                if (ack->getReasonCode()) {
                    log::error("[{}] session failed: {}", name(), ack->getReasonCodeDescription());
                    _socket.close();
                } else {
                    _status = CONNECTED;
                    log::info("[{}] session is active", name());
                    raiseEvent(EventChannelActive{*this});
                    _timer = Timer::create(
                            [this]() {
                                PosixTime now = boost::posix_time::second_clock::local_time();
                                if ((now - _lastUpdate) > PosixSeconds{10}) {
                                    _socket.close();
                                } else {
                                    post(std::make_shared<PingReqMessage>());
                                }
                            },
                            PosixSeconds{5}
                    );
                    return;
                }
            }
                break;
            case MQTT_MSG_PINGRESP:
                return;
            case MQTT_MSG_DISCONNECT:
                _status = IDLE;
                return;
            case MQTT_MSG_PUBLISH: {
                auto pub = (PublishMessage *) msg.get();
                if (pub->getQos() >= message::QOS_AT_LEAST_ONCE) {
                    post(std::make_shared<message::PubAckMessage>(pub->getPacketIdentifier()));
                }
            }
                break;
        }

        if (_status == CONNECTED) {
            _timer->reset();
        }
    }

    void DefaultConnection::channelActive() {
        _timer = Timer::create([this]() { this->_socket.close(); }, PosixSeconds{5});

        log::info("[{}] channel is active", name());

        auto msg = std::make_shared<ConnectMessage>(_props.clientName);
        msg->setUserName(_props.username);
        msg->setPassword(_props.password);
        post(msg);
    }

    void DefaultConnection::channelInactive(const ErrorCode &err) {
        if (err) {
            log::error("[{}] channel is inactive, {}", name(), err.message());

            _socket.close();
            asio::dispatch(IoServiceHolder::get_mutable_instance(), [this]() {
                startConnect();
            });
        } else {
            log::info("[{}] channel is active", name());
        }
        raiseEvent(EventChannelInactive{*this, err});
    }

    void DefaultConnection::channelReadComplete(const ErrorCode &err, size_t readSize) {
        if (err) {
            log::warning("[{}] read failed, {}", name(), err.message());
            channelInactive(err);
            return;
        }

        if (readSize) {
            _inc.sputn((const char *) _incBuf.data(), readSize);

            try {
                while (_inc.size()) {
                    onMessage(_decoder.decode(_inc));
                }
            } catch (std::exception &ex) {
                // catch segmented messages
            }
        }

        startRead();
    }

    VoidFuture DefaultConnection::post(const message::Message::Ptr &msg) {
        auto promise = std::make_shared<VoidPromise>();

        asio::dispatch(IoServiceHolder::get_mutable_instance(), [this, msg, promise]() {
            if (msg->getType() != MQTT_MSG_CONNECT && _status != CONNECTED) {
                log::info("[{}] {} ignored", name(), msg->getTypeStr());
                promise->set_exception(system::system_error(boost::asio::error::operation_aborted));
                return;
            }
            log::debug("[{}] send: {}", name(), msg->getTypeStr());

            // TODO remove it
            auto identifier = dynamic_cast<MessagePacketIdentifier *>(msg.get());
            if (identifier && identifier->getPacketIdentifier() == 0) {
                identifier->setPacketIdentifier(++_packetIdentifier);
            }
            _encoder.encode(_out, msg);

            async_write(_socket, _out, [this, promise](const ErrorCode &errorCode, std::size_t size) mutable {
                if (errorCode) {
                    promise->set_exception(system::system_error(errorCode));
                } else {
                    promise->set_value();
                }
                channelWriteComplete(errorCode, size);
            });
        });

        return promise->get_future();
    }

    void DefaultConnection::channelWriteComplete(const ErrorCode &err, size_t writeSize) {
        if (err) {
            log::error("[{}] end failed: [{}] {}, {}", name(), err.value(), err.message(), writeSize);
            channelInactive(err);
        }
    }

    void DefaultConnection::setEventManager(const EventManager::Ptr &eventManager) {
        this->_eventManager = eventManager;
    }
}