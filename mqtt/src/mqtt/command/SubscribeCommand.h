//
// Created by Ivan Kishchenko on 26.12.2020.
//

#ifndef MQTT_SUBSCRIBECOMMAND_H
#define MQTT_SUBSCRIBECOMMAND_H

#include "mqtt/UserStd.h"
#include "mqtt/Client.h"
#include "mqtt/Command.h"

#include <mqtt/message/SubscribeMessage.h>
#include <mqtt/message/SubAckMessage.h>

#include <utility>

namespace mqtt {

    class SubscribeCommand : public Command,
                             public std::enable_shared_from_this<SubscribeCommand> {
    private:
        std::string _topic{};
        uint16_t _qos{};
    private:
        SubscribeCommand(const Client::Ptr &client, std::string_view topic, uint16_t qos)
                : Command(client), _topic(topic), _qos(qos) {}
    public:
        template<typename ... T>
        static std::shared_ptr<SubscribeCommand> create(T &&... all) {
            return std::shared_ptr<SubscribeCommand>(new SubscribeCommand(std::forward<T>(all)...));
        }

        boost::future<void> execute() override;
    };
}

#endif //MQTT_SUBSCRIBECOMMAND_H
