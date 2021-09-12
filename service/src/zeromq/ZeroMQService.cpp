//
// Created by Ivan Kishchenko on 23.08.2021.
//

#include "ZeroMQService.h"
#include "zeromq/raspberry/ZeroMQLogger.h"

#include <thread>
#include "zeromq/ZeroMQUtils.h"

void ZeroMQService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);
    ZeroMQUtils::init();


//    ZeroMQBufFix<1024> buf;
//    std::ostream out(&buf);
//
//    ZeroMQGreeting greeting(false);
//    out << greeting;
//    zero_mq::log::info("data: {}:{}", buf.in_avail(), buf.size());
//
//    std::istream inc(&buf);
//    inc >> greeting;
//    zero_mq::log::info("data: {}:{}", buf.in_avail(), buf.size());
//    inc.clear();
//    inc.seekg(0);
//    zero_mq::log::info("data: {}:{}", buf.in_avail(), buf.size());

//    if (out) {
//        zero_mq::log::info("data: {}:{}", buf.in_avail(), buf.size());
//    }
//    inc >> greeting;
//    if (inc) {
//        zero_mq::log::info("data: {}:{}", buf.in_avail(), buf.size());
//    }
//    buf.compress();

//    ZeroMQCommand cmd(ZERO_MQ_CMD_READY);
//    cmd.props.emplace(ZERO_MQ_PROP_IDENTIFY, "joystick");
//    cmd.props.emplace(ZERO_MQ_PROP_SOCKET_TYPE, "PUB");
//
//    ZeroMQEncoder encoder;
//    encoder.write(buf, cmd);
//
//    zero_mq::log::info("size: {}:{}", buf.in_avail(), buf.size());
//
//    ZeroMQMessage msg;
//    msg << "joystick";
//    msg << "Hello World";
//
//    encoder.write(buf, msg);
//
//    zero_mq::log::info("size: {}:{}", buf.in_avail(), buf.size());
//
//    ZeroMQDecoder decoder;
//    decoder.onCommand([](const ZeroMQCommand &cmd) {
//        zero_mq::log::info("cmd: {}", cmd.name);
//    });
//    decoder.onMessage([](const ZeroMQMessage &msg) {
//        for (const auto &item: msg.data) {
//            zero_mq::log::info("msg: {}", item);
//        }
//    });
//
//    while (!decoder.read(buf)) {
//        zero_mq::log::info("size: {}:{}", buf.in_avail(), buf.size());
//    }

//    _pub = std::make_unique<zmqpp::socket>(_context, zmqpp::socket_type::publish);
//    _pub->connect("tcp://192.168.100.163:5556");
    _sub = std::make_unique<zmqpp::socket>(_context, zmqpp::socket_type::subscribe);
    _sub->subscribe("joystick");
    _sub->bind("tcp://*:5556");
    _sub->subscribe("joystick");

    zero_mq::log::info("init: {}", 5556);

    _thread = std::make_unique<std::thread>([this]() {
        while (true) {
            zmqpp::message message;
            _sub->receive(message);
            std::string topic, data;

            message >> topic;
            message >> data;
            zero_mq::log::info("data recv: {}:{}", topic, data);
            //sleep(5);

        }
    });

    _publisher = std::make_unique<ZeroMQSocket>(registry.getIoService(), "127.0.0.1", 5556);
    //_publisher->publish("joystick", "test");
}

void ZeroMQService::preDestroy(Registry &registry) {
    BaseService::preDestroy(registry);
}
