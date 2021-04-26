//
// Created by Ivan Kishchenko on 09.04.2021.
//

#ifndef ROVER_BOOSTSERIALPORT_H
#define ROVER_BOOSTSERIALPORT_H

#include <boost/asio/serial_port.hpp>
#include <boost/asio/io_service.hpp>
#include "net/Transport.h"

class BoostSerialPort : public Transport {
    boost::asio::serial_port _serial;
public:
    BoostSerialPort(boost::asio::io_service& service, std::string_view port, unsigned int baudRate);
    void onMessage(const uint8_t *data, size_t size) override;

    int send(const uint8_t *data, size_t size) override;
private:
};


#endif //ROVER_BOOSTSERIALPORT_H
