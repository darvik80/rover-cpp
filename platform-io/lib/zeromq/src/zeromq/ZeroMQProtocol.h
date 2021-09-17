//
// Created by Ivan Kishchenko on 28.08.2021.
//

#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>
#include <iostream>
#include <iomanip>

#include <algorithm>
#include <vector>
#include <functional>

#include "ZeroMQBuf.h"
#include "ZeroMQFlag.h"

struct ZeroMQVersion {
    uint8_t major{3};
    uint8_t minor{1};

};

struct ZeroMQGreeting {
    ZeroMQVersion version{};
    std::string mechanism{"NULL"};
    bool isServer{false};
public:
    explicit ZeroMQGreeting(bool isServer)
            : mechanism("NULL"), isServer(isServer) {

    }

    ZeroMQGreeting(const ZeroMQVersion &version, const std::string& mechanism, bool isServer)
            : version(version), mechanism(mechanism), isServer(isServer) {}

    friend std::ostream &operator<<(std::ostream &out, const ZeroMQGreeting &greeting) {
        out << (uint8_t) 0xFF << std::setfill((char) 0x00) << std::setw(8) << (uint8_t) 0x00 << (uint8_t) 0x7F;
        out << (uint8_t) greeting.version.major << (uint8_t) greeting.version.minor;
        out << std::left << std::setw(20) << greeting.mechanism;
        out << (uint8_t) (greeting.isServer ? 0x01 : 0x00);
        out << std::setw(31) << (uint8_t) 0x00;

        return out;
    }

    friend std::istream &operator>>(std::istream &inc, ZeroMQGreeting &greeting) {
        if (inc.get() != 0xFF) {
            inc.setstate(std::ios::badbit);
            return inc;
        }
        inc.ignore(8);
        if (inc.get() != 0x7f) {
            inc.setstate(std::ios::badbit);
            return inc;
        }

        greeting.version.major = inc.get();
        greeting.version.minor = inc.get();
        for (auto idx = 0; idx < 20; idx++) {
            auto ch = inc.get();
            if (ch) {
                greeting.mechanism += (char) ch;
            }
        }
        greeting.isServer = inc.get() > 0;
        inc.ignore(31);
        if (inc.eof()) {
            return inc;

        }

        return inc;
    }
};


#define ZERO_MQ_CMD_READY "READY"
#define ZERO_MQ_PROP_IDENTIFY "Identify"
#define ZERO_MQ_PROP_SOCKET_TYPE "Socket-Type"

#define ZERO_MQ_CMD_SUBSCRIBE "SUBSCRIBE"
#define ZERO_MQ_CMD_CANCEL "CANCEL"
#define ZERO_MQ_PROP_SUBSCRIPTION "Subscription"

struct ZeroMQCommand {
    std::string name;
    std::unordered_map<std::string, std::string> props;

    explicit ZeroMQCommand(const std::string name)
            : name(name) {}

    explicit ZeroMQCommand()
            : name{} {}

    [[nodiscard]] const std::string &getName() const {
        return name;
    }
};

struct ZeroMQMessage {
    std::vector<std::string> data;

    ZeroMQMessage &operator<<(const std::string& msg) {
        data.emplace_back(msg);
        return *this;
    }
};

typedef std::function<void(const ZeroMQMessage &msg)> ZeroMQMessageHandler;
typedef std::function<void(const ZeroMQCommand &msg)> ZeroMQCommandHandler;

class ZeroMQDecoder {
    ZeroMQCommandHandler _cmdHandler;
    ZeroMQMessageHandler _msgHandler;

    std::unique_ptr<ZeroMQMessage> _msg;
public:
    void onCommand(const ZeroMQCommandHandler& handler) {
        _cmdHandler = handler;
    }

    void onMessage(const ZeroMQMessageHandler& handler) {
        _msgHandler = handler;
    }

    std::error_code read(ZeroMQCharBuf &buf);
};

class ZeroMQEncoder {
    std::error_code writeCmdSub(ZeroMQCharBuf &buf, ZeroMQCommand& cmd);
    std::error_code writeCmdReady(ZeroMQCharBuf &buf, ZeroMQCommand& cmd);
public:
    std::error_code write(ZeroMQCharBuf &buf, ZeroMQGreeting& greeting);
    std::error_code write(ZeroMQCharBuf &buf, ZeroMQCommand& cmd);
    std::error_code write(ZeroMQCharBuf &buf, ZeroMQMessage& msg);
};