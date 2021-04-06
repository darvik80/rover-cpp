//
// Created by Kishchenko, Ivan on 4/2/21.
//

#ifndef ROVER_CONTEXT_H
#define ROVER_CONTEXT_H

#ifdef RASPBERRY_ARCH
#include <boost/asio.hpp>

typedef boost::asio::io_service IoService;
typedef std::shared_ptr<IoService> IoServicePtr;
#endif

struct Context {
#ifdef RASPBERRY_ARCH
    IoServicePtr service;
#endif
    std::string macAddress;
};


#endif //ROVER_CONTEXT_H
