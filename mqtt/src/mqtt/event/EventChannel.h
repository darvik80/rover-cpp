//
// Created by Ivan Kishchenko on 25.12.2020.
//

#ifndef MQTT_EVENTHANDLER_H
#define MQTT_EVENTHANDLER_H

#include <memory>
#include <event/EventHandler.h>
#include "EventNetwork.h"
#include "EventTopic.h"

namespace mqtt {

    class EventChannelActiveHandler : virtual public em::TEventHandler<EventChannelActive> {
    public:
        void onEvent(const EventChannelActive &event) override = 0;
    };

    class EventChannelInactiveHandler : virtual public em::TEventHandler<EventChannelInactive> {
    public:
        void onEvent(const EventChannelInactive &event) override = 0;
    };

    class EventChannelMessageHandler : virtual public em::TEventHandler<EventChannelMessage> {
    public:
        void onEvent(const EventChannelMessage &event) override = 0;
    };

    class EventTopicSubscribeHandler : virtual public em::TEventHandler<EventTopicSubscribe> {
    public:
        void onEvent(const EventTopicSubscribe &event) override = 0;
    };

    class EventTopicUnSubscribeHandler : virtual public em::TEventHandler<EventTopicUnSubscribe> {
    public:
        void onEvent(const EventTopicUnSubscribe &event) override = 0;
    };

    template<class T>
    class EventHandlerWrapper : public em::TEventHandler<T> {
        std::function<void(const T &)> _handler;
    public:
        explicit EventHandlerWrapper(const std::function<void(const T &)> &&handler) : _handler(handler) {
        }

        void onEvent(const T &event) override {
            _handler(event);
        }
    };
}

#endif //MQTT_EVENTHANDLER_H
