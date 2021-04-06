//
// Created by Kishchenko, Ivan on 4/6/21.
//

#ifndef PLATFORM_IO_SERIALTRANSPORT_H
#define PLATFORM_IO_SERIALTRANSPORT_H

#include "net/Transport.h"
#include <Arduino.h>
#include <Service.h>

class SerialTransport : public Transport, public Service {
private:
    HardwareSerial& _serial;
public:
    SerialTransport(HardwareSerial &serial, unsigned long baud, uint8_t config = SERIAL_8N1);

    int setup() override;

    int loop() override;

    void onMessage(const uint8_t *data, size_t size) override;

    int send(const char *data, size_t size) override;
};


#endif //PLATFORM_IO_SERIALTRANSPORT_H
