//
// Created by Ivan Kishchenko on 05.09.2021.
//

#pragma once

#include <string>
#include <streambuf>
#include <array>
#include "ZeroMQUtils.h"
#include <cstring>

template<class CharT = char>
class ZeroMQBuf : public std::basic_streambuf<CharT> {
protected:
    ZeroMQBuf() {}
public:
    using Base = std::basic_streambuf<CharT>;
    using int_type = typename Base::int_type;
    using pos_type = typename Base::pos_type;
    using off_type = typename Base::off_type;

    ZeroMQBuf(CharT *data, size_t size) {
        Base::setp(data, data + size);
        Base::setg(data, data, data);
    }

public:
    std::streamsize xsputn(const CharT *s, std::streamsize n) override {
        Base::setg(Base::pbase(), Base::pbase(), Base::pptr() + n);
        return Base::xsputn(s, n);
    }

public:
    std::string dump() {
        return ZeroMQUtils::netDump((uint8_t *) Base::pbase(), Base::pptr() - Base::pbase());
    }

    std::size_t size() {
        return Base::pptr() - Base::pbase();
    }

    std::size_t available() {
        return Base::epptr() - Base::pbase() - Base::in_avail();
    }

    CharT *data() {
        return Base::pbase();
    }

    bool append(const CharT *s, std::streamsize n) {
        return sputn(s, n) == n;
    }

    bool consume(std::size_t needConsume) {
        if (needConsume > size()) {
            return false;
        }

        size_t avail = size() - needConsume;
        std::memcpy(Base::pbase(), Base::pbase() + needConsume, avail);
        Base::setg(Base::pbase(), Base::pbase(), Base::pbase() + avail);
        Base::setp(Base::pbase(), Base::epptr());
        Base::pbump(avail);

        return true;
    }

    bool move(std::size_t size) {
        if (size > (Base::epptr()-Base::pptr())) {
            return false;
        }

        Base::pbump(size);

        return false;
    }
};

typedef ZeroMQBuf<> ZeroMQCharBuf;

template<std::size_t SIZE, class CharT = char>
class ZeroMQBufFix : public ZeroMQBuf<CharT> {
    std::array<CharT, SIZE> _buf{};
public:
    ZeroMQBufFix() {
        ZeroMQBuf<CharT>::setp(_buf.begin(), _buf.end());
        ZeroMQBuf<CharT>::setg(_buf.begin(), _buf.begin(), _buf.begin());
    }
};
