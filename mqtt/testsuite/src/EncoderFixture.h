//
// Created by Kishchenko, Ivan on 9/16/20.
//

#ifndef MQTT_ENCODERFIXTURE_H
#define MQTT_ENCODERFIXTURE_H

#include "mqtt/Encoder.h"
#include <boost/test/unit_test.hpp>

class EncoderFixture {
public:
    mqtt::Encoder encoder;
};


#endif //MQTT_ENCODERFIXTURE_H
