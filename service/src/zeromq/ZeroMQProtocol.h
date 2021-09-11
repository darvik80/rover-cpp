//
// Created by Ivan Kishchenko on 28.08.2021.
//

#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include <cstdint>
#include <iostream>
#include <iomanip>

#include <algorithm>
#include <vector>
#include <functional>

#include "ZeroMQErrorCode.h"
#include "ZeroMQBuf.h"
#include "ZeroMQFlag.h"

#include <boost/asio.hpp>

struct ZeroMQVersion {
    uint8_t major{3};
    uint8_t minor{1};
};

class ZeroMQGreeting {
    ZeroMQVersion _version;
    std::string _mechanism{"NULL"};
    bool _isServer{false};
public:
    explicit ZeroMQGreeting(bool isServer)
            : _version{3, 1}, _mechanism("NULL"), _isServer(isServer) {

    }

    ZeroMQGreeting(const ZeroMQVersion &version, const std::string &mechanism, bool isServer)
            : _version(version), _mechanism(mechanism), _isServer(isServer) {}

    friend std::ostream &operator<<(std::ostream &out, const ZeroMQGreeting &greeting) {
        out << (uint8_t) 0xFF << std::setfill((char) 0x00) << std::setw(8) << (uint8_t) 0x00 << (uint8_t) 0x7F;
        out << (uint8_t) greeting._version.major << (uint8_t) greeting._version.minor;
        out << std::left << std::setw(20) << greeting._mechanism;
        out << (uint8_t) (greeting._isServer ? 0x01 : 0x00);
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

        greeting._version.major = inc.get();
        greeting._version.minor = inc.get();
        for (auto idx = 0; idx < 20; idx++) {
            auto ch = inc.get();
            if (ch) {
                greeting._mechanism += (char) ch;
            }
        }
        greeting._isServer = inc.get() > 0;
        inc.ignore(31);
        if (inc.eof()) {
            //inc.setstate(std::ios::badbit);
            return inc;

        }

        return inc;
    }
};


template<ZeroMQFlag flags, typename T>
class ZeroMQBin {
    T _val;

private:
    static T swapEndian(T val) {
        if (sizeof(val) == 1) {
            return val;
        }
        T res = val;
        void *buf = &res;
        static_assert(std::is_pod<T>::value, "swapEndian support POD type only");
        char *startIndex = static_cast<char *>((void *) buf);
        char *endIndex = startIndex + sizeof(T);
        std::reverse(startIndex, endIndex);

        return res;
    }

public:
    explicit ZeroMQBin(T val)
            : _val(val) {
    }

    explicit ZeroMQBin()
            : _val{} {
    }

    friend std::ostream &operator<<(std::ostream &out, const ZeroMQBin &b) {
        if (sizeof(_val) == sizeof(uint32_t)) {
            T val = swapEndian(b._val);
            out.write(reinterpret_cast<const char *>(&(val)), sizeof(T));
        } else {
            uint8_t flag = flags;
            if (b._val > 255) {
                flag |= flag_long;
                out << flag;
                T val = swapEndian(b._val);
                out.write(reinterpret_cast<const char *>(&(val)), sizeof(T));
            } else {
                out << flag << (uint8_t) b._val;
            }
        }
        return out;
    }

    friend std::istream &operator>>(std::istream &inc, ZeroMQBin &b) {
        if (sizeof(_val) == sizeof(uint32_t)) {
            inc.read(reinterpret_cast<char *>(&(b._val)), sizeof(T));
            b._val = swapEndian(b._val);
        } else {
            uint8_t flag;
            inc >> flag;
            inc.read(reinterpret_cast<char *>(&(b._val)), flag & flag_long ? sizeof(T) : sizeof(uint8_t));
            if (flag & flag_long) {
                b._val = swapEndian(b._val);
            }
        }
        return inc;
    }

    const T &getValue() const {
        return _val;
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

    explicit ZeroMQCommand(std::string_view name)
            : name(name) {}

    explicit ZeroMQCommand()
            : name{} {}

    [[nodiscard]] const std::string &getName() const {
        return name;
    }
};

struct ZeroMQMessage {
    std::vector<std::string> data;

    ZeroMQMessage &operator<<(std::string_view msg) {
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
    void onCommand(ZeroMQCommandHandler handler) {
        _cmdHandler = handler;
    }

    void onMessage(ZeroMQMessageHandler handler) {
        _msgHandler = handler;
    }

    std::error_code read(ZeroMQCharBuf &buf);
};

class ZeroMQEncoder {
public:
    std::error_code write(ZeroMQCharBuf &buf, ZeroMQCommand& cmd);
    std::error_code write(ZeroMQCharBuf &buf, ZeroMQMessage& msg);
};