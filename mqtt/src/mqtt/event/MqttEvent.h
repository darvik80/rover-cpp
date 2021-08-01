//
// Created by Ivan Kishchenko on 20.12.2020.
//

#ifndef MQTT_EVENT_H
#define MQTT_EVENT_H

#include "mqtt/message/Message.h"
#include "mqtt/UserStd.h"
#include <event/Event.h>

namespace mqtt {

class MqttEvent : public em::Event {
    private:
        const ErrorCode _err;

    public:
        MqttEvent() : Event() {}
        explicit MqttEvent(em::EventSource::Ptr source) : Event(source) {}

        explicit MqttEvent(const ErrorCode &err)
                : Event(), _err(err) {}

        MqttEvent(em::EventSource::Ptr source, const ErrorCode &err)
                : Event(source), _err(err) {}

        [[nodiscard]] const ErrorCode &getErr() const {
            return _err;
        }
    };
}

#endif //MQTT_EVENT_H
