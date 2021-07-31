//
// Created by Ivan Kishchenko on 24.12.2020.
//

#ifndef MQTT_SUBSCRIBER_H
#define MQTT_SUBSCRIBER_H

#include "UserStd.h"

#include "Client.h"
#include "Command.h"

#include "event/EventChannel.h"
#include "event/EventNetwork.h"

#include "command/SubscribeCommand.h"
#include "command/UnSubscribeCommand.h"

#include "message/PublishMessage.h"
#include "logging/MqttLogger.h"
#include "TopicFilter.h"

namespace mqtt {
    class Subscriber : public EventChannelMessageHandler, public EventChannelActiveHandler, public std::enable_shared_from_this<Subscriber> {
    public:
        typedef std::shared_ptr<Subscriber> Ptr;
        typedef std::weak_ptr<Subscriber> WeakPtr;
    private:
        std::mutex _lock;

        DataCallback _callback;
        Command::Ptr _cmdSubscribe;
        Command::Ptr _cmdUnSubscribe;

        Client::WeakPtr _client;

        TopicFilter _filter;
    private:
        Subscriber(const Client::Ptr& client, std::string_view topic, uint8_t qos, DataCallback callback)
                : _client(client), _filter(topic), _callback(std::move(callback)) {
            _cmdSubscribe = SubscribeCommand::create(client, topic, qos);
            _cmdUnSubscribe = UnSubscribeCommand::create(client, topic);
            log::debug("[sub] subscriber: {} created", _filter.getFilter());
        }
    public:
        template<typename ... T>
        static std::shared_ptr<Subscriber> create(T &&... all) {
            return std::shared_ptr<Subscriber>(new Subscriber(std::forward<T>(all)...));
        }

        void onEvent(const EventChannelMessage &event) override {
            auto pub = dynamic_cast<message::PublishMessage *>(event.getMessage().get());
            if (pub && _callback) {
                if (_filter.filter(pub->getTopic())) {
                    _callback(pub->getMessage());
                }
            }
        }

        void onEvent(const EventChannelActive &event) override {
            log::info("[sub] detected active channel, re-subscribe to: {}", _filter.getFilter());
            start();
        }

        void start() {
            std::lock_guard guard(_lock);
            if (_cmdSubscribe) {
                _cmdSubscribe->execute().then([this](VoidFuture future) {
                    try {
                        future.get();
                        log::info("[sub] subscribed to: {}", _filter.getFilter());
                    } catch (boost::system::system_error& ex) {
                        log::warning("[sub] failed subscribe to: {}, error: {}", _filter.getFilter(), ex.what());
                    }
                });
            }
        }

        void shutdown() {
            std::lock_guard guard(_lock);
            _cmdSubscribe.reset();
            _cmdUnSubscribe->execute().then([this](VoidFuture future) {
                try {
                    future.get();
                    log::info("[sub] un-subscribed from: {}", _filter.getFilter());
                } catch (boost::system::system_error& ex) {
                    log::error("[sub] un-subscribed from:: {}, error: {}", _filter.getFilter(), ex.what());
                }
            });

            auto client = _client.lock();
            if (client) {
                client->getEventManager()->raiseEvent(EventTopicUnSubscribe{shared_from_this(), _filter.getFilter()});
            }
        }

        ~Subscriber() override {
            log::debug("[sub] subscriber: {} destroyed", _filter.getFilter());
        }
    };


    class Subscription {
        Subscriber::WeakPtr _subscriber;
    public:
        explicit Subscription(const Subscriber::Ptr &subscriber)
                : _subscriber(subscriber) {
        }

        explicit Subscription(const Subscriber::WeakPtr &origin)
                : _subscriber(origin) {
        }

        Subscription(const Subscription &other)
                : _subscriber(other._subscriber) {

        }

        Subscription &operator=(const Subscription &other) {
            if (&other == this) {
                return *this;
            }
            _subscriber = other._subscriber;
            return *this;
        }

    public: //control
        bool start() {
            Subscriber::Ptr ptr = _subscriber.lock();
            if (ptr) {
                ptr->start();
            }

            return ptr != nullptr;
        }

        [[nodiscard]] bool shutdown() const {
            Subscriber::Ptr ptr = _subscriber.lock();
            if (ptr) {
                ptr->shutdown();
            }

            return ptr != nullptr;
        }
    };
}


#endif //MQTT_SUBSCRIBER_H
