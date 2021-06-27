//
// Created by Ivan Kishchenko on 09.04.2021.
//

#ifndef ROVER_BOOSTINPUTSTREAM_H
#define ROVER_BOOSTINPUTSTREAM_H

#include <stream/InputStream.h>

class BoostInputStream : public InputStream {
public:
    bool hasNext() override {
        return false;
    }

    int readNext() override {
        return 0;
    }

};


#endif //ROVER_BOOSTINPUTSTREAM_H
