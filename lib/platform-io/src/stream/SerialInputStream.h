//
// Created by Ivan Kishchenko on 09.04.2021.
//

#ifndef PLATFORM_IO_SERIALINPUTSTREAM_H
#define PLATFORM_IO_SERIALINPUTSTREAM_H

#include <stream/InputStream.h>

class SerialInputStream : public InputStream {
    Stream& _stream;
public:
    explicit SerialInputStream(Stream &stream)
            : _stream(stream) {}

    bool hasNext() override {
        return _stream.available();
    }

    int readNext() override {
        return _stream.read();
    }
};

#endif //PLATFORM_IO_SERIALINPUTSTREAM_H
