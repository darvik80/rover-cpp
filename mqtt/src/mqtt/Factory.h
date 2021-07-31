//
// Created by Ivan Kishchenko on 24.12.2020.
//

#ifndef MQTT_FACTORY_H
#define MQTT_FACTORY_H

#include <unordered_set>

#include "mqtt/properties/MqttProperties.h"
#include "mqtt/Connection.h"

#include "mqtt/Subscriber.h"
#include "mqtt/Publisher.h"

namespace mqtt {

    class DefaultClient
            : public Client,
              public EventTopicUnSubscribeHandler,
              public EventChannelMessageHandler,
              public std::enable_shared_from_this<DefaultClient> {
    private:
        std::atomic<uint16_t> _pid{0};
        Connection::Ptr _connection;
        EventManager::Ptr _eventManager;

        std::unordered_set<Subscriber::Ptr> _subscribers;
    private:
        DefaultClient(const Connection::Ptr &connection, const EventManager::Ptr &eventManager);

    public:

        template<typename ... T>
        static std::shared_ptr<DefaultClient> create(T &&... all) {
            return std::shared_ptr<DefaultClient>(new DefaultClient(std::forward<T>(all)...));
        }

        VoidFuture post(const message::Message::Ptr &msg) override;

        Subscription subscribe(std::string_view topic, uint8_t qos, const DataCallback &&callback) override;

        Publisher publisher(std::string_view topic, uint8_t qos) override;

        EventManager::Ptr getEventManager() override;

        void onEvent(const EventTopicUnSubscribe &event) override;

        void onEvent(const EventChannelMessage &event) override;

        uint16_t getPacketIdentifier() override {
            return ++_pid;
        }

        ~DefaultClient() override;
    };

    class Factory {
    public:
        typedef std::shared_ptr<Factory> Ptr;
    public:
        Client::Ptr create(const MqttProperties &props);
    };
}


#endif //MQTT_FACTORY_H
