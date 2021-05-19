//
// Created by Ivan Kishchenko on 15.05.2021.
//

#include "BoostSerialPort.h"
#include "logging/Logging.h"

#include <boost/crc.hpp>
#include <cstdint>

using namespace boost;
using namespace serial;

BoostSerialPort::BoostSerialPort(asio::io_service &service, const SerialProperties &props, serial::SerialPortCodecCallback& callback)
        : _serial(service), _timer(service), _props(props), _codec(SerialPortCodec::MODE_MASTER, callback) {

    open();
}

int BoostSerialPort::send(const uint8_t *data, size_t size) {
    _out.sputn((const char *) data, (int) size);
    return size;
}

void BoostSerialPort::flush() {
    asio::async_write(_serial, _out, [this](const boost::system::error_code &ec, std::size_t sent) {
        if (ec) {
            onError(ec);
        } else {
            logging::info("[serial-port] sent: {}", sent);
        }
    });
}

uint16_t BoostSerialPort::crc16(const uint8_t *data, size_t size) {
    boost::crc_16_type crc16;
    crc16.process_block(data, data + size);

    return crc16.checksum();
}


void BoostSerialPort::asyncRead() {
    if (!_serial.is_open()) {
        return;
    }

    _serial.async_read_some(
            boost::asio::buffer(_incBuf, SERIAL_PORT_READ_BUF_SIZE),
            [this](const boost::system::error_code &ec, size_t size) {
                cancelTimer();

                if (!_serial.is_open()) {
                    return;
                }

                if (ec) {
                    onError(ec);
                    open();
                    return;
                }
                logging::info("[serial-port] recv: {}", size);
                onMessage((const uint8_t*)&_incBuf, size);
                asyncRead();
            }
    );

    setTimer(boost::posix_time::seconds{5}, [this]() { onIdle(); });
}

const char* BoostSerialPort::deviceId() {
    return _props.port.c_str();
}

void BoostSerialPort::onConnect() {
    _codec.onConnect(*this);
}

void BoostSerialPort::onDisconnect() {
    _codec.onDisconnect(*this);
    _out.consume(_out.size());
}

void BoostSerialPort::onIdle() {
    _codec.onIdle(*this);

    setTimer(posix_time::seconds{5}, [this]() {
        onIdle();
    });
}

void BoostSerialPort::onError(const system::error_code &ec) {
    _codec.onError(*this, ec.value());
}

void BoostSerialPort::onMessage(const uint8_t *data, size_t size) {
    _codec.onMessage(*this, data, size);
}

void BoostSerialPort::open() {
    if (_serial.is_open()) {
        try {
            _serial.cancel();
            _serial.close();
        } catch (std::exception &ex) {
            logging::warning("can't close port {}, {}", _props.port, ex.what());
        }

        onDisconnect();
    }

    try {
        _serial.open(_props.port);
        _serial.set_option(boost::asio::serial_port_base::baud_rate(_props.baudRate));
        _serial.set_option(boost::asio::serial_port_base::character_size(8));
        _serial.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
        _serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
        _serial.set_option(
                boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
        onConnect();
        asyncRead();
    } catch (std::exception& ex) {
        logging::warning("can't reopen port: {}", ex.what());
        setTimer(posix_time::seconds{5}, [this]() {
            open();
        });
    }
}

void BoostSerialPort::setTimer(posix_time::time_duration duration, const std::function<void()> &fn) {
    _timer.expires_from_now(duration);
    _timer.async_wait([fn](const boost::system::error_code &ec) {
        if (!ec) {
            fn();
        }
    });
}

void BoostSerialPort::cancelTimer() {
    _timer.cancel();
}
