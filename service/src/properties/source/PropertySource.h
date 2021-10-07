//
// Created by Ivan Kishchenko on 11.04.2021.
//

#pragma once

#include <string>
#include <string_view>
#include <memory>
#include "properties/NetworkProperties.h"
#include "properties/LoggingProperties.h"
#include "properties/HttpProperties.h"
#include "properties/GrpcProperties.h"
#include "properties/SerialProperties.h"
#include "properties/MqttProperties.h"
#include "properties/JoystickProperties.h"

class PropertySource {
public:
    typedef std::shared_ptr<PropertySource> Ptr;
public:
    virtual void getProperties(NetworkProperties& props) = 0;
    virtual void getProperties(LoggingProperties& props) = 0;
    virtual void getProperties(HttpProperties& props) = 0;
    virtual void getProperties(GrpcProperties& props) = 0;
    virtual void getProperties(SerialProperties& props) = 0;
    virtual void getProperties(MqttProperties& props) = 0;
    virtual void getProperties(JoystickProperties& props) = 0;
    ~PropertySource() = default;
};
