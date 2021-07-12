//
// Created by Ivan Kishchenko on 10.07.2021.
//

#ifdef SUPPORT_XBOXDRV

#include "JoystickService.h"

void JoystickService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);
    _stream = std::make_unique<boost::asio::posix::stream_descriptor>(
                registry.getIoService()
            );
    int fd = open("/dev/input/js0", O_RDONLY);
    if (fd > 0) {
        char name[128];
        if (ioctl(fd, JSIOCGNAME(sizeof(name)), name) < 0) {
            strncpy(name, "Unknown", sizeof(name));
        }
        info("joystick name: {}", name);

        warning("event size: {}, events_size: {}", sizeof(js_event), sizeof(_events));
        _stream->assign(fd);
        _stream->async_read_some(
                boost::asio::buffer(&_events, sizeof(_events)),
                [this](const boost::system::error_code error, std::size_t readable) {
                    onRead(error, readable);
                }
        );
    } else {
        warning("failed to open joystick");
    }
}

void JoystickService::onRead(const boost::system::error_code &error, std::size_t readable) {
    if (!error) {
        std::size_t size = readable / sizeof(js_event);
        for (int idx = 0; idx < size; idx++) {
            if (_events[idx].type == JS_EVENT_AXIS) {
                if (_events[idx].number != 4 && _events[idx].number !=5) {
                    if (_events[idx].value > 1024 || _events[idx].value < -1024) {
                        continue;
                    }
                } else {
                    _events[idx].value = (_events[idx].value + 32768) >> 8;
                }
            }
            info("ev: {}, {}, {}", _events[idx].number, _events[idx].type, _events[idx].value);

        }
        _stream->async_read_some(
                boost::asio::buffer(&_events, sizeof(_events)),
                [this](const boost::system::error_code error, std::size_t readable) {
                    onRead(error, readable);
                }
        );
    } else {
        warning("ev failed: {}", error.message());
    }
}

void JoystickService::preDestroy(Registry &registry) {
    BaseService::preDestroy(registry);
}

#endif
