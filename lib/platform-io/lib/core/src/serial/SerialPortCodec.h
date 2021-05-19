//
// Created by Ivan Kishchenko on 16.05.2021.
//

#ifndef ROVER_SERIALPORTCODEC_H
#define ROVER_SERIALPORTCODEC_H

#include <string>
#include "SerialPort.h"
#include "Protocol.h"


#ifndef SERIAL_MSG_SIZE
#define SERIAL_MSG_SIZE 128
#endif

namespace serial {

    class SerialPortCodec : public SerialPortHandler {
        enum CodecState {
            IDLE,
            WAIT_HEADER,
            WAIT_MESSAGE,
            READ_MESSAGE,
        };
        CodecState _state{IDLE};


        uint8_t *_pos{nullptr};
        uint8_t _buffer[SERIAL_MSG_SIZE]{};
        uint16_t _msgLen{0};

        enum ConnectionStatus {
            DISCONNECTED,
            PROCESS,
            CONNECTED,
        };

        ConnectionStatus _connStatus{DISCONNECTED};
    public:
        void onConnect(SerialPort &port) override {
            _pos = _buffer;
        }

        void onDisconnect(SerialPort &port) override {
        }

        void onIdle(SerialPort &port) override {
            if (_connStatus == DISCONNECTED) {
                sendMessage(port, Message{MSG_SYNC, 0, nullptr, 0});
            } else if (_connStatus == CONNECTED) {
                sendMessage(port, Message{MSG_PING, 0, nullptr, 0});
            }
        }

        void onMessage(SerialPort &port, const uint8_t *data, size_t size) override {
            for (int idx = 0; idx < size; idx++) {
                switch (_state) {
                    case IDLE:
                        if (data[idx] == MSG_MAGIC) {
                            _state = WAIT_HEADER;
                            _pos = _buffer;
                        }
                        break;
                    case WAIT_HEADER: {
                        if (data[idx] == MSG_MAGIC) {
                            break;
                        }
                        *(_pos++) = data[idx];
                        auto dataSize = (size_t) (_pos - _buffer);
                        if (dataSize >= 3) {
                            _msgLen = *(uint16_t * )(_buffer + sizeof(uint8_t));
                            _msgLen += sizeof(uint16_t); //crc32
                            _state = WAIT_MESSAGE;
                        }
                    }
                        break;
                    case WAIT_MESSAGE: {
                        *(_pos++) = data[idx];
                        if (--_msgLen == 0) {
                            _state = READ_MESSAGE;
                        }
                    }
                        break;
                    case READ_MESSAGE: {
                        if (data[idx] == MSG_MAGIC) {
                            Message message{
                                    _buffer[0],
                                    *(uint16_t * )(_buffer + sizeof(uint16_t)),
                                    nullptr,
                                    0
                            };
                            message.data = message.len > 0 ? _buffer + 3 : nullptr;

                            uint16_t crc = *(uint16_t * )(_pos - sizeof(uint16_t));
                            if (crc == port.crc16(message.data, message.len)) {
                                onMessage(port, message);
                            }
                        }
                        _state = IDLE;
                        _pos = _buffer;
                    }
                        break;
                }
            }
        }

        void onError(SerialPort &port, int ec) override {
            _state = IDLE;
            _connStatus = DISCONNECTED;
        }

    private:
        void onMessage(SerialPort &port, Message &message) {
            if (_connStatus == DISCONNECTED) {
                if (message.msgId == MSG_SYNC) {
                    sendMessage(port, Message{MSG_CONN, 0, nullptr});
                    _connStatus = PROCESS;
                }
            } else if (_connStatus == PROCESS) {
                if (message.msgId == MSG_CONN) {
                    _connStatus = CONNECTED;
                } else {
                    _connStatus = DISCONNECTED;
                }
            } else {
                if (message.msgId == MSG_PONG) {
                    // TODO: logs ping
                } else if (message.msgId == MSG_DOWN) {
                    onError(port, -1);
                } else {

                }
            }
        }

        void sendMessage(SerialPort &port, const Message &message) {
            port.send(MSG_MAGIC);
            port.send(message.msgId);
            port.send(message.len);
            if (message.len > 0) {
                port.send(message.data, message.len);
            }
            port.send(port.crc16(message.data, message.len));
            port.send(MSG_MAGIC);
            port.flush();
        }
    };
}

#endif //ROVER_SERIALPORTCODEC_H
