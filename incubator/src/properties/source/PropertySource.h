//
// Created by Ivan Kishchenko on 11.04.2021.
//

#ifndef ROVER_PROPERTYSOURCE_H
#define ROVER_PROPERTYSOURCE_H

#include <string>
#include <string_view>
#include <memory>
#include <properties/NetworkProperties.h>

class PropertySource {
public:
    typedef std::shared_ptr<PropertySource> Ptr;
public:
    virtual void getProperties(NetworkProperties& props) = 0;
};


#endif //ROVER_PROPERTYSOURCE_H