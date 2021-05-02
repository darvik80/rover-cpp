//
// Created by Ivan Kishchenko on 02.05.2021.
//

#ifndef ROVER_SCHEDULER_H
#define ROVER_SCHEDULER_H

#include <utility>
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>

#include "BaseService.h"

class TimerHandler {

    std::shared_ptr<boost::asio::deadline_timer> _timer;
public:
    TimerHandler() = default;

    explicit TimerHandler(boost::asio::io_context& service)
            : _timer(std::make_unique<boost::asio::deadline_timer>(service)) {}

    TimerHandler(std::shared_ptr<boost::asio::deadline_timer>& timer)
            : _timer(std::move(timer)) {
    }

    TimerHandler(const std::weak_ptr<boost::asio::deadline_timer>& timer)
            : _timer(timer) {
    }

    TimerHandler(TimerHandler &other)
            : _timer(std::move(other._timer)) {

    }

    TimerHandler &operator=(const TimerHandler &other) {
        if (&other == this) {
            return *this;
        }
        _timer = other._timer;
        return *this;
    }

public:
    void schedule(boost::posix_time::time_duration duration, const std::function<void()>& fn) {
        _timer->expires_from_now(duration);
        _timer->async_wait([fn](const boost::system::error_code& ec) {
            if (!ec) {
                fn();
            }
        });
    }

    [[nodiscard]] bool cancel() {
        if (_timer) {
            return _timer->cancel() > 0;
        }
        return false;
    }
};

class Scheduler : public BaseService {
    boost::asio::io_service& _service;
public:
    explicit Scheduler(boost::asio::io_service &service)
            : _service(service) {}

    const char *name() override {
        return "scheduler";
    }

    TimerHandler schedule(boost::posix_time::time_duration duration, const std::function<void()>& fn);

    int order() override {
        return INT32_MAX - 1;
    }
};


#endif //ROVER_SCHEDULER_H
