//
// Created by Ivan Kishchenko on 25.12.2020.
//

#ifndef MQTT_COMMAND_H
#define MQTT_COMMAND_H

#include <utility>

#include "UserStd.h"

#include "event/EventChannel.h"
#include "event/EventSystem.h"
#include "event/EventTopic.h"
#include "event/EventNetwork.h"

#include "logging/MqttLogger.h"
#include "Client.h"

namespace mqtt {

    class Command {
        Client::Ptr _client;
    public:
        typedef std::shared_ptr<Command> Ptr;
    protected:
        explicit Command(const Client::Ptr &client) : _client(client) {}

        template<class T>
        boost::future<void> request(std::shared_ptr<T> &req) {
            return _client->post(req);
        }

        template<class T, class R>
        boost::future<R> request(std::shared_ptr<T> &req) {
            auto timer = std::make_shared<DeadlineTimer>(IoServiceHolder::get_mutable_instance(), PosixSeconds{5});
            auto promise = std::make_shared<boost::promise<R>>();

            auto msgHandler = std::make_shared<EventHandlerWrapper<EventChannelMessage>>(
                    [req, promise, timer](const EventChannelMessage &event) {
                        auto *ack = dynamic_cast<R *>(event.getMessage().get());
                        if (!ack || ack->getPacketIdentifier() != req->getPacketIdentifier()) {
                            return;
                        }

                        if (timer->cancel()) {
                            log::debug("[cmd] handle ACK, pid: {}", ack->getPacketIdentifier());
                            promise->set_value(*ack);
                        }
                    });
            _client->getEventManager()->subscribe<EventChannelMessage>(msgHandler);

            em::EventHandler::Ptr netHandler = std::make_shared<EventHandlerWrapper<EventChannelInactive>>(
                    [req, promise, timer](const EventChannelInactive &event) {
                        if (timer->cancel()) {
                            log::error("[cmd] failed, pid: {}, error: {}", req->getPacketIdentifier(), event.getErr().message());
                            promise->set_exception(event.getErr());
                        }
                    });
            _client->getEventManager()->subscribe<EventChannelInactive>(netHandler);

            _client->post(req).then([req, msgHandler, netHandler, promise, timer](VoidFuture result) {
                try {
                    result.get();
                } catch (boost::system::system_error& ex) {
                    //log::error("[cmd] failed send request, pid: {}, error: [{}] {}", req->getPacketIdentifier(), ex.code(), ex.what());
                    promise->set_exception(ex);
                    return;
                }

                timer->async_wait([req, msgHandler, netHandler, promise](const ErrorCode &err) {
                    if (!err) {
                        log::error("[cmd] request timeout, pid: {}", req->getPacketIdentifier());
                        promise->set_exception(boost::system::system_error(boost::asio::error::timed_out));
                    }
                });
            });

            return promise->get_future();
        }

        uint16_t getPacketIdentifier() {
            return _client->getPacketIdentifier();
        }

    public:
        virtual boost::future<void> execute() = 0;
    };

}

#endif //MQTT_COMMAND_H
