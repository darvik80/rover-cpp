//
// Created by Kishchenko, Ivan on 3/25/21.
//

#ifndef ROVER_CONFIG_H
#define ROVER_CONFIG_H


#ifdef RASPBERRY_ARCH
#include <boost/asio.hpp>

typedef boost::asio::io_service IoService;
typedef std::shared_ptr<IoService> IoServicePtr;

#endif

struct NetworkConfiguration {
    bool registryClient = false;
    std::string registryHost = "127.0.0.1";
    uint16_t registryPort = 54545;
};

struct Configuration {
    NetworkConfiguration network;
};

struct Content {
#ifdef RASPBERRY_ARCH
    IoServicePtr service;
#endif

    Configuration config;
};

#endif //ROVER_CONFIG_H
