//
// Created by Ivan Kishchenko on 23.08.2021.
//

#pragma once

#include "BaseService.h"
#include <zmqpp/zmqpp.hpp>
#include <memory>
#include <array>

#include "scheduler/SchedulerService.h"

#include "zeromq/raspberry/ZeroMQSocket.h"

class ZeroMQService : public BaseService {
    std::unique_ptr<std::thread> _thread;
    std::unique_ptr<std::thread> _threadPub;

    zmqpp::context _context;;
    std::unique_ptr<zmqpp::socket> _sub;

    std::unique_ptr<zmqpp::socket> _pub;

    TimerHandler _timer;

    std::unique_ptr<ZeroMQSocket> _publisher;
public:
    const char *name() override {
        return "zeromq";
    }

    void postConstruct(Registry &registry) override;


    void preDestroy(Registry &registry) override;

private:
    void run();
};
