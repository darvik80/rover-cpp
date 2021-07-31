//
// Created by Ivan Kishchenko on 26.12.2020.
//

#ifndef MQTT_PUBLISHER_H
#define MQTT_PUBLISHER_H

#include "command/PublishCommand.h"
#include "Client.h"

namespace mqtt {

    class Publisher {
        Client::Ptr _client;
        std::string _topic;
        uint8_t _qos;
    private:
        void exec(Command::Ptr cmd) const {
            cmd->execute().then([topic = _topic](boost::future<void> future) {
                try {
                    future.get();
                    log::debug("[pub] published to: {}", topic);
                } catch (boost::system::system_error& ex) {
                    log::debug("[pub] failed publish to: {}, error", topic, ex.what());
                }
            });
        }
    public:
        Publisher(const Client::Ptr &client, std::string_view topic, uint8_t qos)
                : _client(client), _topic(topic), _qos(qos) {}

        void publish(const ByteBuffer& data) const {
            exec(PublishCommand::create(_client, _topic, _qos, data));
        }

        void publish(std::string_view data) const {
            exec(PublishCommand::create(_client, _topic, _qos, data));
        }

        void publish(const char* data) const {
            exec(PublishCommand::create(_client, _topic, _qos, data));
        }
    };

}


#endif //MQTT_PUBLISHER_H
