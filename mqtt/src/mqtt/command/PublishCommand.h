//
// Created by Ivan Kishchenko on 26.12.2020.
//

#ifndef MQTT_PUBLISHCOMMAND_H
#define MQTT_PUBLISHCOMMAND_H

#include "mqtt/Command.h"
#include "mqtt/Client.h"

namespace mqtt {
    class PublishCommand : public Command, public std::enable_shared_from_this<PublishCommand> {
    private:
        std::string _topic{};
        ByteBuffer _data;
        uint8_t _qos;
    private:
        PublishCommand(const PublishCommand &origin) = default;

        PublishCommand(const Client::Ptr &client, std::string_view topic, uint8_t qos, const ByteBuffer &data)
                : Command(client), _topic(topic), _qos(qos), _data(data) {}

        PublishCommand(const Client::Ptr &client, std::string_view topic, uint8_t qos, std::string_view data)
                : Command(client), _topic(topic), _qos(qos), _data(data.begin(), data.end()) {
        }

        PublishCommand(const Client::Ptr &client, std::string_view topic, uint8_t qos, const char *data)
                : Command(client), _topic(topic), _qos(qos) {
            size_t size = strlen(data);
            _data.resize(size);
            std::copy((uint8_t *) data, (uint8_t *) (data + size), _data.begin());
        }

    public:
        template<typename ... T>
        static std::shared_ptr<PublishCommand> create(T &&... all) {
            return std::shared_ptr<PublishCommand>(new PublishCommand(std::forward<T>(all)...));
        }

        boost::future<void> execute() override;
    };

}


#endif //MQTT_PUBLISHCOMMAND_H
