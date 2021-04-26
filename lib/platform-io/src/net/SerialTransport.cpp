//
// Created by Kishchenko, Ivan on 4/6/21.
//

#include "SerialTransport.h"

SerialTransport::SerialTransport(HardwareSerial &serial, unsigned long baud)
        : _serial(serial), _stream(serial) {
    serial.begin(baud);
}

void SerialTransport::onMessage(const uint8_t *data, size_t size) {

}

int SerialTransport::send(const uint8_t *data, size_t size) {
    _serial.write(data, size);
    return 0;
}

int SerialTransport::setup() {
    return 0;
}

int SerialTransport::loop() {
    if (_comparator.process(_stream)) {
        switch (_state) {
            case SERIAL_WAIT_CONF:
                _comparator = StreamComparator("CONF");
                break;
            case SERIAL_WAIT_CONN:
                break;
            case SERIAL_CONN:
                break;
            default:
                break;
        }
    }
    return 0;
}

