//
// Created by Ivan Kishchenko on 13.09.2020.
//

#ifndef MQTT_PINGRESPMESSAGE_H
#define MQTT_PINGRESPMESSAGE_H

#include "Message.h"

namespace mqtt::message {

    class PingRespMessage : public Message {
    public:
        PingRespMessage() : Message(MQTT_MSG_PINGRESP){ }
    };

}


#endif //MQTT_PINGRESPMESSAGE_H
