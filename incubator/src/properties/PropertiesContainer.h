//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-12.
//

#ifndef ROVER_PROPERTIESCONTAINER_H
#define ROVER_PROPERTIESCONTAINER_H


#include "Properties.h"
#include <set>
#include <memory>

class PropertiesContainer : public Properties {
public:
    void load(const std::string &filePath) override;

    std::string getString(const std::string &name) const override;

    bool getBoolean(const std::string &name) const override;

    int getInt(const std::string &name) const override;

    long getLong(const std::string &name) const override;

    double getDouble(const std::string &name) const override;

    std::list<std::string> getStringArray(const std::string &name) const override;

    void addProperties(const SPProperties &properties) {
        _properties.insert(properties);
    }

private:
    typedef std::set<std::shared_ptr<Properties> > PropertiesVec;
    PropertiesVec _properties;
};


#endif //ROVER_PROPERTIESCONTAINER_H
