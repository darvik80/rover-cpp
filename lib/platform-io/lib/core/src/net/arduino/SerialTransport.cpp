//
// Created by Kishchenko, Ivan on 4/6/21.
//

#include "SerialTransport.h"

SerialTransport::SerialTransport(HardwareSerial &serial, unsigned long baud, uint8_t config)
        : _serial(serial) {
    serial.begin(baud, config);
}

void SerialTransport::onMessage(const uint8_t *data, size_t size) {

}

int SerialTransport::send(const char *data, size_t size) {
    _serial.write(data, size);
    return 0;
}

int SerialTransport::setup() {
    return 0;
}

int SerialTransport::loop() {
    return 0;
}

