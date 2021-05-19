//
// Created by Ivan Kishchenko on 15.05.2021.
//

#ifndef CORE_SERIAL_H
#define CORE_SERIAL_H

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION

#include <set>
#include <memory>

#include <boost/thread/future.hpp>
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <properties/SerialProperties.h>

#include "serial/Protocol.h"
#include "serial/SerialPort.h"
#include "serial/SerialPortCodec.h"


#define SERIAL_PORT_READ_BUF_SIZE 256

class BoostSerialPort : public serial::SerialPort {
    boost::asio::deadline_timer _timer;
    boost::asio::serial_port _serial;
    SerialProperties _props;

    uint8_t _incBuf[SERIAL_PORT_READ_BUF_SIZE];
    boost::asio::streambuf _out;

    std::vector<uint8_t> _buffer{UINT8_MAX};

    serial::SerialPortCodec _codec;
public:
    BoostSerialPort(boost::asio::io_service &service, const SerialProperties &props, serial::SerialPortCodecCallback& callback);

    const char* deviceId() override;

    int send(const uint8_t *data, size_t size) override;

    void flush() override;

    void onMessage(const uint8_t *data, size_t size) override;

    uint16_t crc16(const uint8_t *data, size_t size) override;

private:
    void open();

    void setTimer(boost::posix_time::time_duration duration, const std::function<void()> &fn);

    void cancelTimer();

private:
    void asyncRead();

    void onIdle();

    void onConnect();

    void onDisconnect();

    void onError(const boost::system::error_code &ec);

};


#endif //CORE_SERIAL_H
