//
// Created by Ivan Kishchenko on 16.05.2021.
//

#ifndef ROVER_SERIALPORTCODEC_H
#define ROVER_SERIALPORTCODEC_H

#include "SerialPort.h"
#include "Protocol.h"


#ifndef SERIAL_MSG_SIZE
#define SERIAL_MSG_SIZE 128
#endif

#if defined(ARDUINO)
#include <etl/vector.h>
#else

#include <vector>

#endif

namespace serial {

    class SerialPortCodecCallback {
    public:
        virtual void onMessage(SerialPort& port, Message& message) = 0;
        virtual void onConnect(SerialPort &port) = 0;
        virtual void onDisconnect(SerialPort &port) = 0;
        virtual ~SerialPortCodecCallback() = default;
    };

    class SerialPortCodec : public SerialPortHandler {
    public:
        enum Mode {
            MODE_MASTER,
            MODE_SLAVE
        };
    private:
#if defined(ARDUINO)
        template<class T>
        using vector = etl::vector<T, SERIAL_MSG_SIZE>;
#else
        template<class T>
        using vector = std::vector<T>;
#endif

        enum CodecState {
            STATE_IDLE,
            STATE_WAIT_HEADER,
            STATE_WAIT_LEN,
            STATE_WAIT_CRC,
            STATE_WAIT_BODY,
            STATE_WAIT_FOOTER,
        };

        enum ConnectionStatus {
            DISCONNECTED,
            PROCESS,
            CONNECTED,
        };

        CodecState _state{STATE_IDLE};


        vector<uint8_t> _buffer{SERIAL_MSG_SIZE};

        Message _message;

        ConnectionStatus _connStatus{DISCONNECTED};

        Mode _mode;
        SerialPortCodecCallback& _callback;

    public:

        explicit SerialPortCodec(Mode mode, SerialPortCodecCallback& callback) : _mode(mode), _callback(callback) {}

        void onConnect(SerialPort &port) override {
        }

        void onDisconnect(SerialPort &port) override {
            _callback.onDisconnect(port);
            _connStatus = DISCONNECTED;
        }

        void onIdle(SerialPort &port) override {
            if (_mode == MODE_MASTER) {
                if (_connStatus == DISCONNECTED) {
                    sendMessage(port, Message{MSG_SYNC, 0, nullptr, 0});
                } else if (_connStatus == CONNECTED) {
                    sendMessage(port, Message{MSG_PING, 0, nullptr, 0});
                }
            }
        }

        void onMessage(SerialPort &port, const uint8_t *data, size_t size) override {
            for (int idx = 0; idx < size; idx++) {
                switch (_state) {
                    case STATE_IDLE:
                        if (data[idx] == MSG_MAGIC) {
                            _state = STATE_WAIT_HEADER;
                        }
                        break;
                    case STATE_WAIT_HEADER:
                        if (data[idx] == MSG_MAGIC) {
                            break;
                        }

                        _message.msgId = data[idx];
                        _buffer.resize(0);
                        _state = STATE_WAIT_CRC;
                        break;
                    case STATE_WAIT_CRC:
                        _buffer.emplace_back(data[idx]);
                        if (_buffer.size() >= 2) {
                            _message.crc16 = *(uint16_t *) _buffer.data();
                            _buffer.resize(0);
                            _state = STATE_WAIT_LEN;
                        }
                        break;
                    case STATE_WAIT_LEN:
                        _buffer.emplace_back(data[idx]);
                        if (_buffer.size() >= 2) {
                            _message.len = *(uint16_t *) _buffer.data();
                            _buffer.resize(0);
                            _state = _message.len ? STATE_WAIT_BODY : STATE_WAIT_FOOTER;
                        }
                        break;
                    case STATE_WAIT_BODY:
                        if (_message.len) {
                            _buffer.emplace_back(data[idx]);
                        }
                        if (_buffer.size() >= _message.len) {
                            _state = STATE_WAIT_FOOTER;
                        }
                        break;
                    case STATE_WAIT_FOOTER:
                        if (data[idx] == MSG_MAGIC) {
                            _message.data = _buffer.empty() ? nullptr : _buffer.data();
                            if (_message.crc16 == port.crc16(_message.data, _message.len)) {
                                onMessage(port, _message);
                            } else {
                                _state = STATE_IDLE;
                            }
                        }
                        _state = STATE_IDLE;
                        break;
                }
            }
        }

        void onError(SerialPort &port, int ec) override {
            _state = STATE_IDLE;
            _connStatus = DISCONNECTED;
        }

        void sendMessage(SerialPort &port, const Message &message) {
            port.send(MSG_MAGIC);
            port.send(message.msgId);
            port.send(port.crc16(message.data, message.len));
            port.send(message.len);
            if (message.len > 0) {
                port.send(message.data, message.len);
            }
            port.send(MSG_MAGIC);
            port.flush();
        }
    private:
        void onMessage(SerialPort &port, Message &message) {
            if (_mode == MODE_MASTER) {
                if (_connStatus == DISCONNECTED) {
                    if (message.msgId == MSG_SYNC) {
                        sendMessage(port, Message{MSG_CONN, 0, nullptr});
                        _connStatus = PROCESS;
                    }
                } else if (_connStatus == PROCESS) {
                    if (message.msgId == MSG_CONN) {
                        _connStatus = CONNECTED;
                        _callback.onConnect(port);
                    } else {
                        _connStatus = DISCONNECTED;
                    }
                }
            } else {
                if (message.msgId == MSG_SYNC) {
                    sendMessage(port, Message{MSG_SYNC, 0, nullptr});
                    _connStatus = PROCESS;
                } else if (_connStatus == PROCESS) {
                    if (message.msgId == MSG_CONN) {
                        sendMessage(port, Message{MSG_CONN, 0, nullptr});
                        _connStatus = CONNECTED;
                        _callback.onConnect(port);
                    } else {
                        _connStatus = DISCONNECTED;
                    }
                }
            }

            if (_connStatus == CONNECTED) {
                if (message.msgId == MSG_PING) {
                    sendMessage(port, Message{MSG_PONG, 0, nullptr});
                } else if (message.msgId == MSG_DOWN) {
                    onError(port, -1);
                } else {
                    _callback.onMessage(port, message);
                }
            }
        }
    };
}

#endif //ROVER_SERIALPORTCODEC_H
