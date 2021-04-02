//
// Created by Kishchenko, Ivan on 4/2/21.
//

#ifndef ROVER_CONTEXT_H
#define ROVER_CONTEXT_H

#ifdef PROFILE_RASPBERRY
#include <boost/asio.hpp>

typedef boost::asio::io_service IoService;
typedef std::shared_ptr<IoService> IoServicePtr;

#endif

struct Context {
#ifdef PROFILE_RASPBERRY
    IoServicePtr service;
#endif
};


#endif //ROVER_CONTEXT_H
