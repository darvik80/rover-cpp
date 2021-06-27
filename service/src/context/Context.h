//
// Created by Ivan Kishchenko on 09.04.2021.
//

#ifndef ROVER_CONTEXT_H
#define ROVER_CONTEXT_H

#include <CoreContext.h>

typedef boost::asio::io_service IoService;
typedef std::shared_ptr<IoService> IoServicePtr;

struct RaspberryContext : public CoreContext {
    IoServicePtr service;
};


#endif //ROVER_CONTEXT_H
