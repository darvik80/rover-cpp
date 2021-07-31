#include <iostream>

#include <boost/asio.hpp>

#include <logging/Logging.h>
#include "logging/MqttLogger.h"
#include "mqtt/Library.h"

#include <boost/program_options.hpp>

using namespace mqtt;

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
    logging::LoggingProperties logProps;
    logProps.level="info";
    logging::setup(logProps);

    boost::asio::io_service main;
    boost::asio::signal_set signals(main, SIGINT, SIGTERM, SIGQUIT);
    signals.async_wait([](const boost::system::error_code &error, int signalNumber) {
        if (!error) {
            // A signal occurred.
            log::info("handle: {}", signalNumber);
        }
    });

    auto lib = std::make_shared<Library>();
    lib->init();
    auto factory = lib->getFactory();

    MqttProperties properties{
            "192.168.100.3",
            1883,
            "", "",
            "rover"
    };

    auto client = factory->create(properties);
    DeadlineTimer timer(main, PosixSeconds{10});
    auto sub = client->subscribe("/home/test", message::QOS_AT_LEAST_ONCE, [](const ByteBuffer &data) {
        std::string msg(data.begin(), data.end());
        log::info("got msg: {}", msg);
    });
    auto pub = client->publisher("/home/test", message::QOS_AT_LEAST_ONCE);
    timer.async_wait([pub, &timer](const ErrorCode &err) {
        //sub.shutdown();
        pub.publish("Hello World");
    });
    main.run();
    lib->shutdown();
    return 0;
}
