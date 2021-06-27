//
// Created by Ivan Kishchenko on 11.04.2021.
//

#ifndef ROVER_PROPERTYSOURCE_H
#define ROVER_PROPERTYSOURCE_H

#include <string>
#include <string_view>
#include <memory>
#include "properties/NetworkProperties.h"
#include "properties/LoggingProperties.h"
#include "properties/HttpProperties.h"
#include "properties/SerialProperties.h"

class PropertySource {
public:
    typedef std::shared_ptr<PropertySource> Ptr;
public:
    virtual void getProperties(NetworkProperties& props) = 0;
    virtual void getProperties(LoggingProperties& props) = 0;
    virtual void getProperties(HttpProperties& props) = 0;
    virtual void getProperties(SerialProperties& props) = 0;
    ~PropertySource() = default;
};


#endif //ROVER_PROPERTYSOURCE_H
