//
// Created by Ivan Kishchenko on 01.05.2021.
//

#ifndef PLATFORM_IO_SERIALSERVICE_H
#define PLATFORM_IO_SERIALSERVICE_H

#include "Service.h"
#include "CoreConfig.h"
#include <serial/Protocol.h>
#include <etl/function.h>
#include <etl/optional.h>

typedef void(*callback)();

class SerialService : public BaseService {
    serial::ConnState _state{serial::IDLE};
    Stream& _stream;

    enum RecvState {
        IDLE,
        HEADER,
        BODY,
        FOOTER
    };

    RecvState _recvState{IDLE};
    uint8_t _cmd{0};
    uint16_t _len{0};
    etl::vector<uint8_t, 128> _buffer;

public:
    explicit SerialService(Registry& registry, Stream& stream);
    void postConstruct() override;

    void run() override;
    void preDestroy() override;

    void send(uint8_t msgId, const uint8_t* data, uint16_t size);
    void send(uint8_t msgId, const char* data) {
        send(msgId, (const uint8_t*)data, strlen(data));
    }
private:
    void onMessage(uint8_t msgId, etl::ivector<uint8_t>& data);
    void onReceive(Stream& stream);
    void setState(serial::ConnState newState);
};


#endif //PLATFORM_IO_SERIALSERVICE_H
