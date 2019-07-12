//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-12.
//

#ifndef ROVER_PROPERTIESMAP_H
#define ROVER_PROPERTIESMAP_H


#include "Properties.h"
#include <map>
#include <string>

class PropertiesMap : public Properties {
private:
    std::map<std::string, std::string> tree;
public:
    void load(const std::string &filePath) override;

    std::string getString(const std::string &name) const override;

    bool getBoolean(const std::string &name) const override;

    int getInt(const std::string &name) const override;

    long getLong(const std::string &name) const override;

    double getDouble(const std::string &name) const override;

    std::list<std::string> getStringArray(const std::string &name) const override;

    void setString(const std::string &name, const std::string &value) {
        tree[name] = value;
    }
};


#endif //ROVER_PROPERTIESMAP_H
