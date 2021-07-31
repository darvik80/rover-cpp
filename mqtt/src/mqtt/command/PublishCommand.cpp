//
// Created by Ivan Kishchenko on 26.12.2020.
//

#include <mqtt/message/PubRelMessage.h>
#include <mqtt/message/PubRecMessage.h>
#include <mqtt/message/PubCompMessage.h>

#include <utility>
#include "PublishCommand.h"
#include "mqtt/message/PublishMessage.h"
#include "mqtt/message/PubAckMessage.h"

#include "mqtt/message/QoS.h"

#include "mqtt/logging/MqttLogger.h"

namespace mqtt {
    using namespace mqtt::message;

    boost::future<void> PublishCommand::execute() {
        auto promise = std::make_shared<boost::promise<void>>();
        auto msg = std::make_shared<message::PublishMessage>(_topic, _qos, getPacketIdentifier(), _data);

        if (_qos == QOS_AT_MOST_ONCE) {
            return request<PublishMessage>(msg);
        } else if (_qos == QOS_AT_LEAST_ONCE) {
            request<PublishMessage, PubAckMessage>(msg).then([promise](boost::future<PubAckMessage> ack) {
                try {
                    ack.get();
                    promise->set_value();
                } catch (boost::system::system_error &ex) {
                    promise->set_exception(ack.get_exception_ptr());
                }
            });
        } else if (_qos == QOS_AT_LEAST_ONCE) {
            request<PublishMessage, PubRecMessage>(msg).then([self = shared_from_this(), promise](boost::future<PubRecMessage> rec) {
                try {
                    auto rel = std::make_shared<PubRelMessage>();
                    rel->setPacketIdentifier(rec.get().getPacketIdentifier());
                    self->request<PubRelMessage, PubCompMessage>(rel).then([promise](boost::future<PubCompMessage> comp) {
                        try {
                            comp.get();
                            promise->set_value();
                        } catch (boost::system::system_error &ex) {
                            promise->set_exception(comp.get_exception_ptr());
                        }
                    });
                } catch (boost::system::system_error &ex) {
                    promise->set_exception(rec.get_exception_ptr());
                }
            });
        }

        return promise->get_future();
    }

}