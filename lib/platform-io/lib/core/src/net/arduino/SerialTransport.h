//
// Created by Kishchenko, Ivan on 4/6/21.
//

#ifndef PLATFORM_IO_SERIALTRANSPORT_H
#define PLATFORM_IO_SERIALTRANSPORT_H

#include "net/Transport.h"
#include <Service.h>

enum SerialState {
    SERIAL_IDLE,
    SERIAL_READY_SYNC,
    SERIAL_WAIT_CONF,
    SERIAL_WAIT_CONN,
    SERIAL_CONN,
};

class SerialTransport : public Transport, public Service {
private:
    HardwareSerial& _serial;
    SerialState _state{SERIAL_IDLE};
public:
    SerialTransport(HardwareSerial &serial, unsigned long baud);

    int setup() override;

    int loop() override;

    void onMessage(const uint8_t *data, size_t size) override;

    int send(const char *data, size_t size) override;
};


#endif //PLATFORM_IO_SERIALTRANSPORT_H
