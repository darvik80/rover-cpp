//
// Created by Ivan Kishchenko on 28.08.2021.
//

#ifndef PLATFORM_IO_ZEROMQTRANSPORTPROTOCOL_H
#define PLATFORM_IO_ZEROMQTRANSPORTPROTOCOL_H

#include <string>
#include <streambuf>
#include <cstdint>

struct ZeroMQVersion {
    int major{3};
    int minor{0};
};

struct ZeroMQGreeting {
    ZeroMQVersion version;
    bool isServer;
};

struct ZeroMQCommandData {

};

struct ZeroMQCommand {
    std::string name;
    ZeroMQCommandData data;
};

class ZeroMQStream : public std::strea {

};

#endif //PLATFORM_IO_ZEROMQTRANSPORTPROTOCOL_H
