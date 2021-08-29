//
// Created by Ivan Kishchenko on 23.08.2021.
//

#include "ZeroMQService.h"
#include "ZeroMQLogger.h"

#include <chrono>
#include <thread>
#include <fstream>
#include "ZeroMQProtocol.h"
#include "ZeroMQUtils.h"

void ZeroMQService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);
    ZeroMQUtils::init();

    //std::ofstream file("output.txt");

    ZeroMQBuf<1024> buf;
    std::ostream file(&buf);
    ZeroMQGreeting greeting;

    ZeroMQCommandReady cmd;
    cmd.setProperty("Socket-Type", "PUB");
    cmd.setProperty("Identify", "joystick");

    ZeroMQMessage msg;
    msg << "joystick";
    msg << "test";

    file << greeting << cmd << msg;

    zero_mq::log::info("test: \n{}", buf.dump());

    std::istream inc(&buf);

    ZeroMQGreeting ginc;
    ZeroMQCommand c;
    ZeroMQMessage data;
    inc >> ginc >> c >> data;

    _sub = std::make_unique<zmqpp::socket>(_context, zmqpp::socket_type::subscribe);
    _sub->subscribe("joystick");
    _sub->bind("tcp://*:5556");

    zero_mq::log::info("init: {}", 5556);

    _thread = std::make_unique<std::thread>([this]() {
        while (true) {
            zmqpp::message message;
            _sub->receive(message);

            // Read as a string
            std::string topic, text;
            message >> topic;
            message >> text;

            zero_mq::log::info("data recv: {}:{}", topic, text);
        }
    });

    _publisher = std::make_unique<ZeroMQSocket>(registry.getIoService(), "127.0.0.1", 5556);

//    _pub = std::make_unique<zmqpp::socket>(_context, zmqpp::socket_type::publish);
//    _pub->connect("tcp://192.168.100.18:5556");
//
//    _threadPub = std::make_unique<std::thread>([this]() {
//        while (true) {
//            zmqpp::message message;
//            message.push_back("joystick");
//            message.push_back("hello");
//
//            // Send it off to any subscribers
//            _pub->send(message);
//            std::this_thread::sleep_for(std::chrono::seconds (10));
//        }
//    });
}

void ZeroMQService::preDestroy(Registry &registry) {
    BaseService::preDestroy(registry);
}
