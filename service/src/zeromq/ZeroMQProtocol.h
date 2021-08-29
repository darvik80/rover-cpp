//
// Created by Ivan Kishchenko on 28.08.2021.
//

#ifndef ROVER_ZEROMQPROTOCOL_H
#define ROVER_ZEROMQPROTOCOL_H


#include <string>
#include <unordered_map>
#include <optional>
#include <streambuf>
#include <cstdint>
#include <iostream>
#include <iomanip>

#include <algorithm>

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

struct ZeroMQGreeting {
    ZeroMQVersion version;
    std::string mechanism{"NULL"};
    bool isServer{false};
public:
    friend std::ostream &operator<<(std::ostream &out, ZeroMQGreeting &greeting) {
        out.exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);
        out << (uint8_t) 0xFF << std::setfill((char) 0x00) << std::setw(8) << (uint8_t) 0x00 << (uint8_t) 0x7F;
        out << (uint8_t) greeting.version.major << (uint8_t) greeting.version.minor;
        out << std::left << std::setw(20) << greeting.mechanism;
        out << (uint8_t) (greeting.isServer ? 0x01 : 0x00);
        out << std::setw(31) << (uint8_t) 0x00;

        return out;
    }

    friend std::istream &operator>>(std::istream &inc, ZeroMQGreeting &greeting) {
        inc.exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);
        if (inc.get() != 0xFF) {
            throw std::invalid_argument("invalid greeting #1");
        }

        inc.ignore(8);

        if (inc.get() != 0x7f) {
            throw std::invalid_argument("invalid greeting #2");
        }

        inc >> greeting.version.major >> greeting.version.minor;
        char mechanism[20];
        inc.read(mechanism, sizeof(mechanism));
        greeting.mechanism = (const char *) mechanism;
        greeting.isServer = inc.get();
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
        inc.exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);

        if ((inc.peek() & flag_cmd) == 0) {
            throw std::invalid_argument("invalid command #1");
        }
        ZeroMQBin<flag_cmd, uint64_t> cmdZSize;
        inc >> cmdZSize;

        std::size_t left = cmdZSize.getValue();
        int cmdSize = inc.get();
        for (int idx = 0; idx < cmdSize; ++idx) {
            cmd._name += (char) inc.get();
        }
        left -= cmdSize + 1;
        while (left) {
            std::string prop, val;
            int propSize = inc.get();
            for (int idx = 0; idx < propSize; ++idx) {
                prop += (char) inc.get();
            }
            ZeroMQBin<flag_msg, uint32_t> valSize;
            inc >> valSize;
            for (int idx = 0; idx < valSize.getValue(); ++idx) {
                val += (char) inc.get();
            }

            cmd._props.emplace(prop, val);
            left -= (propSize + 1 + valSize.getValue() + 4);
        }

        return inc;
    }
};

class ZeroMQCommandReady : public ZeroMQCommand {
public:
    ZeroMQCommandReady()
            : ZeroMQCommand("READY") {}
};


class ZeroMQMessage {
    std::vector<std::string> _data;
public:
    ZeroMQMessage &operator<<(std::string_view msg) {
        _data.emplace_back(msg);
        return *this;
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
            std::string val;
            for (int idx = 0; idx < size.getValue(); ++idx) {
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

template<std::size_t SIZE, class CharT = char>
class ZeroMQBuf : public std::basic_streambuf<CharT> {
    std::array<CharT, SIZE> _buf;
public:
    using Base = std::basic_streambuf<CharT>;
    using int_type = typename Base::int_type;

    ZeroMQBuf()
            : _buf{} {
        Base::setp(_buf.begin(), _buf.end());
        //Base::setg(_buf.begin(), _buf.begin(), _buf.begin());
    }

protected:
    int overflow(int_type val) override {
//        ptrdiff_t diff = Base::epptr() - Base::pbase() + 1;
//        Base::setp(_buf.begin(), _buf.begin() + diff);
//
//        Base::setg(_buf.begin(),  _buf.begin(), _buf.begin() + diff);
        return Base::overflow(val);
    }

    std::streamsize xsputn(const CharT *__s, std::streamsize __n) override {
        ptrdiff_t diff = Base::pptr() - Base::pbase()+__n;
        Base::setg(_buf.begin(),  _buf.begin(), _buf.begin() + diff);
        return Base::xsputn(__s, __n);
    }

public:
    std::string dump() {
        ptrdiff_t diff = Base::pptr() - Base::pbase();
        return ZeroMQUtils::netDump((uint8_t*)_buf.begin(), diff);
    }
};

#endif //ROVER_ZEROMQPROTOCOL_H
