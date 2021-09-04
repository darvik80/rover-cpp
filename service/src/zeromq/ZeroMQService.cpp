//
// Created by Ivan Kishchenko on 23.08.2021.
//

#include "ZeroMQService.h"
#include "ZeroMQLogger.h"

#include <thread>
#include "ZeroMQUtils.h"

void ZeroMQService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);
    ZeroMQUtils::init();


    std::array<char, 10> data{};
    ZeroMQBuf<char> buf(data.data(), data.size());
    std::ostream out(&buf);
    std::istream inc(&buf);

    ZeroMQGreeting greeting(false);
    out << greeting;
    if (out) {
        zero_mq::log::info("data: {}:{}", buf.in_avail(), buf.size());
    }
    inc >> greeting;
    if (inc) {
        zero_mq::log::info("data: {}:{}", buf.in_avail(), buf.size());
    }
//    zero_mq::log::info("data: {}:{}:", buf.in_avail(), buf.size());
//    zero_mq::log::info("data: {}", buf.dump());
//    buf.compress();
//    out << greeting;
//    zero_mq::log::info("data: {}:{}", buf.in_avail(), buf.size());
//    zero_mq::log::info("data: {}", buf.dump());

    _pub = std::make_unique<zmqpp::socket>(_context, zmqpp::socket_type::publish);
    _pub->connect("tcp://192.168.100.163:5556");
//    _sub = std::make_unique<zmqpp::socket>(_context, zmqpp::socket_type::subscribe);
//    _sub->subscribe("joystick");
//    _sub->bind("tcp://*:5556");
//
//    zero_mq::log::info("init: {}", 5556);
//
    _thread = std::make_unique<std::thread>([this]() {
        while (true) {
            zmqpp::message message;
            message << "joystick";
            message << "test";

            _pub->send(message);
            sleep(5);

            zero_mq::log::info("data send");
        }
    });

    //_publisher = std::make_unique<ZeroMQSocket>(registry.getIoService(), "192.168.100.163", 5556);
    //_publisher->publish("joystick", "test");
}

void ZeroMQService::preDestroy(Registry &registry) {
    BaseService::preDestroy(registry);
}
