//
// Created by Ivan Kishchenko on 01.05.2021.
//

#include "SerialService.h"
#include <serial/Protocol.h>
#include <etl/crc16.h>

#include "serial/SerialMessage.h"

SerialService::SerialService(Registry& registry, Stream &stream)
        : BaseService(registry), _stream(stream) {
}

void SerialService::postConstruct() {
    Serial.println("ARM Device, proto: v1");
}

void SerialService::run() {
    if (!_stream.available()) {
        return;
    }

    onReceive(_stream);
}

void SerialService::preDestroy() {

}

void SerialService::send(uint8_t msgId, const uint8_t *data, uint8_t size) {
    etl::crc16 crc;
    crc.add(data, data + size);
    uint16_t ctrl = crc.value();

    _stream.write(serial::MSG_MAGIC);
    _stream.write(msgId);
    _stream.write(size);
    if (size) {
        _stream.write(data, size);
    }
    _stream.write((const char *) &ctrl, 2);
    _stream.write(serial::MSG_MAGIC);
}

void SerialService::onReceive(Stream &stream) {
    switch (_recvState) {
        case IDLE: {
            while (stream.available()) {
                int ch = stream.read();
                if (ch == serial::MSG_MAGIC) {
                    _recvState = HEADER;
                    break;
                }
            }
            if (!stream.available()) {
                return;
            }
        }
        case HEADER:
            if (stream.available() < 2) {
                return;
            }

            _cmd = stream.read();
            _len = stream.read();
            _recvState = BODY;
        case BODY:
            if (stream.available() < _len) {
                return;
            }
            _buffer.resize(_len);
            stream.readBytes(_buffer.data(), _len);
            _recvState = FOOTER;
        case FOOTER:
            if (stream.available() < 3) {
                return;
            }

            uint16_t ctrl{0};
            stream.readBytes((char *) &ctrl, 2);
            int magic = stream.read();

            etl::crc16 crc;
            crc.add(_buffer.data(), _buffer.data() + _len);
            if (crc.value() != ctrl || magic != serial::MSG_MAGIC) {
                _recvState = IDLE;
                setState(serial::IDLE);
                return;
            }

            onMessage(_cmd, _buffer);
            _recvState = IDLE;
            break;
    }
}

void SerialService::onMessage(uint8_t msgId, etl::vector<uint8_t, UINT8_MAX> &data) {
    if (_state == serial::IDLE) {
        if (msgId == serial::MSG_SYNC) {
            send(serial::MSG_SYNC, nullptr, 0);
            setState(serial::WAIT_SYNC);
        }
    } else if (_state == serial::WAIT_SYNC) {
        if (msgId == serial::MSG_CONN) {
            send(serial::MSG_CONN, nullptr, 0);
            setState(serial::CONN);
        } else {
            setState(serial::IDLE);
        }
    } else {
        if (msgId == serial::MSG_PING) {
            send(serial::MSG_PONG, nullptr, 0);
        } else {
            etl::send_message(getRegistry().getMessageBus(), SerialDataMessage{*this, msgId, data});
        }
    }
}

void SerialService::setState(serial::ConnState newState) {
    if (_state == serial::CONN && newState == serial::IDLE) {
        etl::send_message(getRegistry().getMessageBus(), SerialDisconnected{*this});
    } else if (newState == serial::CONN) {
        etl::send_message(getRegistry().getMessageBus(), SerialConnected{*this});
    }
    _state = newState;
}
