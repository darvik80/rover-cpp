#include <iostream>

#include <boost/asio.hpp>

#include <mqtt/logging/MqttLogger.h>
#include "mqtt/Library.h"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
    logger::LoggingProperties logProps;
    logProps.level="info";
    logger::setup(logProps);

    boost::asio::io_service main;
    boost::asio::signal_set signals(main, SIGINT, SIGTERM, SIGQUIT);
    signals.async_wait([](const boost::system::error_code &error, int signalNumber) {
        if (!error) {
            // A signal occurred.
            logger::info("handle: {}", signalNumber);
        }
    });

    auto lib = std::make_shared<mqtt::Library>();
    lib->init();
    auto factory = lib->getFactory();

    mqtt::MqttProperties properties{
            "192.168.100.3",
            1883,
            "", "",
            "rover"
    };

    auto client = factory->create(properties);
    mqtt::DeadlineTimer timer(main, mqtt::PosixSeconds{10});
    auto sub = client->subscribe("/home/test", mqtt::message::QOS_AT_LEAST_ONCE, [](const mqtt::ByteBuffer &data) {
        std::string msg(data.begin(), data.end());
        logger::info("got msg: {}", msg);
    });
    auto pub = client->publisher("/home/test", mqtt::message::QOS_AT_LEAST_ONCE);
    timer.async_wait([pub, &timer](const mqtt::ErrorCode &err) {
        //sub.shutdown();
        pub.publish("Hello World");
    });
    main.run();
    lib->shutdown();
    return 0;
}
