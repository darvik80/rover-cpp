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

class BoostSerialPort {
    boost::asio::serial_port _serial;

    boost::asio::streambuf _inc;
    boost::asio::streambuf _out;
public:
    BoostSerialPort(boost::asio::io_service& service, std::string_view port, unsigned int baudRate);
    void onMessage(const uint8_t *data, size_t size);

    boost::future<void> send(uint8_t msgId, const uint8_t *data, size_t size);
private:
};


#endif //ROVER_BOOSTSERIALPORT_H
