//
// Created by Ivan Kishchenko on 24.04.2021.
//

#ifndef ROVER_EVENT_H
#define ROVER_EVENT_H

#include <utility>

#include "Service.h"

class Event {
private:
    Service::Ptr _source;
public:
    Event() = default;

    explicit Event(Service::Ptr source)
            : _source(std::move(source)) {}

    Service::Ptr source() {
        return _source;
    }

    virtual ~Event() = default;
};


#endif //ROVER_EVENT_H
