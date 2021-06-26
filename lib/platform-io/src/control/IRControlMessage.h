//
// Created by Ivan Kishchenko on 26.06.2021.
//

#ifndef PLATFORM_IO_IRCONTROLMESSAGE_H
#define PLATFORM_IO_IRCONTROLMESSAGE_H

#include "Registry.h"
#include <device/controller/IRRemoteButton.h>

class IRControlMessage : public etl::message<MSG_IR_CONTROL> {
    Button code;
public:
    explicit IRControlMessage(Button code) : code(code) {}
    Button getCode() const {
        return code;
    }
};


#endif //PLATFORM_IO_IRCONTROLMESSAGE_H
