//
// Created by Ivan Kishchenko on 10.07.2021.
//

#ifndef ROVER_JOYSTICKSERVICE_H
#define ROVER_JOYSTICKSERVICE_H

#ifdef SUPPORT_XBOXDRV

#include "BaseService.h"
#include <boost/asio/posix/stream_descriptor.hpp>
#include <linux/joystick.h>

class JoystickService : public BaseService {
    js_event _events[64];
    std::unique_ptr<boost::asio::posix::stream_descriptor> _stream;
public:
    const char *name() override {
        return "joystick";
    }

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry &registry) override;
private:
    void onRead(const boost::system::error_code &error, std::size_t readable);
};

#endif

#endif //ROVER_JOYSTICKSERVICE_H
