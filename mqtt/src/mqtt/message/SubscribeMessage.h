//
// Created by Ivan Kishchenko on 12.09.2020.
//

#ifndef MQTT_SUBSCRIBEMESSAGE_H
#define MQTT_SUBSCRIBEMESSAGE_H

#include <string>
#include <vector>
#include "Message.h"
#include "MessagePacketIdentifier.h"

namespace mqtt::message {

    class SubscribePayload {
    private:
        std::string _topicFilter;
        union {
            uint8_t all;    /**< all connack flags */
            struct {
                unsigned int reserved : 7;    /**< message type nibble */
                unsigned int qos : 3;    /**< message type nibble */
            } bits;
        } _requestedQos{};     /**< connack flags byte */
    public:
        SubscribePayload(const std::string &topicFilter, uint8_t qos);

        [[nodiscard]] const std::string &getTopicFilter() const;

        [[nodiscard]] uint8_t getQos() const;
    };

    class SubscribeMessage : public Message, public MessagePacketIdentifier {
    private:
        std::vector<SubscribePayload> _topics;
    public:
        SubscribeMessage() : Message(MQTT_MSG_SUBSCRIBE) { }

        SubscribeMessage(std::string_view topicFilter, uint8_t qos, uint16_t pid) : Message(MQTT_MSG_SUBSCRIBE) {
            addTopic(topicFilter, qos);
            setPacketIdentifier(pid);
        }

        [[nodiscard]] const std::vector<SubscribePayload> &getTopics() const;


        void addTopic(std::string_view topicFilter, uint8_t qos);
    };

}


#endif //MQTT_SUBSCRIBEMESSAGE_H
