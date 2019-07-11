//
// Created by Ivan Kishchenko on 2019-07-11.
//

#ifndef ROVER_PROPERTIESJSON_H
#define ROVER_PROPERTIESJSON_H


#include "Properties.h"

class PropertiesJson : public Properties {
public:
    std::string getString(const std::string &name) const override;

    int getInt(const std::string &name) const override;

    long getLong(const std::string &name) const override;

    double getDouble(const std::string &name) const override;

    std::list<std::string> getStringArray(const std::string &name) const override;
};


#endif //ROVER_PROPERTIESJSON_H
