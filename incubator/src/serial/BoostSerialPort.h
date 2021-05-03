//
// Created by Ivan Kishchenko on 09.04.2021.
//

#ifndef ROVER_BOOSTSERIALPORT_H
#define ROVER_BOOSTSERIALPORT_H

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION

#include <boost/thread/future.hpp>

#include <boost/asio.hpp>
#include "net/Transport.h"
#include <serial/Protocol.h>
#include <scheduler/Scheduler.h>

#define SERIAL_PORT_READ_BUF_SIZE 1024

class BoostSerialPort {
    TimerHandler _idleTimer;

    boost::asio::serial_port _serial;

    char _incBuf[SERIAL_PORT_READ_BUF_SIZE];
    boost::asio::streambuf _inc;
    boost::asio::streambuf _out;

    enum RecvState {
        IDLE,
        HEADER,
        BODY,
        FOOTER
    };

    RecvState _recvState{IDLE};
    int _cmd{0};
    int _len{0};
    std::vector<uint8_t> _buffer{UINT8_MAX};

    std::function<void(uint8_t, const uint8_t *, size_t)> _fnOnMessage{nullptr};
    std::function<void()> _fnOnIdle{nullptr};
    std::function<void()> _fnOnConnected{nullptr};

    std::function<void()> _fnReopen{nullptr};
public:
    BoostSerialPort(boost::asio::io_service& service, std::string_view port, unsigned int baudRate);

    void setOnConnected(const std::function<void()>& fn) {
        _fnOnConnected = fn;
    }

    void setOnMessage(const std::function<void(uint8_t, const uint8_t *, size_t)>& fn) {
        _fnOnMessage = fn;
    }
    void setOnIdle(const std::function<void()>& fn) {
        _fnOnIdle = fn;
    }
    boost::future<void> send(uint8_t msgId, const uint8_t *data, size_t size);
private:
    void asyncRead();
    void onMessage(uint8_t msgId, const uint8_t *data, size_t size);
    void onIdle();


};


#endif //ROVER_BOOSTSERIALPORT_H
