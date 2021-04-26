//
// Created by Ivan Kishchenko on 08.04.2021.
//

#ifndef ROVER_INPUTSTREAM_H
#define ROVER_INPUTSTREAM_H

#include "CoreConfig.h"

class InputStream {
public:
    virtual bool hasNext() = 0;

    virtual int readNext() = 0;
};

#endif //ROVER_INPUTSTREAM_H
