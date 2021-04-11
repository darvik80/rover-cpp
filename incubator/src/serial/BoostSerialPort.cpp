//
// Created by Ivan Kishchenko on 09.04.2021.
//

#include "BoostSerialPort.h"

BoostSerialPort::BoostSerialPort(boost::asio::io_service& service, std::string_view port, unsigned int baudRate)
: _serial(service, port.data())
{
    _serial.set_option(boost::asio::serial_port_base::baud_rate(baudRate));
}

void BoostSerialPort::onMessage(const uint8_t *data, size_t size) {

}

int BoostSerialPort::send(const uint8_t *data, size_t size) {
    return 0;
}
