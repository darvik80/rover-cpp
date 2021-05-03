//
// Created by Ivan Kishchenko on 09.04.2021.
//

#ifndef ROVER_BOOSTSERIALPORT_H
#define ROVER_BOOSTSERIALPORT_H

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION

#include <set>

#include <boost/thread/future.hpp>

#include <boost/asio.hpp>
#include "net/Transport.h"
#include <serial/Protocol.h>
#include <scheduler/Scheduler.h>

#define SERIAL_PORT_READ_BUF_SIZE 1024

class SerialPort {
public:
    virtual std::string deviceId() = 0;
    virtual boost::future<void> send(uint8_t msgId, const uint8_t *data, size_t size) = 0;
    virtual void onMessage(uint8_t msgId, const uint8_t *data, size_t size) = 0;
};

class SerialPortCallback : public SerialPort {
public:
    typedef std::shared_ptr<SerialPortCallback> Ptr;
    typedef std::set<Ptr> SetPtr;
public:
    virtual void onConnect(SerialPort& port) = 0;
    virtual void onDisconnect(SerialPort& port, boost::system::error_code& ec) = 0;
    virtual void onMessage(SerialPort& port, uint8_t msgId, const uint8_t *data, size_t size) = 0;
    virtual void onError(SerialPort& port, boost::system::error_code& ec) = 0;
};

class BoostSerialPort : public SerialPortCallback {
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


    std::function<void()> _fnReopen{nullptr};

    SerialPortCallback::SetPtr _callbacks;
public:
    BoostSerialPort(boost::asio::io_service& service, std::string_view port, unsigned int baudRate);

    boost::future<void> send(uint8_t msgId, const uint8_t *data, size_t size) override;
    void onMessage(uint8_t msgId, const uint8_t *data, size_t size) override;

    void addCallback(SerialPortCallback::Ptr callback) {
        _callbacks.emplace(callback);
    }
public:
    std::string deviceId() override;

    void onConnect(SerialPort &port) override;

    void onDisconnect(SerialPort &port, boost::system::error_code &ec) override;

    void onMessage(SerialPort &port, uint8_t msgId, const uint8_t *data, size_t size) override;

    void onError(SerialPort &port, boost::system::error_code &ec) override;

private:
    void asyncRead();
    void onIdle();
};


#endif //ROVER_BOOSTSERIALPORT_H
