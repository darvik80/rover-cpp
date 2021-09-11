//
// Created by Ivan Kishchenko on 28.08.2021.
//

#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include <streambuf>
#include <cstdint>
#include <iostream>
#include <iomanip>

#include <algorithm>
#include <vector>

#include "ZeroMQUtils.h"

enum ZeroMQFlag {
    flag_msg = 0x00,
    flag_cmd = 0x04,
    flag_long = 0x02,
    flag_more = 0x01,
};

struct ZeroMQVersion {
    uint8_t major{3};
    uint8_t minor{0};
};

class ZeroMQGreeting {
    ZeroMQVersion _version;
    std::string _mechanism{"NULL"};
    bool _isServer{false};
public:
    explicit ZeroMQGreeting(bool isServer)
            : _version{3, 0}, _mechanism("NULL"), _isServer(isServer) {

    }

    const ZeroMQVersion &getVersion() const {
        return _version;
    }

    const std::string &getMechanism() const {
        return _mechanism;
    }

    bool isIsServer() const {
        return _isServer;
    }

    ZeroMQGreeting(const ZeroMQVersion &version, std::string_view mechanism, bool isServer)
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
        greeting._mechanism.clear();
        for (auto idx = 0; idx < 20; idx++) {
            auto ch = inc.get();
            if (ch) {
                greeting._mechanism += (char) ch;
            }
        }
        greeting._isServer = inc.get() > 0;
        inc.ignore(31);

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

class ZeroMQCommand {
    std::string _name;
    std::unordered_map<std::string, std::string> _props;
public:
    explicit ZeroMQCommand(std::string_view name)
            : _name(name) {}

    explicit ZeroMQCommand()
            : _name{} {}

    [[nodiscard]] const std::string &getName() const {
        return _name;
    }

    void setProperty(std::string_view prop, std::string_view val) {
        _props.emplace(prop, val);
    }

    std::string getProperty(std::string_view prop) {
        return _props[prop.data()];
    }

    friend std::ostream &operator<<(std::ostream &out, const ZeroMQCommand &cmd) {
        std::size_t size = cmd.getName().size() + 1;
        for (const auto &prop: cmd._props) {
            size += 1 + prop.first.size();
            size += 4 + prop.second.size();
        }

        out << ZeroMQBin<flag_cmd, uint64_t>(size) << (uint8_t) cmd._name.size() << cmd._name;
        for (const auto &prop: cmd._props) {
            out << (uint8_t) prop.first.size() << prop.first;
            out << ZeroMQBin<flag_msg, uint32_t>(prop.second.size()) << prop.second;
        }

        return out;
    }

    friend std::istream &operator>>(std::istream &inc, ZeroMQCommand &cmd) {
        if ((inc.peek() & flag_cmd) == 0) {
            inc.setstate(std::ios_base::eofbit);
            return inc;
        }
        ZeroMQBin<flag_cmd, uint64_t> cmdZSize;
        inc >> cmdZSize;
        if (!inc) {
            return inc;
        }

        std::size_t left = cmdZSize.getValue();
        if (inc.rdbuf()->in_avail() < left) {
            inc.setstate(std::ios::badbit);
            return inc;
        }

        int cmdSize = inc.get();
        for (int idx = 0; idx < cmdSize; ++idx) {
            cmd._name += (char) inc.get();
        }
        left -= cmdSize + 1;
        if (cmd._name == ZERO_MQ_CMD_READY) {
            while (left) {
                std::string prop, val;
                std::size_t propSize = inc.get();
                for (std::size_t idx = 0; idx < propSize; ++idx) {
                    prop += (char) inc.get();
                }
                ZeroMQBin<flag_msg, uint32_t> valSize;
                inc >> valSize;
                for (std::size_t idx = 0; idx < valSize.getValue(); ++idx) {
                    val += (char) inc.get();
                }

                cmd._props.emplace(prop, val);
                left -= (propSize + 1 + valSize.getValue() + 4);
            }
        } else if (cmd._name == ZERO_MQ_CMD_SUBSCRIBE || cmd._name == ZERO_MQ_CMD_CANCEL) {
            std::string topic;
            for (std::size_t idx = 0; idx < left; ++idx) {
                topic += (char) inc.get();
            }

            cmd._props.emplace(ZERO_MQ_PROP_SUBSCRIPTION, topic);
        } else {
            for (std::size_t idx = 0; idx < left; ++idx) {
                inc.get();
            }
        }

        return inc;
    }
};

class ZeroMQMessage {
    std::vector<std::string> _data;
public:
    ZeroMQMessage &operator<<(std::string_view msg) {
        _data.emplace_back(msg);
        return *this;
    }

