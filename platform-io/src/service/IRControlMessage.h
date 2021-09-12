//
// Created by Ivan Kishchenko on 26.06.2021.
//

#ifndef PLATFORM_IO_IRCONTROLMESSAGE_H
#define PLATFORM_IO_IRCONTROLMESSAGE_H

#include "Const.h"
#include <device/controller/IRRemoteButton.h>
#include <etl/message.h>

class IRControlMessage : public etl::message<MSG_IR_CONTROL> {
    Button code;
public:
    explicit IRControlMessage(Button code) : code(code) {}
    Button getCode() const {
        return code;
    }
};


#endif //PLATFORM_IO_IRCONTROLMESSAGE_H
