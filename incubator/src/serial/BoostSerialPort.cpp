//
// Created by Ivan Kishchenko on 09.04.2021.
//

#include "BoostSerialPort.h"
#include <boost/crc.hpp>

#include <iostream>

using namespace boost;

BoostSerialPort::BoostSerialPort(boost::asio::io_service &service, std::string_view port, unsigned int baudRate)
        : _serial(service), _idleTimer(service) {

    _fnReopen = [this, port, baudRate]() {
        try {
            _serial.open(port.data());
            _serial.set_option(boost::asio::serial_port_base::baud_rate(baudRate));
            _serial.set_option(boost::asio::serial_port_base::character_size(8));
            _serial.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
            _serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
            _serial.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
            if (_fnOnConnected) {
                _fnOnConnected();
            }
        } catch (std::exception& ex) {}
    };

    _fnReopen();
    _idleTimer.schedule(boost::posix_time::seconds{5}, [this]() {
        onIdle();
    });

    asyncRead();
}

void BoostSerialPort::onIdle() {
    if (_fnOnIdle) {
        _fnOnIdle();
    }
    _idleTimer.schedule(boost::posix_time::seconds{5}, [this]() {
        onIdle();
    });
}

void BoostSerialPort::onMessage(uint8_t msgId, const uint8_t *data, size_t size) {
    if (_fnOnMessage) {
        _fnOnMessage(msgId, data, size);
    }
}

boost::future<void> BoostSerialPort::send(uint8_t msgId, const uint8_t *data, size_t size) {
    auto promise = std::make_shared<boost::promise<void>>();
    _out.sputc(serial::MSG_MAGIC);
    _out.sputc((char) msgId);
    _out.sputc((char) size);

    if (size) {
        _out.sputn((const char *) data, (int) size);
    }
    boost::crc_16_type crc16;
    crc16.process_block(data, data + size);
    uint16_t ctrl = crc16.checksum();
    _out.sputn((const char *) &ctrl, 2);
    _out.sputc(serial::MSG_MAGIC);

    asio::async_write(_serial, _out, [promise, this](const boost::system::error_code &ec, std::size_t size) {
        if (ec) {
            promise->set_exception(system::system_error(ec));
        } else {
            promise->set_value();
        }
    });

    return promise->get_future();
}

void BoostSerialPort::asyncRead() {
    //if (!_serial.is_open()) {
    //    return;
    //}

    _serial.async_read_some(
            boost::asio::buffer(_incBuf, SERIAL_PORT_READ_BUF_SIZE),
            [this](const boost::system::error_code &ec, size_t size) {
                _idleTimer.cancel();

                //if (!_serial.is_open()) {
                //    return;
                //}

                if (ec) {
                    std::clog << "err: " << ec.message() << std::endl;
                    sleep(10);
                    _serial.cancel();
                    _serial.close();
                    _fnReopen();
                    asyncRead();
                    return;
                }
                _inc.sputn(_incBuf, (int) size);

                std::clog << "inc before: " << size << "/" << _inc.size() << std::endl;
                std::istream inc(&_inc);
                while (!inc.eof()) {
                    if (_recvState == IDLE) {
                        auto ch = inc.get();
                        if (ch == EOF) {
                            continue;
                        }
                        if (serial::MSG_MAGIC != ch) {
                            continue;
                        }
                        _recvState = HEADER;
                    } else if (_recvState == HEADER) {
                        if (_inc.size() < 2) {
                            break;
                        }
                        _cmd = inc.get();
                        _len = inc.get();
                        _buffer.resize(_len);
                        _recvState = BODY;
                    } else if (_recvState == BODY) {
                        if (_len > 0) {
                            if (_inc.size() < _len) {
                                break;
                            }
                            inc.read((char *) _buffer.data(), _len);
                        }
                        _recvState = FOOTER;
                    } else if (_recvState == FOOTER) {
                        if (_inc.size() < 3) {
                            break;
                        }

                        uint16_t ctrl{0};
                        inc.read((char *) &ctrl, 2);
                        int magic = inc.get();

                        boost::crc_16_type crc16;
                        crc16.process_block(_buffer.data(), _buffer.data() + _buffer.size());
                        if (crc16.checksum() != ctrl || magic != serial::MSG_MAGIC) {
                            _recvState = IDLE;
                            break;
                        }

                        onMessage(_cmd, _buffer.data(), _buffer.size());
                        _recvState = IDLE;
                    }
                }
                std::clog << "inc after: " << _inc.size() << std::endl;
                asyncRead();
            }
    );

    _idleTimer.schedule(boost::posix_time::seconds{5}, [this]() {
        onIdle();
    });
}