    const std::vector<std::string> &getData() const {
        return _data;
    }

    friend std::ostream &operator<<(std::ostream &out, const ZeroMQMessage &msg) {
        const auto &data = msg._data;
        if (!data.empty()) {
            for (std::size_t idx = 0; idx < data.size() - 1; ++idx) {
                const auto &item = data.at(idx);
                out << ZeroMQBin<flag_more, uint64_t>(item.size()) << item;
            }

            const auto &item = data.back();
            out << ZeroMQBin<flag_msg, uint64_t>(item.size()) << item;
        }

        return out;
    }

    friend std::istream &operator>>(std::istream &inc, ZeroMQMessage &msg) {
        msg._data.clear();
        while (!inc.eof()) {
            uint8_t flag = inc.peek();
            ZeroMQBin<flag_more, uint64_t> size;
            inc >> size;
            if (!inc) {
                return inc;
            }
            if (inc.rdbuf()->in_avail() < size.getValue()) {
                inc.setstate(std::ios::badbit);
                return inc;
            }
            std::string val;
            for (size_t idx = 0; idx < size.getValue(); ++idx) {
                val += (char) inc.get();
            }
            msg._data.emplace_back(val);

            if ((flag & flag_more) == 0) {
                break;
            }
        }

        return inc;
    }
};

template<class CharT = char>
class ZeroMQBuf : public std::basic_streambuf<CharT> {
public:
    using Base = std::basic_streambuf<CharT>;
    using char_type = typename Base::char_type;
    using int_type = typename Base::int_type;

    virtual std::size_t size() = 0;
    virtual const char_type* data() = 0;
    virtual std::string dump() = 0;
};

template<std::size_t SIZE>
class ZeroMQBufFix : public ZeroMQBuf<> {
    std::array<char, SIZE> _buf;
public:
    using char_type = typename ZeroMQBuf::char_type;
    using int_type = typename ZeroMQBuf::int_type;

    ZeroMQBufFix(CharT *data, size_t size)
            : _buf{} {
        setp(_buf.begin(), _buf.end());
        setg(_buf.begin(), _buf.begin(), _buf.begin());
    }

protected:

    std::streamsize xsputn(const char_type *s, std::streamsize n) override {
        ptrdiff_t diff = ZeroMQBuf::pptr() - Base::pbase() + n;
        ZeroMQBuf::setg(_buf.begin(), _buf.begin(), _buf.begin() + diff);
        return Base::xsputn(s, n);
    }

public:
    std::string dump() override {
        ptrdiff_t diff = ZeroMQBuf::pptr() - ZeroMQBuf::pbase();
        return ZeroMQUtils::netDump((uint8_t *) _buf.begin(), diff);
    }

    std::size_t size() override {
        return ZeroMQBuf::pptr() - ZeroMQBuf::pbase();
    }
    const char_type* data() override {
        return ZeroMQBuf::pbase();
    }

    void compress() {
        std::size_t inAvail = Base::in_avail();
        std::memcpy(Base::pbase(), Base::pptr()-Base::in_avail(), inAvail);
        Base::setg(Base::pbase(), Base::pbase(), Base::pbase() + inAvail);
        Base::setp(Base::pbase(), Base::epptr());
        Base::pbump(inAvail);
    }
};
