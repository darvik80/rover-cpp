//
// Created by Ivan Kishchenko on 13.09.2020.
//

#ifndef MQTT_PINGREQMESSAGE_H
#define MQTT_PINGREQMESSAGE_H

#include "Message.h"

namespace mqtt::message {

    class PingReqMessage : public Message {
    public:
        PingReqMessage() : Message(MQTT_MSG_PINGREQ){ }
    };

}


#endif //MQTT_PINGREQMESSAGE_H
