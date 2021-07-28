//
// Created by Ivan Kishchenko on 26.04.2021.
//

#ifndef ROVER_APPLICATIONEVENT_H
#define ROVER_APPLICATIONEVENT_H

#include <event/Event.h>

class ApplicationStartedEvent : public Event {

};

class ApplicationCloseEvent : public Event {
    int _signal;
public:
    explicit ApplicationCloseEvent(int signal) : _signal(signal) {}

    [[nodiscard]] int getSignal() const {
        return _signal;
    }
};

class ApplicationShutdownEvent : public Event {

};

#endif //ROVER_APPLICATIONEVENT_H
