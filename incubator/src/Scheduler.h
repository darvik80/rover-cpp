//
// Created by Ivan Kishchenko on 02.05.2021.
//

#ifndef ROVER_SCHEDULER_H
#define ROVER_SCHEDULER_H

#include "BaseService.h"

typedef boost::asio::deadline_timer DeadlineTimer;
typedef boost::posix_time::ptime PosixTime;
typedef boost::posix_time::time_duration PosixDuration;
typedef boost::posix_time::seconds PosixSeconds;
typedef boost::posix_time::millisec PosixMilliseconds;
typedef boost::posix_time::microseconds PosixMicroseconds;

class Task {
public:
    virtual void execute() = 0;
    virtual void cancel() = 0;
};


class Scheduler : public BaseService {
private:
public:
    void postConstruct(Registry &registry) override;

    const char *name() override {
        return "scheduler";
    }

    void schedule(PosixTime time, PosixDuration duration, std::function<void()> fn);
};


#endif //ROVER_SCHEDULER_H
