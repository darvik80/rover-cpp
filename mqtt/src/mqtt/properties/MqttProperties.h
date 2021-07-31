//
// Created by Kishchenko, Ivan on 11/30/20.
//

#ifndef MQTT_CONNECTIONPROPERTIES_H
#define MQTT_CONNECTIONPROPERTIES_H

#include <string>

namespace mqtt {

        struct MqttProperties {
            std::string address;
            int port;

            std::string username;
            std::string password;

            std::string clientName;
        };

    }

#endif //MQTT_CONNECTIONPROPERTIES_H
