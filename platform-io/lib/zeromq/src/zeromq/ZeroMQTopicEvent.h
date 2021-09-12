//
// Created by Ivan Kishchenko on 04.09.2021.
//

#pragma once

#include <string>
#include <functional>

struct ZeroMQTopicEvent {
    std::string topic;
    std::string data;
};

typedef std::function<void(const ZeroMQTopicEvent&)> ZeroMQTopicEventHandler;
