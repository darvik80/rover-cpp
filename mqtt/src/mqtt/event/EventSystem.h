//
// Created by Kishchenko, Ivan on 1/18/21.
//

#ifndef MQTT_EVENTSYSTEM_H
#define MQTT_EVENTSYSTEM_H

#include "MqttEvent.h"

namespace mqtt {

    class EventTimeout : public MqttEvent {
    public:
        explicit EventTimeout(em::EventSource::Ptr source)
                : MqttEvent(source, ErrorCode{boost::asio::error::timed_out}) {}
    };
}


#endif //MQTT_EVENTSYSTEM_H
