//
// Created by Ivan Kishchenko on 09.04.2021.
//

#include "BoostSerialPort.h"
#include <boost/crc.hpp>

using namespace boost;

BoostSerialPort::BoostSerialPort(boost::asio::io_service& service, std::string_view port, unsigned int baudRate)
: _serial(service, port.data())
{
    _serial.set_option(boost::asio::serial_port_base::baud_rate(baudRate));
}

void BoostSerialPort::onMessage(const uint8_t *data, size_t size) {

}

boost::future<void> BoostSerialPort::send(uint8_t msgId, const uint8_t *data, size_t size) {
    auto promise = std::make_shared<boost::promise<void>>();
    _out.sputc(serial::MSG_MAGIC);
    _out.sputc((char)msgId);
    _out.sputc((char)size);

    if (size) {
        _out.sputn((const char *) data, (int)size);
    }
    boost::crc_16_type crc16;
    crc16.process_block(data, data + size);
    uint16_t ctrl = crc16.checksum();
    _out.sputn((const char *)&ctrl, 2);
    _out.sputc(serial::MSG_MAGIC);

    asio::async_write(_serial, _out, [promise, this](const boost::system::error_code& ec, std::size_t size) {
        if (ec) {
            promise->set_exception(system::system_error(ec));
        }  else {
            promise->set_value();
        }
    });

    return promise->get_future();
}
