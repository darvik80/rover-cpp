//
// Created by Ivan Kishchenko on 24.12.2020.
//

#include "Factory.h"

namespace mqtt {

    VoidFuture DefaultClient::post(const message::Message::Ptr &msg) {
        return _connection->post(msg);
    }

    em::EventManager::Ptr DefaultClient::getEventManager() {
        return _eventManager;
    }

    void DefaultClient::onEvent(const EventTopicUnSubscribe &event) {
        log::info("MQTT Client remove subscriber: {}", event.getTopic());
        _subscribers.erase(event.getSubscriber());
    }

    void DefaultClient::onEvent(const EventChannelMessage &event) {

    }

    Subscription DefaultClient::subscribe(std::string_view topic, uint8_t qos, const DataCallback &&callback) {
        auto subscriber = Subscriber::create(shared_from_this(), topic, qos, callback);

        _eventManager->subscribe<EventChannelActive>(subscriber);
        _eventManager->subscribe<EventChannelMessage>(subscriber);
        _subscribers.insert(subscriber);

        subscriber->start();
        return Subscription{subscriber};
    }

    Publisher DefaultClient::publisher(std::string_view topic, uint8_t qos) {
        return Publisher{shared_from_this(), topic, qos};
    }

    DefaultClient::~DefaultClient() {
        log::info("MQTT Client destroyed");
    }


    DefaultClient::DefaultClient(const Connection::Ptr &connection, const em::EventManager::Ptr &eventManager)
            : _connection(connection), _eventManager(eventManager) {

    }

    Client::Ptr Factory::create(const MqttProperties &props) {
        auto conn = std::make_shared<DefaultConnection>(props);
        auto eventManager = std::make_shared<em::EventManager>();
        conn->setEventManager(eventManager);
        auto client = DefaultClient::create(conn, eventManager);

        eventManager->subscribe<EventTopicUnSubscribe>(client);
        eventManager->subscribe<EventChannelMessage>(client);

        conn->start();

        return client;
    }

}