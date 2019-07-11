//
// Created by Ivan Kishchenko on 2019-07-11.
//

#ifndef ROVER_PROPERTIES_H
#define ROVER_PROPERTIES_H

#include <string>
#include <list>

class Properties {
public:
    virtual std::string getString(const std::string& name) const = 0;
    virtual int getInt(const std::string& name) const = 0;
    virtual long getLong(const std::string& name) const = 0;
    virtual double getDouble(const std::string& name) const = 0;
    virtual std::list<std::string> getStringArray(const std::string& name) const = 0;
};


#endif //ROVER_PROPERTIES_H
