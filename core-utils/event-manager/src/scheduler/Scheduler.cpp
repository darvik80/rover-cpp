//
// Created by Ivan Kishchenko on 02.05.2021.
//

#include "Scheduler.h"

TimerHandler Scheduler::schedule(const boost::posix_time::time_duration& duration, const std::function<void()>& fn) {
    TimerHandler handler(_service);
    handler.schedule(duration, fn);

    return handler;
}