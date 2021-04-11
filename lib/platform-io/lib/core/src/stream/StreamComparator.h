//
// Created by Ivan Kishchenko on 08.04.2021.
//

#ifndef ROVER_STREAMCOMPARATOR_H
#define ROVER_STREAMCOMPARATOR_H

#include "InputStream.h"

class StreamComparator {
    const char *_data;
    size_t _offset{0};
public:
    explicit StreamComparator(const char *data)
            : _data(data) {}

    bool process(InputStream &stream) {
        while (stream.hasNext()) {
            int ch = stream.readNext();
            if (_data[_offset++] != ch) {
                _offset = 0;
            }

            if (_data[_offset] == 0 && stream.hasNext()) {
                _offset = 0;
            }
        }

        return _data[_offset] == 0;
    }
};

#endif //ROVER_STREAMCOMPARATOR_H
