//
// Created by Ivan Kishchenko on 04.09.2021.
//

#ifndef PLATFORM_IO_ZEROMQTOPICEVENT_H
#define PLATFORM_IO_ZEROMQTOPICEVENT_H

#include <string>
#include <functional>

struct ZeroMQTopicEvent {
    std::string topic;
    std::string data;
};

typedef std::function<void(const ZeroMQTopicEvent&)> ZeroMQTopicEventHandler;

#endif //PLATFORM_IO_ZEROMQTOPICEVENT_H
