//
// Created by Ivan Kishchenko on 2019-07-11.
//

#ifndef ROVER_PROPERTIESJSON_H
#define ROVER_PROPERTIESJSON_H


#include "Properties.h"
#include <boost/property_tree/ptree.hpp>

class PropertiesJson : public Properties {
public:
    void load(const std::string &filePath) override;

    std::string getString(const std::string &name) const override;

    bool getBoolean(const std::string &name) const override;

    int getInt(const std::string &name) const override;

    long getLong(const std::string &name) const override;

    double getDouble(const std::string &name) const override;

    std::list<std::string> getStringArray(const std::string &name) const override;

private:
    boost::property_tree::ptree tree;
};

#endif //ROVER_PROPERTIESJSON_H
